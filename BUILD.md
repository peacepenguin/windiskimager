# Build

Everything is done by a script in `tools/`. You run all of them the same way —
from the repo root, on your own machine. What differs is where the work then
happens.

A native Windows build lands in `build/`; a cross build in `build-x64/` or
`build-arm64/`, whichever route it took.

## What runs what

**On Windows**, in the MSYS2 UCRT64 shell:

- **`tools/build.sh`** → `build/`
  - cmake and ninja against the native Qt
- **`tools/deploy.sh`** → `dist/`
  - `windeployqt6`, then `objdump` for what it misses, then the licence of
    every library it ships, from `pacman`
- **`tools/gpttest.sh`** → pass or fail
  - builds the harness in `tools/gpttest/`, which links the real
    `src/disk.cpp`, and runs it
- **`tools/imgtest.sh`** → pass or fail
  - the same idea for `src/imagesource.cpp`: reads images back and compares
    them with what went in. Builds its own fixtures
- **`tools/mkicon/`** → the executable's `.ico`, into the build directory
  - renders the app icon SVG into the multi-size `.ico` the executable needs.
    Run by the build itself, on both platforms; never invoked by hand
- **`tools/gui-probe.ps1`** → measurements of the running window
  - PowerShell and UI Automation, against a test build already on screen

**On a Fedora host, with the target's toolkit installed**
(`sudo tools/toolkit-env.sh x64 install`, or `arm64`):

- **`tools/build-cross.sh x64|arm64`** → `build-x64/`, `build-arm64/`
  - cmake and ninja against the toolkit's llvm-mingw and Qt
- **`tools/deploy-cross.sh x64|arm64`** → `dist-x64/`, `dist-arm64/`
  - `llvm-objdump`, and the Qt plugins gathered by hand, then the licence of
    every library it ships, from the toolkit's manifest

**On Linux, without it** — the same build, one step further out:

- **`tools/build-container.sh x64|arm64`** → `build-x64/`, `build-arm64/`
  - podman, which pulls the toolkit image, llvm-mingw-qt6, and runs
    **`tools/build-cross.sh`** inside it
- **`tools/deploy-container.sh x64|arm64`** → `dist-x64/`, `dist-arm64/`
  - podman, which runs **`tools/deploy-cross.sh`** inside the image

  You run these on the host; it is the *build* and the *packaging* that happen
  in a container. The scripts are wrappers and nothing else.

**On Windows or Linux:**

- **`tools/lupdate.sh`** → `src/lang/*.ts`
  - whichever Qt 6 `lupdate` the host has — MSYS2's `lupdate`, Fedora's
    `lupdate-qt6` — else podman running *itself* in the toolkit's image

**On Linux:**

- **`tools/make-test-images.sh`** → test images, and a `MANIFEST.txt`
  describing them
  - `sfdisk`, `mkfs.vfat`, gzip and xz -- raw/gzip/xz sets, the GPT-rewrite
    pair (see [TESTING-GPT-BUG.md](TESTING-GPT-BUG.md)), and the
    shrink-on-read set (see below)
- **`tools/verify-flashed.sh`** → pass or fail
  - `cmp`, an image against a device

Every build script reads **`tools/build-env.sh`** for the cmake flags, the
packaging and licence steps and the podman plumbing, and every cross build
reads **`tools/toolkit-env.sh`** for the toolkit it builds against —
including `tools/Containerfile.toolkit` when it builds the image, and
[.github/workflows/build.yml](.github/workflows/build.yml), which runs
`tools/build-container.sh` and `tools/deploy-container.sh` for each target
the same as anyone with podman.

## Windows, natively

Once:

```
# non-admin powershell:
winget install --id MSYS2.MSYS2 -e --source winget
```

Then in the **MSYS2 UCRT64** shell, from the repo root:

```
# update all msys2 base packages first:
pacman -Suy

# then install dependancy packages:
pacman -S --needed $(bash tools/build-env.sh packages-msys2)
```

Then, any time:

```
tools/build.sh                 # into build/
tools/deploy.sh                # into dist/
```

`build/WinDiskImager.exe` only runs inside the MSYS2 shell, because its Qt
DLLs are not next to it. `dist/` is the standalone copy: move it anywhere and run
`dist/WinDiskImager.exe` **as Administrator**, which raw device access needs.

`tools/build.sh clean` drops the build directory first. Without it, ninja stays
incremental.

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

## Windows, cross-compiled from Linux

The release builds, `win-x64` and `win-arm64`, are cross-compiled on Fedora,
and the two are built the same way: one toolkit recipe,
[tools/toolkit-env.sh](tools/toolkit-env.sh), with the target architecture
the only thing that differs. Each target's toolkit is llvm-mingw (clang, lld
and the mingw-w64 runtime) and a sysroot of its own, `/opt/win-x64` or
`/opt/win-arm64`, holding zlib, xz, zstd, bzip2 and Qt compiled from source.
The MSYS2 build above stays for quick work on a Windows box; what ships is
this one.

`tools/build-cross.sh` *is* the cross build. Run it on a Fedora host and it
builds; `tools/build-container.sh` runs that same script inside the toolkit's
image, and [.github/workflows/build.yml](.github/workflows/build.yml) runs
`tools/build-container.sh` for each target. However this is built, it is
built by that one script, against a toolkit built by that one recipe.

**On a Fedora host.** Build the target's toolkit once -- most of it is Qt, so
expect it to take a while -- then build and package against it:

```
sudo tools/toolkit-env.sh x64 install
tools/build-cross.sh x64
tools/deploy-cross.sh x64
```

and the same with `arm64`, or `sudo tools/toolkit-env.sh all install` for
both. The two toolkits share everything but their sysroots: the host
packages, and one llvm-mingw in `/opt/llvm-mingw`, which targets every
Windows architecture. `tools/toolkit-env.sh ARCH check` (or `all check`)
says whether a toolkit is complete, and `stale` whether anything it was
built from has been updated since.

**Anywhere podman runs**, including a Fedora host that would rather not
install the toolkit. One image, llvm-mingw-qt6, holds both targets'
toolkits; the wrapper pulls the published one (see "The toolkit image"
below), so there is no Qt to build:

```
tools/build-container.sh x64
tools/deploy-container.sh x64
```

Those scripts are wrappers: each starts the container and runs `build-cross.sh`
or `deploy-cross.sh` inside it, where the toolkit is already installed, so
the host needs nothing but podman. `build-container.sh` and `build-cross.sh`
take the same arguments — the target, then `clean` to start over, `test` for a
no-elevation build — and both fail if what comes out is not a Windows binary
for that target, which is what a host compiler picked up by mistake, or the
other target's, would produce.

Either one before a push catches a broken cross build without waiting on the
workflow. Everything they write is gitignored, so the build directory persists
and ninja stays incremental: a no-op rebuild is well under a second, a one-file
change around twenty.

The two routes share `build-ARCH/`, and switching between them costs one
reconfigure. A cmake cache is tied to the path it was generated for, and the
container sees this tree as `/src`, so a cache from the other route is
dropped and rebuilt -- the script says so when it happens. `BUILD_DIR=...`
overrides the directory.

CI builds both targets side by side, as the `win-x64` and `win-arm64` jobs,
each pulling the published image. A tag's release waits for both and carries
both zips, `-win-x64` and `-win-arm64` (before 2.0.4, the x64 zip was
`-win64`). Each package's `THIRD-PARTY-NOTICES.txt` ends by naming the image
it was built in, by digest, so a release says exactly which toolkit build it
shipped with.

### Where the toolkit's sources come from

Nothing in the toolkit is pinned. The libraries and Qt are built from the
upstream tarballs in Fedora's own source RPMs, fetched and signature-checked
by `srpm_fetch`, without Fedora's patches or spec files:

| Library | Source RPM of |
|---|---|
| Qt (qtbase, qtsvg, qttranslations) | `qt6-qtbase`, `qt6-qtsvg`, `qt6-qttranslations`: the very builds the host Qt is installed from |
| zlib, zstd, bzip2 | `mingw64-zlib`, `mingw64-zstd`, `mingw64-bzip2` (Fedora's native zlib is zlib-ng) |
| xz | `xz`: Fedora's `mingw64-xz` is 5.2.4, older than the 5.4 the multi-threaded decoder needs |

The host Qt the cross build takes `moc`, `rcc` and `uic` from must be the
same version as the target Qt; taking the source from the host Qt's own build
makes it so. llvm-mingw is the newest release on GitHub, checked against the
SHA-256 GitHub publishes for it; `TOOLKIT_LLVM_MINGW_PIN` in
`tools/toolkit-env.sh` holds it at a release instead, for when a new one
breaks something. Moving to a new Fedora release needs no edit:
`fedora-minimal:latest` moves, and the image is rebuilt on it.

Qt is configured for what a widgets app on Windows needs and no more: no ICU
(35 MB of DLLs the app has no use for), no OpenSSL, D-Bus or SQL, and its own
bundled copies of the image and font libraries it does use. That is why a
package holds four Qt DLLs, the C++ runtime (`libc++.dll`, `libunwind.dll`)
and the four compression libraries, and nothing else.

Nothing in the toolkit is owned by a package manager, so each library records
itself in the sysroot's licence manifest (`manifest_add` in
`tools/build-env.sh`) with its version, licence and source RPM, and keeps its
licence files from its own source tree. `deploy_write_licenses` reads that in
place of rpm, so the package lists every library it ships as "built from
source", with the source it came from; a file in the sysroot the manifest
does not know stops the deploy. [arm64qtcross.md](arm64qtcross.md) has every
step worked through by hand, with what each one showed and why each choice
was made.

## The toolkit image

`tools/Containerfile.toolkit` builds llvm-mingw-qt6 by running
`tools/toolkit-env.sh` on `quay.io/fedora/fedora-minimal:latest`, in stages:
the host packages and llvm-mingw in one, each target's sysroot on it in a
stage of its own, and the finished sysroots copied into the image, so it
holds each thing once and none of the build trees. Nothing in it is
particular to WinDiskImager: it suits any Qt Widgets application that needs
no more of Qt than qtbase, qtsvg and qtbase's translations -- no ICU,
OpenSSL, D-Bus or SQL -- with zlib, xz, zstd and bzip2 beside it.

It is published as `ghcr.io/peacepenguin/llvm-mingw-qt6`
(`TOOLKIT_REGISTRY` in `tools/toolkit-env.sh`), by
[.github/workflows/toolkit.yml](.github/workflows/toolkit.yml) running
`tools/toolkit-publish.sh`, with three tags:

| Tag | What it is |
|---|---|
| `r3-20261003` | one build, kept for good |
| `r3` | the newest build for toolkit revision 3: what the build pulls |
| `latest` | the newest build |

`TOOLKIT_IMAGE_REVISION` is the revision: raised whenever what the image
holds or how it is laid out changes, so a build never runs in an image made
for another one. A commit that raises it builds the new revision's image
itself until that is published.

The workflow keeps it current, since the toolkit is built from Fedora's
repositories, updates included, and a build is only as current as its
image:

- **Daily**, it checks the published image with the `stale` test below and
  rebuilds only if that finds something.
- **When a file that goes into the image changes** on master, it rebuilds
  regardless.
- **By hand**, from the Actions tab, optionally forcing a rebuild.

The image is stale when `fedora-minimal:latest` is a newer Fedora release
than it was built on, or its `stale` command (`tools/toolkit-env.sh all
stale`) finds, for either target, a newer source RPM for any library or Qt,
or a newer llvm-mingw. Updates to the build tools alone -- cmake, gcc, mesa
-- do not count: nothing of them is shipped. A new image is built from
scratch and is published only once WinDiskImager builds and packages in it
for both targets.

Two things GitHub leaves to the repository's owner, once, after the first
publish: the package starts out **private**, so make it public in the
package's settings (Package settings, Change visibility) for anyone to pull it
without logging in; and GitHub stops scheduled workflows in a repository with
no activity for 60 days, which the Actions tab offers to re-enable.

### Locally

`tools/build-container.sh` pulls `r3` before each build -- a check that
fetches nothing when the image has not changed -- and `W32DI_REFRESH=0`
skips that, for working offline. The deploy wrapper never pulls, so a
package is always made from the image its build used: run the build wrapper
first. If the image cannot be pulled and there is no copy here, the wrapper
builds it here instead.

To work on the toolkit itself, build it here from the working tree:

```
TOOLKIT_IMAGE_SOURCE=local tools/build-container.sh arm64
```

That image is tagged by what it is asked to be -- the targets, the base
image, the package list, the llvm-mingw pin and the revision -- and is
checked for updates like the published one before each build, then rebuilt
from scratch under the same tag when it has fallen behind; `podman image
prune` reclaims the one replaced. To publish by hand, after `podman login
ghcr.io`: `tools/toolkit-publish.sh`, or `--force` to rebuild regardless.

The one GitHub API call in an image build -- the lookup of llvm-mingw's
newest release -- is limited to 60 an hour without a token. With
`GITHUB_TOKEN` set, `container_run` passes it into the build as a podman
secret, mounted for that step only and never stored in the image; both
workflows set it to the run's own token. Locally, if the limit is ever hit:

```
GITHUB_TOKEN=$(gh auth token) TOOLKIT_IMAGE_SOURCE=local tools/build-container.sh arm64
```

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
builds and where each target's sysroot is — lives in
[tools/toolkit-env.sh](tools/toolkit-env.sh), and **the rest of the build** —
the paths to `lrelease-qt6` and `lupdate-qt6`, the cmake flags, the
packaging and licence steps, the MSYS2 package list, and the podman plumbing
every container run goes through — in [tools/build-env.sh](tools/build-env.sh).
The container image, CI and every cross script read them, so a local
container build, a Fedora host's build and the CI job cannot end up on
different toolkits. A path can be pointed elsewhere for a host that lays it
out differently:

```
CROSS_LUPDATE=/usr/bin/lupdate-qt6 tools/lupdate.sh
```

```
tools/toolkit-env.sh arm64 print SYSROOT    # where the arm64 toolkit is
tools/toolkit-env.sh x64 env                # its settings, for a shell of your own
tools/build-env.sh packages-msys2           # what a native Windows build needs
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
