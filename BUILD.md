# Build

Every script is in `tools/` and runs from the repo root. The release builds,
`win-x64` and `win-arm64`, are cross-compiled on Fedora with llvm-mingw
against a Qt built for them; the two differ only in the target named. MSYS2
builds x64 natively, for quick work on a Windows box.

| Way | Once | Build | Package |
|---|---|---|---|
| [Fedora, no podman](#fedora-no-podman) | `sudo tools/toolkit-env.sh ARCH install` | `tools/build-cross.sh ARCH` | `tools/deploy-cross.sh ARCH` |
| [Podman, toolkit built locally](#podman-toolkit-built-locally) | | `TOOLKIT_IMAGE_SOURCE=local tools/build-container.sh ARCH` | `TOOLKIT_IMAGE_SOURCE=local tools/deploy-container.sh ARCH` |
| [Podman, toolkit from ghcr.io](#podman-toolkit-from-ghcrio) | | `tools/build-container.sh ARCH` | `tools/deploy-container.sh ARCH` |
| [Windows, MSYS2](#windows-msys2) (x64) | see below | `tools/build.sh` | `tools/deploy.sh` |

`ARCH` is `x64` or `arm64`. A cross build lands in `build-ARCH/` and
packages into `dist-ARCH/`; MSYS2 uses `build/` and `dist/`. Run a package's
`WinDiskImager.exe` **as Administrator**, which raw device access needs.

## Fedora, no podman

Install the toolkit once, for one target or both. Most of it is Qt, so
expect it to take a while:

```
sudo tools/toolkit-env.sh x64 install      # or arm64, or all
```

Then:

```
tools/build-cross.sh x64
tools/deploy-cross.sh x64
```

The toolkit is `/opt/llvm-mingw`, shared by both targets, and a sysroot per
target, `/opt/win-x64` and `/opt/win-arm64`. `tools/toolkit-env.sh ARCH check`
says whether one is complete, and `stale` whether anything it was built from
has been updated since; run `install` again to rebuild it.

## Podman, toolkit built locally

The same toolkit, built into an image here from the working tree, on
`quay.io/fedora/fedora-minimal:latest`. Nothing is installed on the host.
For working on the toolkit itself, or without network access to ghcr.io:

```
TOOLKIT_IMAGE_SOURCE=local tools/build-container.sh x64
TOOLKIT_IMAGE_SOURCE=local tools/deploy-container.sh x64
```

The first build makes the image, both targets in one; each later build first
checks it for updates, and rebuilds it if it has fallen behind (see
[Keeping the toolkit current](#keeping-the-toolkit-current)).

## Podman, toolkit from ghcr.io

The default: the published image, `ghcr.io/peacepenguin/llvm-mingw-qt6`, so
there is no Qt to build. This is what CI does.

```
tools/build-container.sh x64
tools/deploy-container.sh x64
```

The build pulls the image first, which fetches nothing if it has not
changed. If it cannot be pulled and there is no copy here, the build makes
it locally instead.

## Windows, MSYS2

Once, in a non-admin PowerShell:

```
winget install --id MSYS2.MSYS2 -e --source winget
```

then in the **MSYS2 UCRT64** shell, from the repo root:

```
pacman -Suy
pacman -S --needed $(bash tools/build-env.sh packages-msys2)
```

Then, any time:

```
tools/build.sh
tools/deploy.sh
```

`build/WinDiskImager.exe` only runs inside the MSYS2 shell, where its Qt DLLs
are; `dist/` is the standalone copy.

## Options

Every build script takes these after the target (MSYS2's take them alone):

- **`clean`** drops the build directory first. Otherwise ninja stays
  incremental: a no-op rebuild is well under a second.
- **`test`** builds a binary that asks for no elevation -- see
  [Working on the interface](#working-on-the-interface). Neither deploy script
  will package one.

And from the environment:

- **`BUILD_DIR=...`** builds somewhere other than `build-ARCH/`.
- **`W32DI_REFRESH=0`** skips the container build's update check or pull, for
  working offline.
- **`GITHUB_TOKEN=...`** lifts GitHub's limit of 60 anonymous API requests an
  hour for the one lookup a toolkit build makes, llvm-mingw's newest release.
  It is passed in as a podman secret, never stored in the image:
  `GITHUB_TOKEN=$(gh auth token) TOOLKIT_IMAGE_SOURCE=local tools/build-container.sh arm64`.

A cross build fails if what comes out is not a Windows binary for its target.
A Fedora host's build and a container's share `build-ARCH/`; switching
between them drops the cmake cache once, and says so.

## How the cross build works

[tools/toolkit-env.sh](tools/toolkit-env.sh) builds the toolkit, the same way
on a Fedora host as in the image: the host packages and llvm-mingw, then for
each target zlib, xz, zstd, bzip2 and Qt into its sysroot. Nothing is pinned;
each is the upstream tarball from Fedora's source RPM, checked against
Fedora's signature, with none of Fedora's patches:

| Library | Source RPM of |
|---|---|
| Qt (qtbase, qtsvg, qttranslations) | `qt6-qtbase`, `qt6-qtsvg`, `qt6-qttranslations`: the builds the host Qt, whose `moc`, `rcc` and `uic` the cross build uses, is installed from |
| zlib, zstd, bzip2 | `mingw64-zlib`, `mingw64-zstd`, `mingw64-bzip2` (Fedora's native zlib is zlib-ng) |
| xz | `xz` (`mingw64-xz` is 5.2.4; the multi-threaded decoder needs 5.4) |

llvm-mingw is its newest GitHub release, checked against GitHub's SHA-256;
`TOOLKIT_LLVM_MINGW_PIN` holds it at one. Qt has only what a widgets app
needs -- no ICU, OpenSSL, D-Bus or SQL -- so a package is four Qt DLLs,
`libc++.dll` and `libunwind.dll`, and the four compression libraries.

Each library records its version, licence and source RPM in its sysroot's
manifest, `share/llvm-mingw-qt6/sources.tsv`, with its licence files beside
it. The deploy ships those and lists them in `THIRD-PARTY-NOTICES.txt`, which
ends by naming the image the package was built in, by digest; a file the
manifest does not know stops the deploy.

`tools/build-cross.sh` and `tools/deploy-cross.sh` are the cross build. The
container scripts run them inside the image and nothing else;
[tools/build-env.sh](tools/build-env.sh) holds what every build shares.

## The toolkit image

`tools/Containerfile.toolkit` builds `llvm-mingw-qt6` in stages -- the host
part, each target's sysroot on it, then the host part with both sysroots
copied in -- so it holds each thing once and no build trees. Nothing in it is
particular to WinDiskImager.

[.github/workflows/toolkit.yml](.github/workflows/toolkit.yml) publishes it
to `ghcr.io/peacepenguin/llvm-mingw-qt6` with `tools/toolkit-publish.sh`,
after building and packaging WinDiskImager in it for both targets:

| Tag | |
|---|---|
| `r3-YYYYMMDD` | one build, kept |
| `r3` | the newest build for toolkit revision 3: what the build pulls |
| `latest` | the newest build |

`TOOLKIT_IMAGE_REVISION` in `tools/toolkit-env.sh` is raised whenever what the
image holds or how it is laid out changes, so a build never runs in an image
made for another revision; until the new one is published, the build makes it
locally. Raising it is also what gets the new image published: the toolkit
workflow builds for a revision it has not published, and otherwise only for
updates.

### Keeping the toolkit current

An image is behind when `fedora-minimal:latest` is a newer Fedora release
than it was built on, or when `tools/toolkit-env.sh all stale` finds a newer
source RPM for any library or Qt, or a newer llvm-mingw. Updates to build
tools alone -- cmake, gcc, mesa -- do not count, being shipped nowhere.

- The toolkit workflow checks the published image **daily**, and **when a
  file that goes into the image changes** on master, and publishes a new one
  only if there is none for the current revision or it is behind. **By hand**
  from the Actions tab it can be forced, for an install-step change that did
  not raise the revision. A
  daily check that cannot be made -- the registry or base image will not
  pull, or dnf or GitHub errs -- fails the run, so GitHub reports it instead
  of the image quietly going stale.
- A **locally built** image is checked before each build and rebuilt, from
  scratch under the same tag, if it is behind; `podman image prune` reclaims
  the old one.
- The deploy scripts never refresh or pull, so a package is always made from
  the image its build used.

To publish by hand, after `podman login ghcr.io`: `tools/toolkit-publish.sh`,
or `--force` to rebuild regardless.

GitHub leaves two things to the repository's owner: the package starts out
**private** -- make it public under Package settings, Change visibility -- and
scheduled workflows stop after 60 days without activity in the repository,
which the Actions tab offers to re-enable.

## CI

[.github/workflows/build.yml](.github/workflows/build.yml) runs
`tools/build-container.sh` and `tools/deploy-container.sh` for each target,
as the `win-x64` and `win-arm64` jobs, exactly as above. A `v*` tag's release
waits for both and carries `WinDiskImager-vX-win-x64.zip` and
`-win-arm64.zip`, each with its `.sha256` (before 2.0.4, the x64 zip was
`-win64`).

## Other tools

- **`tools/gpttest.sh`**, **`tools/imgtest.sh`**, **`tools/combinetest.sh`**
  (MSYS2): test harnesses, below.
- **`tools/lupdate.sh`**: refreshes `src/lang/*.ts`, with the host's Qt 6
  `lupdate` or else in the toolkit image; see
  [Updating the translations](#updating-the-translations).
- **`tools/make-test-images.sh`**, **`tools/verify-flashed.sh`** (Linux): test
  images, and `cmp` of an image against a device.
- **`tools/gui-probe.ps1`**: measures a running test build's widgets.
- **`tools/mkicon/`**: renders the icon during every build; never run by hand.

## Working on the interface

Every launch of the real binary raises a UAC prompt, which gets old when the
change under test is a tooltip. This builds one that does not:

```
tools/build.sh test
```

It lands in `build/` like any other build, replacing whatever was there, so
switching back is just `tools/build.sh` again.

It **cannot open a device**, so use it for layout, tooltips, translations and
dialog text, and a normal build for anything that touches a card. Both
deploy scripts refuse to package one, since the two are indistinguishable once
the exe sits in a folder of its own. To tell them apart by hand:

```
grep -ac 'level="asInvoker"' build/WinDiskImager.exe    # 1 = test build
```

### Measuring what it drew

Some interface faults are only a few pixels wide. A tooltip is clipped when its
box is narrower than its text needs, and no screenshot tells you that to the
pixel. `tools/gui-probe.ps1` asks the running application where its widgets are,
over UI Automation, and measures what came out:

```
powershell -NoProfile -ExecutionPolicy Bypass -File tools/gui-probe.ps1 -List
powershell -NoProfile -ExecutionPolicy Bypass -File tools/gui-probe.ps1 -Hover cboxHashType
powershell -NoProfile -ExecutionPolicy Bypass -File tools/gui-probe.ps1 -Hover bHashCopy,fixGptCheckBox -Shot tip.png
```

`-List` prints the widgets under the object names they have in
`src/mainwindow.ui`, which are the names `-Hover` takes. Hovering moves the
pointer onto each in turn and reports the tooltip left showing:

```
tip w=170 h=20 at 562,390: Generate selected hash on file
```

Compare that width against what the text needs; if the box is the narrower of
the two, the text is cut off. Naming several widgets walks the pointer from one
to the next while the first tooltip is still up, which is when Qt reuses one
tooltip label for the next -- the case where sizing has gone wrong before. If it
reports no tooltip, the previous one is usually covering the next widget, so the
pointer landed on the tooltip instead: hover that widget on its own.

Positions are read from the application again before every move, so the window
can be anywhere. It drives the real pointer, though, so leave the mouse alone
while it runs.

## Testing the GPT repair

`relocateBackupGPT()` rewrites partition tables and zeroes the stale backup left
mid-device, so a mistake there destroys data rather than merely misbehaving.
This exercises it against a file standing in for a device — no card, no VM, no
UAC prompt, about a second per run:

```
tools/gpttest.sh
```

It compiles the real `src/disk.cpp` into the harness, so the code under test is
the shipped code, and exits non-zero if any check fails — enough to gate a
commit or a release. MSYS2 UCRT64 only, being Win32 code. `clean` starts over;
otherwise it stays incremental.

The sources are in `tools/gpttest/`, a separate cmake project that builds into
`build-gpttest/` rather than `build/`, so it cannot fight the application's
cache.

Nine cases; the harness prints the check total when it finishes. Three relocate
a backup stranded mid-device. Three are cases where **nothing may be written** —
no GPT at all, a backup already at the last LBA, and a corrupt header. One has a
partition sitting over the stale copy: the relocate still happens, but nothing
inside the partition may be zeroed. Two start from a table Windows has already
rewritten — one that must be detected and repaired, and the ordinary layout,
which must *not* be reported as damage.

The cases where nothing may happen are the point. When changing the repair,
confirm the harness still fails when it should: break a guard on purpose, watch
it go red, put it back.

## Testing the image decoder

`src/imagesource.cpp` decides what bytes reach the card when the image is
compressed, and it is the easiest place in the program to be subtly wrong: a
member boundary landing mid-sector, a stream with padding between, a file that
stops in the middle. None of that is reachable from the GPT harness.

```
tools/imgtest.sh
```

It compiles the real `src/imagesource.cpp`, then reads images back through it
and compares them with the bytes that went in -- raw, gzip, xz, bzip2 and zstd;
several gzip members, xz and bzip2 streams or zstd frames in one file, including
pzstd's skippable frames; padding between or after streams; and an image whose
length is not a whole number of sectors. Truncated gzip, xz, bzip2 and zstd files
must be *rejected*, because writing what did come out and calling it done would
put half an image on a card, as must a bad bzip2 CRC or zstd checksum.
Multi-block xz, which liblzma decodes on several threads, has cases of its own.

It also drives `src/transferpipe.cpp`, the worker threads Write, Verify and a
compressed Read run on: the image read ahead must come out exactly as reading
it in order would, errors included, stopping part way must not hang, and
everything compressed through the writer must read back.

The fixtures are built by the harness itself with zlib, liblzma, libbz2 and
libzstd, so nothing compressed is checked in and no compressor needs to be on
the path.

## Testing Custom Partitioning

`src/combine.cpp` builds partition tables from pieces of other images, so its
mistakes would write a device that looks fine and does not boot, or holds the
wrong data.

```
tools/combinetest.sh
```

It builds GPT, MBR and table-less (FAT32, ext4) images in memory, reads their
layouts, plans combinations of them -- reordered, with and without a lead-in,
mixing MBR and GPT sources, the same image twice -- and applies each plan to a
buffer standing in for the device. The result is read back through
`disk.cpp`, not through `combine.cpp`'s own code: the partition listing,
`gptPrimaryState()`, and `relocateBackupGPT()`, which must find the backup
already at the end. Every sector of every partition and lead-in carries a tag
naming what it belongs to, so one landing in the wrong place is caught.
Layouts that cannot be written -- five partitions on an MBR, a type with no
counterpart, too small a device -- must be refused.

For an image file, `src/combinereader.cpp` produces the combined image front
to back, as a compressed file must be written; it must give byte for byte what
the plan applied to a device gives, from a gzip image whose partitions go on
in reverse order -- which it can only do by starting that image again -- and
into an `.img.xz` that decompresses to exactly that. A truncated input must be
an error.

It also drives `src/combinedialog.cpp` off screen, with a gzip image among its
inputs, as a user would: adding images, ticking partitions in order, picking a
lead-in, switching to an image file. `COMBINETEST_SHOT=file.png` saves the
dialog as drawn.

Disks as sources are read through `ImageSource`, which takes a
`\\.\PhysicalDriveN` path as that disk. The harness checks the paths, and
that a disk it cannot open fails as that disk rather than as a file of that
name; it does not read a real disk, which needs Administrator and a disk to
spare. Try one by hand: a card, combined into an image file.

## Testing shrink-on-read

"Skip unpartitioned space" repacks a GPT or MBR device to remove the
unpartitioned gaps between partitions and after the last one, and any of the
gap before the first partition past 32 MiB after the table, instead of reading
the device byte for byte. It has no
automated harness of its own; `tools/make-test-images.sh` (Linux only, see
above) instead builds seven device images purpose-built to exercise it, each
with a gap or region that must (or must not) survive the shrink stamped with
its own ASCII tag rather than left as zeros, so a diff of the shrunk output
against the original catches a dropped gap or lost data that comparing sizes
alone would miss:

```
test-shrink-mbr.img            test-shrink-gpt.img
test-shrink-mbr-tight.img      test-shrink-gpt-tight.img
test-shrink-mbr-multi.img      test-shrink-gpt-multi.img
                                test-shrink-gpt-reserved.img
```

`-tight` is the negative case (nothing to shrink; the box should be a no-op),
`-multi` has three partitions to test gaps *between* partitions specifically,
and `-reserved` raises `FirstUsableLBA` the way an ARM board's U-Boot region
does, with more data above it before the partition, as Armbian's Rockchip
images have. The script's own `MANIFEST.txt`, written alongside the images, spells
out what each one expects and why -- read it before poking at any of them
by hand, especially `test-shrink-gpt-reserved.img`: creating a volume in its
reserved span in Disk Management is a good way to end up with a card stuck
refusing writes until physically reseated (a Windows/VDS issue, not this
app's).

To exercise one: write it to a device, Read it back with the box checked
(and optionally "Compress during Read", which applies to any Read and has
no fixture of their own), and confirm the result matches what the manifest
says it should.

## Changing the icons

The icons are SVGs in `src/images/`, embedded through `gui_icons.qrc`: the three
action icons on the buttons, the folder on the browse button, and
`WinDiskImager.svg`, which is both the window icon and the source of the
executable's icon. Edit an SVG, rebuild, and the change is in the program. There
is nothing to regenerate by hand.

The executable's icon takes one extra step, because the resource compiler accepts
an `.ico` and nothing else. The build renders it: `tools/mkicon` turns the SVG
into an `.ico` in the build directory, writing eight sizes into one file — 16
through 64 as DIBs, which every version of Windows reads, and 128 and 256 as
PNG, which is what the format expects for the large ones and keeps the file to
tens of kilobytes rather than hundreds.

The `.ico` is not in the repository. The SVG is the icon; the `.ico` is a build
artefact like the `.qm` files. It was committed once, and every cross build
then showed it as modified: the rendering is identical everywhere -- all six
DIB sizes come out byte for byte the same on MSYS2 and in the container -- but
the two PNG entries are compressed, and different zlib versions turn identical
pixels into different bytes.

mkicon has to run on the machine doing the build, which when cross-compiling is
not the machine being built for. So the application's cmake configures it
separately, without the cross toolchain, against the host's own Qt — which is
why `gcc-c++`, `qt6-qtbase-devel` and `qt6-qtsvg-devel` are in the toolkit's
package list even though the application itself is built entirely with
llvm-mingw. MSYS2 already has what it needs.

To see an icon at the sizes it will actually be used at, before committing to
it, `QIcon` renders an SVG at any size -- a dozen-line Qt program showing the
file at 16, 24, 32, 48 and 128 says more than looking at it full size does. A
detail that reads at 128 is often a smudge at 16.

**The wrench in the app icon is not ours.** It is Feathericon's, under the MIT
licence, which requires the notice to travel with every copy including binaries.
`THIRD-PARTY-NOTICES.txt` carries it and both deploy scripts ship it. Anything
else brought in from outside needs the same treatment: a permissive licence that
is compatible with the GPL (MIT and BSD are; Apache-2.0 is not compatible with
GPL-2), its notice added to that file, and the source recorded in a comment at
the top of the SVG.

## Updating the translations

`src/lang/*.ts` hold the translations; the build compiles them to `.qm` and
embeds those through `translations.qrc`. Adding or changing a `tr()` string does
not reach the `.ts` files on its own — until they are refreshed, a new string
falls back to English and a *reworded* one silently loses the translation it had.

```
tools/lupdate.sh              # every language
tools/lupdate.sh de fr        # only those
```

This works anywhere: it uses whichever Qt 6 `lupdate` it can find — MSYS2's
`lupdate` on Windows, Fedora's `lupdate-qt6` — and falls back to running itself
in the container when the host has neither. Only version 6 is accepted, since a
Qt 5 `lupdate` writes `.ts` files the Qt 6 build then has to interpret.

Unlike the build scripts it rewrites tracked files, so review the diff:

- **New strings** arrive as `<translation type="unfinished"></translation>`.
  `lrelease` skips them and the app shows English until someone fills them in.
- **Reworded strings** appear as a new unfinished entry while the old translation
  is kept beside it as `type="vanished"`, so a translator can adapt it. Nothing
  is lost.
- Most of the diff is `<location>` line numbers moving with the source. Expected
  and harmless.

Run it before a release, or after any commit that touches user-visible text.

## Where things are defined

Two lists would otherwise be written down in several places, so each has one home
and everything else reads it from there.

**The toolkit** — the Fedora packages, llvm-mingw, the libraries and Qt it
builds, and where each target's sysroot is — lives in
[tools/toolkit-env.sh](tools/toolkit-env.sh); **the rest of the build** — the
host's `lrelease-qt6` and `lupdate-qt6`, the cmake flags, the packaging and
licence steps, the MSYS2 package list and the podman plumbing — in
[tools/build-env.sh](tools/build-env.sh). Every route and CI read them, so no
two can end up on different toolkits.

```
tools/toolkit-env.sh arm64 print SYSROOT    # where the arm64 toolkit is
tools/toolkit-env.sh x64 env                # its settings, for a shell of your own
tools/build-env.sh packages-msys2           # what a native Windows build needs
CROSS_LUPDATE=/usr/bin/lupdate-qt6 tools/lupdate.sh    # a tool somewhere else
```

**The shipped languages** live in `src/CMakeLists.txt`, and both deploy scripts
read the list from there, so trimming Qt's translations cannot drift from the set
the build compiles:

```
set(LANGUAGES es it pl nl de fr zh_CN zh_TW ta_IN ko ja)
```

Adding one takes two edits, not one: that list, and a matching
`<file>lang/diskimager_xx.qm</file>` in `src/translations.qrc`, which names every
embedded file explicitly. Miss the second and the language compiles but is never
shipped.

## Why the deploy scripts look alike

The two deploy scripts are deliberately parallel — same files copied in, same Qt
plugin groups, same trimming of Qt's translations, same iterate-until-stable DLL
closure, same refusal to package a test build. They differ only where the
platform forces it: `windeployqt6` and `objdump` on Windows against a hand-rolled
copy, and `objdump -p` on Linux, and `deploy.sh` empties `dist/` in place rather
than deleting it, because Explorer locks the directory node on Windows.

`windeployqt` cannot run on Linux at all, being a Windows binary, which is why
the cross deploy gathers the Qt plugins and translations by hand. It also strips
the result with `llvm-strip`, so no debug symbols ship.

The `.rc` file must reference the icon with a forward slash. Windows `windres`
accepts a backslash there; the cross build does not.
