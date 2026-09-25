# Build

Everything is done by a script in `tools/`. You run all of them the same way —
from the repo root, on your own machine. What differs is where the work then
happens.

Every build lands in `build/`, whichever route it took.

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

**On Linux, with the cross toolchain installed:**

- **`tools/build-cross.sh`** → `build/`
  - cmake and ninja against the MinGW Qt
- **`tools/deploy-cross.sh`** → `dist/`
  - `objdump`, and the Qt plugins gathered by hand, then the licence of
    every library it ships, from `rpm`

**On Linux, without it** — the same build, one step further out:

- **`tools/build-container.sh`** → `build/`
  - podman, which runs **`tools/build-cross.sh`** inside the image
- **`tools/deploy-container.sh`** → `dist/`
  - podman, which runs **`tools/deploy-cross.sh`** inside the image

  You run these on the host; it is the *build* and the *packaging* that happen
  in a container. The scripts are wrappers and nothing else.

**On Windows or Linux:**

- **`tools/lupdate.sh`** → `src/lang/*.ts`
  - whichever Qt 6 `lupdate` the host has — MSYS2's `lupdate`, Fedora's
    `lupdate-qt6` — else podman running *itself* in the image

**On Linux:**

- **`tools/make-test-images.sh`** → test images, and a `MANIFEST.txt`
  describing them
  - `sfdisk`, `mkfs.vfat`, gzip and xz -- raw/gzip/xz sets, the GPT-rewrite
    pair (see [TESTING-GPT-BUG.md](TESTING-GPT-BUG.md)), and the
    shrink-on-read set (see below)
- **`tools/verify-flashed.sh`** → pass or fail
  - `cmp`, an image against a device

Everything that cross-builds reads **`tools/build-env.sh`** for the packages,
toolchain paths, cmake flags and podman plumbing — including
`tools/Containerfile.build` when it builds the image, and
[.github/workflows/build.yml](.github/workflows/build.yml), which runs
`tools/build-container.sh` and `tools/deploy-container.sh` the same as anyone
with podman.

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

It has to be Fedora: Debian and Ubuntu ship no MinGW Qt6 packages, so there is
nothing to link against there. That is why CI runs `ubuntu-latest` and builds
in a Fedora container, the same one `tools/build-container.sh` uses. The
release is named once, as `CROSS_BASE_IMAGE` (`quay.io/fedora/fedora-minimal:latest`) in
[tools/build-env.sh](tools/build-env.sh). CI builds the image fresh every run.
A local image is checked before each `tools/build-container.sh` run and
rebuilt if it has fallen behind -- see "Keeping the images current" below.

`tools/build-cross.sh` *is* the cross build. Run it on a Fedora host and it
builds; `tools/build-container.sh` runs that same script inside the container,
and [.github/workflows/build.yml](.github/workflows/build.yml) runs
`tools/build-container.sh`. However this is built, it is built by that one
script, with the toolchain and flags from `tools/build-env.sh`.

**On a Fedora host.** Install the toolchain once:

```
sudo bash tools/build-env.sh install
```

That is Fedora's MinGW packages plus one library built from source: liblzma,
into `/opt/mingw64-xz`. Fedora's `mingw64-xz` is 5.2.4, older than the 5.4
the multi-threaded xz decoder needs, so it is built instead from the source
of Fedora's native `xz` package: `srpm_fetch` in `tools/build-env.sh`
downloads that source RPM, checks Fedora's signature on it, and builds the
upstream tarball inside, without Fedora's patches or spec file. So liblzma is
whatever version that Fedora release ships, updates included. Its licence is
copied from the source tree and shipped like any package's, with the source
RPM as its source.

then:

```
tools/build-cross.sh
tools/deploy-cross.sh
```

**Anywhere podman runs**, including a Fedora host that would rather not install
the toolchain. The image builds itself on first use:

```
tools/build-container.sh
tools/deploy-container.sh
```

Those scripts are wrappers: each starts the container and runs `build-cross.sh`
or `deploy-cross.sh` inside it, where the toolchain is already installed, so
the host needs nothing but podman. `build-container.sh` and `build-cross.sh`
take the same arguments — `clean` to start over, `test` for a no-elevation
build — and both fail if what comes out is not a win64 PE binary, which is what
a host compiler picked up by mistake would produce.

Either one before a push catches a broken cross build without waiting on the
workflow. Everything they write is gitignored, so the build directory persists
and ninja stays incremental: a no-op rebuild is well under a second, a one-file
change around twenty.

They share `build/`, and switching between them costs one reconfigure. A cmake
cache is tied to the path it was generated for, and the container sees this tree
as `/src`, so a cache from the other route is dropped and rebuilt -- the script
says so when it happens. `BUILD_DIR=...` overrides the directory.

## Windows on ARM64, cross-compiled from Linux

Fedora packages MinGW for x86 only, so the ARM64 build brings its own
toolkit: llvm-mingw (clang for `aarch64-w64-mingw32`), zlib, xz, zstd, bzip2 and
Qt, built by [tools/woa64-env.sh](tools/woa64-env.sh).
`tools/Containerfile.arm64` builds it into an image -- expect that to take a
long while, most of it Qt, and the image to be several GB -- and after that
the build and package are the x64 ones:

```
tools/build-container-arm64.sh
tools/deploy-container-arm64.sh
```

These run the same `build-cross.sh` and `deploy-cross.sh` as the x64 wrappers,
in the ARM64 image, whose environment points them at the ARM64 toolchain,
sysroot, `llvm-objdump` and `llvm-strip` (`tools/woa64-env.sh env` prints it).
Output goes to `build-arm64/` and `dist-arm64/`, apart from the x64 build's.
The same `clean` and `test` arguments apply.

Nothing in the toolkit is pinned. The libraries and Qt are built from the
upstream tarballs in Fedora's own source RPMs, fetched and signature-checked
by `srpm_fetch`, without Fedora's patches or spec files:

| Library | Source RPM of |
|---|---|
| Qt (qtbase, qtsvg, qttranslations) | `qt6-qtbase`, `qt6-qtsvg`, `qt6-qttranslations`: the very builds the host Qt is installed from |
| zlib, zstd, bzip2 | `mingw64-zlib`, `mingw64-zstd`, `mingw64-bzip2`: what the x64 package ships (Fedora's native zlib is zlib-ng) |
| xz | `xz`, as in the x64 build |

The host Qt the cross build takes `moc`, `rcc` and `uic` from must be the
same version as the target Qt; taking the source from the host Qt's own build
makes it so. llvm-mingw is the newest release on GitHub, checked against the
SHA-256 GitHub publishes for it; `WOA64_LLVM_MINGW_PIN` in
`tools/woa64-env.sh` holds it at a release instead, for when a new one breaks
something. Moving to a new Fedora release needs no edit: `quay.io/fedora/fedora-minimal:latest`
moves, and the image is rebuilt on it.

Nothing in the toolkit is owned by a package manager, so each library records
itself in the sysroot's licence manifest (`manifest_add` in
`tools/build-env.sh`) with its version, licence and source RPM, and keeps its
licence files from its own source tree. `deploy_write_licenses` reads that in
place of rpm, so the ARM64 package lists every library it ships as "built from
source", with the source it came from. The x64 build's liblzma records itself
the same way. [arm64qtcross.md](arm64qtcross.md) has every step worked
through by hand, with what each one showed and why each choice was made.

## Keeping the images current

Both images take their libraries from Fedora's repositories, updates
included, so a build is only as current as its image. CI builds the image
fresh on every run. Locally, each `tools/build-container.sh` and
`tools/build-container-arm64.sh` run first checks the existing image
(`container_stale` in `tools/build-env.sh`) and rebuilds it, from scratch and
under the same tag, when:

- `quay.io/fedora/fedora-minimal:latest` is a newer Fedora release than the image was built on, or
- the image's `stale` command finds the repositories have moved on. For x64
  that is an update to any installed package (the MinGW libraries it ships
  among them) or a newer `xz` source RPM. For ARM64 it is a newer source RPM
  for any library or Qt, or a newer llvm-mingw; updates to the build tools
  alone do not count, since they are not shipped and the ARM64 rebuild takes
  most of an hour.

The check costs a pull of the base image and a download of the repository
metadata, well under a minute. `W32DI_REFRESH=0` skips it, for working
offline; if it cannot run, it warns and builds with the image as it is. The
deploy wrappers never refresh, so a package is always made from the image its
build used -- run the build wrapper first. The image replaced is left
untagged; `podman image prune` reclaims it.

The image tag itself changes only when what the image is asked to be changes:
the base image, the package lists, and the `CROSS_IMAGE_REVISION` and
`WOA64_IMAGE_REVISION` counters, raised when the install steps change.

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
why `gcc-c++`, `qt6-qtbase-devel` and `qt6-qtsvg-devel` are in the Fedora package
list even though the application itself is built entirely with the `mingw64-`
ones. MSYS2 already has what it needs.

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

**The toolchain** — Fedora packages, the MinGW sysroot, the paths to
`lrelease-qt6` and `lupdate-qt6`, the cmake flags, the MSYS2 package list, and
the podman plumbing every container run goes through — lives in
[tools/build-env.sh](tools/build-env.sh). The container image, CI and every cross
script read it, so a local container build and the CI job cannot end up on
different toolchains. Each path can be pointed elsewhere for a host that lays
them out differently:

```
CROSS_LUPDATE=/usr/bin/lupdate-qt6 tools/lupdate.sh
```

```
tools/build-env.sh packages         # what the container and CI install
tools/build-env.sh print SYSROOT    # where it expects the MinGW tree
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
the result: Fedora ships its MinGW DLLs unstripped, and `libstdc++-6.dll` alone
is ~26 MB otherwise.

The `.rc` file must reference the icon with a forward slash. Windows `windres`
accepts a backslash there; the cross build does not.
