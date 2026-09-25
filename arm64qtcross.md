# Cross-compiling WinDiskImager for Windows on ARM64 from Fedora 44

Working notes, not a supported build. Nothing in the real build uses any of
this yet, and none of it has been run end to end: treat every command as a
starting point to verify, and record what actually happened next to it.

> **Scripted since:** these steps are now `tools/woa64-env.sh install`, run by
> `tools/Containerfile.arm64`; `tools/build-container-arm64.sh` and
> `tools/deploy-container-arm64.sh` build and package with it (BUILD.md,
> "Windows on ARM64, cross-compiled from Linux"). Two of the workarounds below
> are gone there: Qt is configured with `-plugindir lib/qt6/plugins` and
> `-translationdir share/qt6/translations`, so the layout symlinks in section 6
> are not needed; and the licence gap at the end of section 6 is closed by the
> manifest described there. These notes stay as the record of how and why.
>
> The script also no longer pins the versions and checksums chosen below.
> The libraries and Qt come from the upstream tarballs in Fedora's
> signature-checked source RPMs, with no Fedora patches applied:
> - Qt from the same `qt6-*` builds as the host Qt;
> - zlib, zstd and bzip2 from `mingw64-*`;
> - xz from `xz`.
>
> llvm-mingw is its newest GitHub release. The image is `fedora:latest`, and
> the build wrapper rebuilds it when any of those have been updated.

## Why this is different from the x64 cross build

Today's cross build leans on Fedora packaging everything for us:

| Piece | x64 today (Fedora packages) | ARM64 (we build it) |
|---|---|---|
| Compiler | `mingw64-gcc-c++` (GCC, `x86_64-w64-mingw32-*`) | **llvm-mingw** (clang, `aarch64-w64-mingw32-*`) |
| C runtime, Windows headers | `mingw64-crt`, `mingw64-headers` | shipped inside llvm-mingw (mingw-w64, UCRT) |
| C++ runtime | libstdc++, libgcc | libc++, libunwind (from llvm-mingw) |
| Qt | `mingw64-qt6-qtbase`, `-qtsvg`, `-qttranslations` | **built from source** |
| zlib, xz, bzip2, zstd | `mingw64-zlib`, `-xz`, `-bzip2`, `-zstd` | **built from source** |
| CMake toolchain file | `/usr/share/mingw/toolchain-mingw64.cmake` | **written by us** |
| Sysroot | `/usr/x86_64-w64-mingw32/sys-root/mingw` | a prefix we choose, e.g. `/opt/woa64/sysroot` |

Fedora's MinGW packages exist only for i686 and x86_64, and GCC has no
Windows-on-ARM64 target, so the two bold rows are the real work. Once they
exist, building the app is the same shape as today: a toolchain file, a
sysroot with Qt and the libraries in it, and `cross_configure` pointed at
both.

Everything below goes under one prefix so it can be deleted, or later baked
into a container image the way `tools/Containerfile.build` does for x64:

```
/opt/llvm-mingw/          the toolchain (unpacked release)
/opt/woa64/sysroot/       target libraries and Qt: include/, lib/, bin/, plugins, ...
/opt/woa64/qt-host/       a host (Linux) Qt of the same version, for its tools
/opt/woa64/src/           source tarballs and build trees
/opt/woa64/toolchain-aarch64-mingw.cmake
```

## 1. Host packages

```bash
sudo dnf install cmake ninja-build git python3 perl-interpreter \
    gcc-c++ make tar xz wget patch \
    qt6-qtbase-devel qt6-qtsvg-devel qt6-linguist qt6-qttools-devel
```

`gcc-c++` and the native Qt `-devel` packages are for the host-side pieces:
building a host Qt (step 4) and `tools/mkicon`, which the app's own build
already compiles natively (`CROSS_NATIVE_QT` in `tools/build-env.sh`).
`qt6-linguist` supplies the native `lrelease-qt6` the app build uses today.

## 2. The toolchain: llvm-mingw

[llvm-mingw](https://github.com/mstorsjo/llvm-mingw) is clang/LLD plus
mingw-w64's headers and CRT, built for i686, x86_64, armv7 and **aarch64**
Windows. It is what MSYS2's CLANGARM64 environment is built on.

Use the prebuilt Linux release (it is statically linked enough to run on
Fedora), picking the **UCRT** variant -- ARM64 Windows has no old
`msvcrt.dll` worth targeting:

```bash
datecode=20260922

cd ~
wget "https://github.com/mstorsjo/llvm-mingw/releases/download/$datecode/llvm-mingw-$datecode-ucrt-ubuntu-22.04-x86_64.tar.xz"

tar xf ~/llvm-mingw-$datecode-ucrt-ubuntu-22.04-x86_64.tar.xz

cd /opt
sudo mv ~/llvm-mingw-$datecode-ucrt-ubuntu-22.04-x86_64 llvm-mingw
export PATH=/opt/llvm-mingw/bin:$PATH
aarch64-w64-mingw32-clang --version

```

It provides triple-prefixed wrappers (`aarch64-w64-mingw32-clang`, `-clang++`,
`-windres`, `-dlltool`, `-ar`, `-ranlib`, `-strip`, `-objcopy`, ...) and the
generic `llvm-objdump`. Check which of `aarch64-w64-mingw32-objdump` /
`llvm-objdump` exists; either reads PE import tables.

Alternative: build it from source with its `build-all.sh`, which takes an
hour or so and needs nothing but a host compiler. Only worth it to pin an
exact LLVM version.

Quick sanity test:

```bash
cd ~
printf '#include <windows.h>\nint main(){MessageBoxA(0,"hi","arm64",0);}\n' > hi.c
aarch64-w64-mingw32-clang hi.c -o hi.exe
file hi.exe        # expect: PE32+ executable for MS Windows 6.00 (console), ARM64, 12 sections
```

## 3. The CMake toolchain file

The ARM64 counterpart of Fedora's `toolchain-mingw64.cmake`.

**3.1** Make the work area, owned by you so nothing after this needs `sudo`:

```bash
sudo mkdir -p /opt/woa64/sysroot /opt/woa64/src
sudo chown -R "$USER": /opt/woa64
```

**3.2** Write the toolchain file. The quoted `'EOF'` stops the shell from
expanding `${TRIPLE}`, which CMake expands itself:

```bash
cat > /opt/woa64/toolchain-aarch64-mingw.cmake <<'EOF'
# Cross-compile for Windows on ARM64 with llvm-mingw.
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR ARM64)

set(TRIPLE aarch64-w64-mingw32)
set(CMAKE_C_COMPILER   /opt/llvm-mingw/bin/${TRIPLE}-clang)
set(CMAKE_CXX_COMPILER /opt/llvm-mingw/bin/${TRIPLE}-clang++)
set(CMAKE_RC_COMPILER  /opt/llvm-mingw/bin/${TRIPLE}-windres)
set(CMAKE_AR           /opt/llvm-mingw/bin/llvm-ar)
set(CMAKE_RANLIB       /opt/llvm-mingw/bin/llvm-ranlib)

# Search the target sysroot (and the toolchain's own) for libraries,
# headers and CMake packages, and the host for programs.
set(CMAKE_FIND_ROOT_PATH /opt/woa64/sysroot /opt/llvm-mingw/${TRIPLE})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
EOF
```

**3.3** Check it landed with `${TRIPLE}` intact (not expanded to nothing):

```bash
grep TRIPLE /opt/woa64/toolchain-aarch64-mingw.cmake
```

Every library below is configured with this file and
`-DCMAKE_INSTALL_PREFIX=/opt/woa64/sysroot`, so the sysroot fills up exactly
like Fedora's `sys-root/mingw` does: `bin/` for DLLs, `lib/` for import
libraries and CMake packages, `include/` for headers.

## 4. The libraries the app links

The app needs zlib, liblzma, libbz2 and libzstd (`find_package` in
`src/CMakeLists.txt`). All four are small, and build as shared DLLs to match
what the x64 package ships.

**Which versions.** Match MSYS2, which is what the native Windows build uses
and which tracks upstream releases closely -- so the ARM64 package runs the
same library versions as the x64 one you test most. Check what MSYS2 has with
`pacman -Q mingw-w64-ucrt-x86_64-{zlib,xz,zstd,bzip2}` in an MSYS2 shell, or on
packages.msys2.org. As of 2026-09-25:

| Library | Version | MSYS2 | Fedora 44 MinGW | Upstream latest |
|---|---|---|---|---|
| zlib | **1.3.2** | 1.3.2 | 1.3.2 | 1.3.2 |
| xz (liblzma) | **5.8.4** | 5.8.4 | 5.2.4 (too old, see 4.3) | 5.8.4 |
| zstd | **1.5.7** | 1.5.7 | 1.5.7 | 1.5.7 |
| bzip2 | **1.0.8** | 1.0.8 | 1.0.8 | 1.0.8 (last stable) |

Each download below is checked against the checksum its project publishes,
the way the x64 build now checks xz (`CROSS_XZ_SHA256` in `tools/build-env.sh`).
A newer version needs its own checksum from the same place.

**4.1** Shorthand for every configure below (set it again in a new shell,
along with `PATH` from step 2):

```bash
export PATH=/opt/llvm-mingw/bin:$PATH
TC=/opt/woa64/toolchain-aarch64-mingw.cmake
SR=/opt/woa64/sysroot
X="-G Ninja -DCMAKE_TOOLCHAIN_FILE=$TC -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$SR"
cd /opt/woa64/src
```

**4.2 zlib 1.3.2** -- the simplest of the four, so it is the test that the
toolchain file works. Checksum: GitHub's digest for the release asset.

```bash
wget https://github.com/madler/zlib/releases/download/v1.3.2/zlib-1.3.2.tar.gz
echo "bb329a0a2cd0274d05519d61c667c062e06990d72e125ee2dfa8de64f0119d16  zlib-1.3.2.tar.gz" | sha256sum -c
tar xf zlib-1.3.2.tar.gz
cmake -S zlib-1.3.2 -B b-zlib $X
cmake --build b-zlib
cmake --install b-zlib
file $SR/bin/*libz*.dll      # expect: PE32+ ... (DLL) ... ARM64
```

zlib 1.3.2's CMake names the DLL `libz.dll` and its import library
`libz.dll.a` (Fedora's are `zlib1.dll` / `libz.dll.a`); CMake's `FindZLIB`
finds either.

**4.3 xz 5.8.4** (liblzma). 5.4+ is needed for the multi-threaded decoder the
app uses -- the reason the x64 build stopped using Fedora's 5.2.4 in 2.0.3.
Only the library is wanted, not the xz tools, translations, docs or tests;
these are the options `cross_build_xz` in `tools/build-env.sh` uses, checked
against 5.8.4's CMake (`XZ_TOOL_SCRIPTS` does not exist on Windows targets).
Checksum: GitHub's digest, the same one pinned as `CROSS_XZ_SHA256`.

```bash
wget https://github.com/tukaani-project/xz/releases/download/v5.8.4/xz-5.8.4.tar.xz
echo "4ce24038fd4221e0d13bc1a2de7a4db56e90b92b3bf75321f6c14be73f65de4b  xz-5.8.4.tar.xz" | sha256sum -c
tar xf xz-5.8.4.tar.xz
cmake -S xz-5.8.4 -B b-xz $X -DBUILD_SHARED_LIBS=ON -DBUILD_TESTING=OFF \
    -DXZ_NLS=OFF -DXZ_DOC=OFF \
    -DXZ_TOOL_XZ=OFF -DXZ_TOOL_XZDEC=OFF -DXZ_TOOL_LZMADEC=OFF -DXZ_TOOL_LZMAINFO=OFF
cmake --build b-xz
cmake --install b-xz
file $SR/bin/liblzma.dll
mkdir -p $SR/share/licenses/xz && cp xz-5.8.4/COPYING xz-5.8.4/COPYING.0BSD $SR/share/licenses/xz/
```

The CMake build names the DLL `liblzma.dll` (Fedora's is `liblzma-5.dll`).
The last line keeps xz's licence -- `COPYING` says what covers what,
`COPYING.0BSD` is liblzma's -- where the licence step in section 6 can find it.

Shortcut worth trying instead: `cross_build_xz` already does all of this for
the x64 build, with the toolchain and prefix as variables:
(not tested yet)
```bash
CROSS_TOOLCHAIN=$TC CROSS_XZ_PREFIX=$SR \
    bash -c '. /path/to/windiskimager/tools/build-env.sh && cross_build_xz'
```

**4.4 zstd 1.5.7** (its CMake project lives under `build/cmake`). This
release predates GitHub's asset digests; the checksum is from the
`zstd-1.5.7.tar.gz.sha256` file published with it.

```bash
wget https://github.com/facebook/zstd/releases/download/v1.5.7/zstd-1.5.7.tar.gz
echo "eb33e51f49a15e023950cd7825ca74a4a2b43db8354825ac24fc1b7ee09e6fa3  zstd-1.5.7.tar.gz" | sha256sum -c
tar xf zstd-1.5.7.tar.gz
cmake -S zstd-1.5.7/build/cmake -B b-zstd $X \
    -DZSTD_BUILD_PROGRAMS=OFF -DZSTD_BUILD_TESTS=OFF \
    -DZSTD_BUILD_STATIC=OFF -DZSTD_BUILD_SHARED=ON \
    -DZSTD_MULTITHREAD_SUPPORT=ON
cmake --build b-zstd
cmake --install b-zstd
file $SR/bin/libzstd*.dll
ls $SR/lib/cmake/zstd/      # zstdConfig.cmake: the app links zstd::libzstd_shared from it
```

**4.5 bzip2** -- 1.0.8 has only a Unix Makefile, so build its seven source
files directly. Two Windows details:

- `bzlib.h` has a `#ifdef _WIN32` branch meant for loading the DLL by hand,
  which declares every function as a pointer. MinGW builds (Fedora's and
  MSYS2's included) take the ordinary branch instead; the `sed` below does
  the same for this copy. Check the line first with `grep -n '_WIN32' bzlib.h`.
- No functions are marked for export, so LLD exports them all, as GNU ld
  does for MinGW.

Checksum: sourceware publishes SHA-512 for bzip2, in
`https://sourceware.org/pub/bzip2/sha512.sum`.

```bash
wget https://sourceware.org/pub/bzip2/bzip2-1.0.8.tar.gz
echo "083f5e675d73f3233c7930ebe20425a533feedeaaa9d8cc86831312a6581cefbe6ed0d08d2fa89be81082f2a5abdabca8b3c080bf97218a1bd59dc118a30b9f3  bzip2-1.0.8.tar.gz" | sha512sum -c
tar xf bzip2-1.0.8.tar.gz
cd bzip2-1.0.8
sed -i 's/^#ifdef _WIN32$/#if defined(_WIN32) \&\& !defined(__MINGW32__)/' bzlib.h
for f in blocksort huffman crctable randtable compress decompress bzlib; do
    aarch64-w64-mingw32-clang -O2 -D_FILE_OFFSET_BITS=64 -c $f.c
done
aarch64-w64-mingw32-clang -shared -o libbz2-1.dll \
    blocksort.o huffman.o crctable.o randtable.o compress.o decompress.o bzlib.o \
    -Wl,--out-implib,libbz2.dll.a
mkdir -p $SR/bin $SR/lib $SR/include
cp libbz2-1.dll $SR/bin/
cp libbz2.dll.a $SR/lib/
cp bzlib.h $SR/include/
cd ..
file $SR/bin/libbz2-1.dll
```

CMake's `FindBZip2` only needs `bzlib.h` and `libbz2.dll.a` in the sysroot,
which is all this installs.

**4.6** All four done -- the sysroot should now hold, at least:

```bash
ls $SR/bin/ $SR/lib/*.dll.a
# bin: libbz2-1.dll  liblzma.dll  libz.dll  libzstd.dll
# lib: libbz2.dll.a  liblzma.dll.a  libz.dll.a  libzstd.dll.a
```

## 5. Qt

Qt 6 cross-compiles with CMake, but needs a **host Qt of the same version**
for the tools that run during the build (`moc`, `rcc`, `uic`, `syncqt`, and
`lrelease` for `qttranslations`). That is the one piece the x64 build never
had to think about, because Fedora built both halves.

### 5a. Pick the version: 6.11.2

Fedora 44 ships `qt6-qtbase-6.11.2` (`rpm -q qt6-qtbase`), and MSYS2 builds
the x64 package with Qt 6.11.2 too. Using the same version means Fedora's own
Qt may serve as the host Qt (5b), and the ARM64 package runs the same Qt as
the x64 one.

Download the submodules from
`https://download.qt.io/official_releases/qt/6.11/6.11.2/submodules/`.
Qt lists only MD5 sums in its `md5sums.txt`; the SHA-256 below is from each
file's `.mirrorlist` page on download.qt.io, cross-checked against those MD5
sums. `qttools` is only needed if 5b ends up building a host Qt.

```bash
cd /opt/woa64/src
Q=https://download.qt.io/official_releases/qt/6.11/6.11.2/submodules
for m in qtbase qtsvg qttranslations qttools; do
    wget "$Q/$m-everywhere-src-6.11.2.tar.xz"
done
sha256sum -c <<'EOF'
5b2e00eccaf5a4d8c14134ffa0ea8dfd0a35ae1ffc7f8d87fa4305a1ed23cf22  qtbase-everywhere-src-6.11.2.tar.xz
d594337feca84c26fb67fe87b85e6a5c12fda404b611d905f9d138210c311876  qtsvg-everywhere-src-6.11.2.tar.xz
021684c1a7937a9fabc3b056a6698ad5978794caf9ac190fd6cc11399e67c014  qttranslations-everywhere-src-6.11.2.tar.xz
9ea75af35c512f7e09e61c8c3af3997f13b4d43bb099cf43fcec470126b4041e  qttools-everywhere-src-6.11.2.tar.xz
EOF
for m in qtbase qtsvg qttranslations qttools; do
    tar xf "$m-everywhere-src-6.11.2.tar.xz"
done
```

Sizes, for a sanity check: qtbase 48 MB, qttools 9.8 MB, qtsvg 2.2 MB,
qttranslations 1.6 MB.

### 5b. Host Qt: check that Fedora's will do

Nothing is built in this step. While cross-building Qt, the build has to run
Qt's own tools -- `moc`, `rcc`, `uic` and others -- on this Linux machine, so
it needs a Linux copy of Qt of the **same version** to take them from: the
"host Qt". Fedora's `qt6-qtbase-devel` is exactly that when its version matches
the source being built (both 6.11.2 here), which saves building one.

The check: the tools are installed, and so are the CMake packages that tell a
cross build where they are.

```bash
ls /usr/lib64/qt6/libexec/ | grep -E '^(moc|rcc|uic|syncqt|tracepointgen|cmake_automoc_parser)$'
ls -d /usr/lib64/cmake/Qt6HostInfo /usr/lib64/cmake/Qt6CoreTools 2>&1
```

All six tools and both directories listed means Fedora's Qt is the host Qt:
5c points at it with `-qt-host-path /usr` plus
`-DQT_HOST_PATH_CMAKE_DIR=/usr/lib64/cmake` (Fedora keeps CMake packages in
`lib64`, not the `lib` Qt would otherwise assume). Those are options for 5c's
`configure`, not commands to run here.

Only if 5c then fails with errors about the host installation, build a small
host Qt from the same tarballs instead, and use `-qt-host-path /opt/woa64/qt-host`
(and no `QT_HOST_PATH_CMAKE_DIR`) in 5c: (not tested)

```bash
cmake -S qtbase-everywhere-src-6.11.2 -B b-host-qtbase -G Ninja \
    -DCMAKE_INSTALL_PREFIX=/opt/woa64/qt-host -DCMAKE_BUILD_TYPE=Release \
    -DQT_BUILD_EXAMPLES=OFF -DQT_BUILD_TESTS=OFF
cmake --build b-host-qtbase && cmake --install b-host-qtbase
# then qttools the same way, with -DCMAKE_PREFIX_PATH=/opt/woa64/qt-host,
# for lrelease/lconvert
```

### 5c. Target qtbase

`win32-clang-g++` is Qt's mkspec for clang targeting MinGW, which llvm-mingw
is. The `configure` script takes Qt's own options first and passes
everything after `--` to CMake. The host Qt is Fedora's, from 5b.

**5c.1** Configure, keeping the output: its summary at the end is what to
read (and paste back) before building. Start from an empty build folder --
a configure run over an old one reuses its cached answers.

```bash
export PATH=/opt/llvm-mingw/bin:$PATH
cd /opt/woa64/src
rm -rf b-qtbase
mkdir -p b-qtbase && cd b-qtbase
../qtbase-everywhere-src-6.11.2/configure \
    -prefix /opt/woa64/sysroot \
    -xplatform win32-clang-g++ \
    -qt-host-path /usr \
    -release -shared \
    -nomake examples -nomake tests \
    -no-icu -no-openssl -no-dbus -no-sql-sqlite \
    -system-zlib -qt-libpng -qt-libjpeg -qt-harfbuzz -qt-freetype -qt-pcre \
    -- -DCMAKE_TOOLCHAIN_FILE=/opt/woa64/toolchain-aarch64-mingw.cmake \
       -DQT_HOST_PATH_CMAKE_DIR=/usr/lib64/cmake \
    2>&1 | tee configure.log
```

**5c.2** Build and install into the sysroot. This is the long one: expect
tens of minutes, depending on cores.

```bash
cmake --build . --parallel 2>&1 | tee build.log
cmake --install .
file /opt/woa64/sysroot/bin/Qt6Core.dll /opt/woa64/sysroot/plugins/platforms/qwindows.dll
```

Both `file` lines should say `PE32+ ... (DLL) ... ARM64`. If the platform
plugin is not under `plugins/`, `find /opt/woa64/sysroot -name qwindows.dll`
says where this layout put it.

Choices worth noting:

- **`-no-icu`** drops the ~35 MB `icudt*.dll` both current packages carry.
  Qt falls back to Windows' own APIs for what the app needs. Worth trying on
  x64 too, separately.
- **`-qt-*`** uses Qt's bundled copies of its third-party libraries, so none of
  harfbuzz, freetype, libpng, pcre2 and so on have to be cross-built first.
  Their licences are then Qt's, listed in "Third-Party Code Used in Qt".
- **`-system-zlib`, not `-qt-zlib`**: the sysroot already has zlib (step 4)
  for the app, and its `zlib.h` in `$SR/include` is found ahead of Qt's
  bundled one by every module built afterwards. With `-qt-zlib`, qtsvg
  compiled against that header and then failed to link (`undefined symbol:
  inflateInit2_`, `inflate`, `inflateEnd`, ...), because Qt had been told zlib
  was inside it. Using the sysroot's zlib gives the package one zlib,
  `libz.dll`, shared by Qt and the app -- as the x64 packages share
  `zlib1.dll`, since MSYS2's and Fedora's Qt both use the system zlib.
- **`-no-openssl`**: the app does no networking (2.0.2 already dropped
  Qt6Network from the package).
- The `qtbase` configure summary is the thing to read: it says which Windows
  features (the `qwindows` platform plugin, `qmodernwindowsstyle`, the SVG
  image plugins once `qtsvg` is built) actually came out. Its "Styles" line
  lists Fusion, Windows and WindowsVista but never Windows 11, even when that
  style is built; check `FEATURE_style_windows11` in `CMakeCache.txt` instead.

For reference, MSYS2's `mingw-w64-qt6-base` PKGBUILD shows the flags a
working CLANGARM64 Qt is built with, and is the best cross-check when something
fails to configure.

### 5d. qtsvg and qttranslations

Built against the target Qt just installed, with `qt-configure-module`, a
shell script the `qtbase` install put in `$SR/bin` (it runs on the host).
It needs no toolchain file: the `qtbase` install recorded one of its own,
`$SR/lib/cmake/Qt6/qt.toolchain.cmake`, which loads
`toolchain-aarch64-mingw.cmake` and the host-Qt settings from 5c. Passing
`-DCMAKE_TOOLCHAIN_FILE` here would replace that and lose the target Qt.

**5d.1** `qttranslations` compiles its `.ts` files with the host `lrelease`,
found through the host Qt's `Qt6LinguistTools` CMake package, which Fedora
ships in `qt6-qttools-devel`:

```bash

ls -d /usr/lib64/cmake/Qt6LinguistTools
```

**5d.2** qtsvg -- the SVG image format and icon engine plugins the app draws
its icons with:

```bash
export PATH=/opt/llvm-mingw/bin:$PATH
SR=/opt/woa64/sysroot
cd /opt/woa64/src
mkdir -p b-qtsvg && cd b-qtsvg
$SR/bin/qt-configure-module ../qtsvg-everywhere-src-6.11.2 2>&1 | tee configure.log
cmake --build . --parallel 2>&1 | tee build.log
cmake --install .
file $SR/bin/Qt6Svg.dll $SR/plugins/imageformats/qsvg.dll $SR/plugins/iconengines/qsvgicon.dll
cd ..
```

If `qt-configure-module` has trouble, plain CMake does the same:
`cmake -S ../<module> -B . -G Ninja -DCMAKE_TOOLCHAIN_FILE=$SR/lib/cmake/Qt6/qt.toolchain.cmake`
-- that is, pointing at Qt's toolchain file, not ours.

**5d.3** qttranslations -- Qt's own translated strings (dialog buttons and
the like), the `qtbase_<lang>.qm` files the package ships.

Not with `qt-configure-module`: its configure stops at
`Skipping the build as the condition "Qt6Linguist_FOUND" is not met`,
because it wants the *target* Qt's Linguist package, which means
cross-building qttools first. That is not worth it, because a `.qm` file is
data with no machine code in it: the ARM64 ones are the same as the x64 ones.
So compile the `.ts` sources from the same 6.11.2 tarball with Fedora's own
`lrelease-qt6` (from `qt6-linguist`, step 1), which is what that module's
build would do:

```bash
TR=/opt/woa64/src/qttranslations-everywhere-src-6.11.2/translations
mkdir -p $SR/translations
for ts in $TR/qtbase_*.ts; do
    lrelease-qt6 -silent "$ts" -qm "$SR/translations/$(basename "${ts%.ts}").qm"
done
ls $SR/translations/ | wc -l       # 31: 6.11.2 has 31 qtbase_*.ts
ls $SR/translations/ | grep -E '^qtbase_(de|fr|ja)\.qm$'
```

Only `qtbase_*` is compiled: that is the module the app uses, and
`deploy-cross.sh` copies only `qtbase_<lang>.qm`. Their licence is
qttranslations' (the tarball's `LICENSES/` directory); Fedora's
`qt6-qttranslations` package covers the same files in the x64 build.

## 6. Building WinDiskImager against it

The existing scripts take everything that differs as a variable, so nothing
in them needs to change to try this. The build and package go under
`/opt/woa64`, outside the repository, so the checkout is left untouched.

**6.1** The source, at 2.0.3 or later on Fedora: 2.0.3 changed
`cross_configure` (it names liblzma outright, from `CROSS_XZ_PREFIX`) and the
deploy functions (several DLL directories). Clone it, or update a clone:

```bash
git clone https://github.com/peacepenguin/windiskimager.git ~/windiskimager
# or, in an existing clone:
git -C ~/windiskimager pull
git -C ~/windiskimager log --oneline -1
```

**6.2** Give `deploy-cross.sh` what it expects to find in the sysroot.

Start from `/opt/woa64`, with the variables every part of 6.2 uses. Nothing
here depends on the current directory -- every path is absolute, and the
symlink targets below are resolved from where each link sits -- but
`llvm-objdump` at the end needs llvm-mingw on `PATH`:

```bash
cd /opt/woa64
export PATH=/opt/llvm-mingw/bin:$PATH
SR=/opt/woa64/sysroot
```

*Qt's layout.* This Qt installed the standard Windows layout --
`$SR/plugins/` and `$SR/translations/` -- where Fedora's MinGW Qt uses
`lib/qt6/plugins/` and `share/qt6/translations/`. `deploy-cross.sh` looks for
`lib/qt6/plugins` or `share/qt6/plugins`, and `share/qt6/translations`, so
link the Fedora names to the real directories rather than change the script.
`../../plugins` is relative to the link, in `$SR/share/qt6/`, so it points at
`$SR/plugins` whatever directory the command is typed in:

```bash
mkdir -p $SR/share/qt6
ln -sfn ../../plugins $SR/share/qt6/plugins
ln -sfn ../../translations $SR/share/qt6/translations
ls $SR/share/qt6/plugins/platforms/ $SR/share/qt6/translations/ | head
```

(For a real build, the cleaner fix is to configure `qtbase` with Fedora's
layout -- `-DINSTALL_PLUGINSDIR=lib/qt6/plugins
-DINSTALL_TRANSLATIONSDIR=share/qt6/translations` -- or to make the two
paths in `deploy-cross.sh` overridable.)

*The C++ runtime.* Everything built with clang++ needs llvm-mingw's
`libc++.dll` and `libunwind.dll`, which live in the toolchain, not the
sysroot. The deploy script searches only the sysroot (and the xz prefix),
so copy them in:

```bash
cp /opt/llvm-mingw/aarch64-w64-mingw32/bin/libc++.dll \
   /opt/llvm-mingw/aarch64-w64-mingw32/bin/libunwind.dll $SR/bin/
```

*objdump.* The deploy script finds each DLL's dependencies from the
`DLL Name:` lines `objdump -p` prints. Check llvm's prints them the same way:

```bash
llvm-objdump -p $SR/bin/Qt6Core.dll | grep 'DLL Name'
```

Expect `libz.dll`, `libc++.dll`, `KERNEL32.dll` and the like.

**6.3** Configure and build. Three variables differ from the x64 build:

- `CROSS_TOOLCHAIN`: Qt's own `qt.toolchain.cmake`, not ours. It loads
  `toolchain-aarch64-mingw.cmake` itself, and adds what a Qt application needs
  on top: where the target Qt is, and the host tools (`moc`, `uic`, `rcc`)
  from Fedora's Qt.
- `CROSS_SYSROOT`: the sysroot, for the deploy script.
- `CROSS_XZ_PREFIX`: also the sysroot. `cross_configure` names
  `$CROSS_XZ_PREFIX/lib/liblzma.dll.a` and its headers outright; the default
  (`/opt/mingw64-xz`) is the x64 build's.

```bash
cd ~/windiskimager
export PATH=/opt/llvm-mingw/bin:$PATH
export CROSS_TOOLCHAIN=/opt/woa64/sysroot/lib/cmake/Qt6/qt.toolchain.cmake
export CROSS_SYSROOT=/opt/woa64/sysroot
export CROSS_XZ_PREFIX=/opt/woa64/sysroot
rm -rf /opt/woa64/build-arm64
bash tools/build-env.sh configure src /opt/woa64/build-arm64 2>&1 | tee /opt/woa64/configure-app.log
cmake --build /opt/woa64/build-arm64 2>&1 | tee /opt/woa64/build-app.log
file /opt/woa64/build-arm64/WinDiskImager.exe
```

`file` should say `PE32+ executable ... (GUI), ARM64`. `cross_configure`
also adds `-DLRELEASE_EXECUTABLE=/usr/bin/lrelease-qt6` (`.ts` to `.qm` does
not depend on the target), and `tools/mkicon` is built with Fedora's native
Qt as in the x64 build. This is also the first time the app is compiled with
clang rather than GCC. It builds with no errors and two warnings, both
harmless:

- `'WINVER' macro redefined` and `'_WIN32_WINNT' macro redefined`, on every
  file: `src/CMakeLists.txt` defines them as `0x0601` (Windows 7), and this
  Qt's CMake target adds `0x0A00` (Windows 10) after it, which wins. Qt 6
  needs Windows 10 anyway, and Windows on ARM64 is 10 or later. Matching the
  app's value to Qt's (`0x0A00`) would silence it on both builds.
- `'closeEvent' overrides a member function but is not marked 'override'`
  (`src/mainwindow.h:60`): clang's `-Winconsistent-missing-override`, because
  other members of the class are marked `override`. Adding it is the fix.

Both are one-line source changes, deliberately not made for this experiment.

**6.4** Package with the same deploy script, pointed at the LLVM tools:

```bash
OBJDUMP=llvm-objdump STRIP=llvm-strip \
    bash tools/deploy-cross.sh /opt/woa64/build-arm64 /opt/woa64/dist-arm64 /opt/woa64/sysroot
ls /opt/woa64/dist-arm64/
```

What should come out, compared with the x64 package:

- `libc++.dll` and `libunwind.dll` instead of `libstdc++-6.dll` and
  `libgcc_s_seh-1.dll`. llvm-mingw also has winpthreads, so
  `libwinpthread-1.dll` appears only if something still links it; libc++, Qt
  and zstd use Windows threads directly.
- `libz.dll`, `liblzma.dll`, `libbz2-1.dll`, `libzstd.dll`, and the four Qt
  DLLs (Core, Gui, Widgets, Svg).
- No ICU, fontconfig, freetype, harfbuzz, libpng, pcre2 or double-conversion
  DLLs: Qt bundled what it needed of those.

### The one part that will not work as-is: licences

`deploy_write_licenses` finds each shipped DLL's licence by asking the
package manager who owns it (`pacman -Qo` / `rpm -qf`). Nothing in this
sysroot is owned by a package.

2.0.3 added the first piece of what this needs: a DLL found under
`CROSS_XZ_PREFIX` is listed as "xz ... (built from source)", with its version,
licence and source URL from `tools/build-env.sh` and its licence files from
`$CROSS_XZ_PREFIX/share/licenses/xz/`. With `CROSS_XZ_PREFIX` set to the whole
sysroot, as 6.3 does, **every** DLL in it matches -- so 6.4's
`THIRD-PARTY-NOTICES.txt` will list Qt, zlib and the rest as "xz", and the
final check will report no LGPL-3.0 text under `licenses/`, since only xz's
licence files were copied. The folder is complete and usable for testing,
but its notices are wrong: not for distribution.

The fix is to generalise that one case into a small manifest (DLL name →
project, version, licence, licence directory, source URL), filled in as each
library above is built. That is the only script change this route really
requires.

**Done:** each root now carries `share/windiskimager/sources.tsv`, one line
per shipped-file pattern (`PATTERN PACKAGE VERSION LICENCE SOURCE`, first match
wins), with that package's licence files in `share/licenses/<package>/`.
`manifest_add` in `tools/build-env.sh` writes it (xz's build records itself);
`woa64_record_sources` in `tools/woa64-env.sh` records the rest.
`deploy_write_licenses` reads it before asking the package manager, and a file
under a manifest root that no line matches is an error rather than a guess.

## 7. Testing

- Nothing ARM64 runs on an x64 Fedora host (Wine runs ARM64 Windows code only
  on ARM64 Linux), so `gpttest` and `imgtest` can be cross-built but not run
  here.
- They can run on a Windows-on-ARM device, or on GitHub's `windows-11-arm`
  runner: copy the `.exe` files and the DLLs over and run them there.
- The app itself: a Snapdragon X laptop, or any Windows 11 ARM64 machine.
  `file WinDiskImager.exe` on the host is the first check: it should say
  `PE32+ executable ... (GUI), ARM64`.

## 8. Turning it into a container

Once the steps above work by hand, they become a `Containerfile` layer on top
of `fedora:44`: unpack llvm-mingw, build the four libraries, build host and
target Qt, and leave `/opt/woa64` in the image. Expect the Qt builds to take
most of an hour and the image to be several GB, which is why it would be a
separate image from `tools/Containerfile.build` rather than an addition to it.

## Open questions to answer as we go

- ~~Does `-qt-host-path /usr` work with Fedora's packaged Qt, or is a host
  build of the same version required?~~ **Answered: it works** when the
  versions match (6.11.2 both), with `-DQT_HOST_PATH_CMAKE_DIR=/usr/lib64/cmake`.
- ~~Which `objdump` wrapper does llvm-mingw ship, and does `llvm-objdump -p`
  print `DLL Name:` lines the way `deploy_resolve_closure` expects?~~
  **Answered: `llvm-objdump` works unchanged** (`OBJDUMP=llvm-objdump`): the
  deploy found every DLL through it.
- Does `qmodernwindowsstyle` build for win32-clang-g++ ARM64, or does it fall
  back to `qwindowsvistastyle`? *Partly answered:* `styles/` is in the
  package and `style-windows11` is on in the configure; which style the app
  actually uses is for a Windows on ARM device to show.
- ~~Does the app compile with clang unchanged?~~ **Answered: yes**, with two
  harmless warnings (see 6.3).
- libzstd multithreading under llvm-mingw: it uses Windows threads directly,
  which should be fine, but check `ZSTD_c_nbWorkers` is accepted (the app
  quietly falls back to one thread if not). Needs a device: a compressed Read
  to `.img.zst` should use every core.
- Does the package start and run on Windows on ARM64? The one question left
  that only a device can answer (section 7).

## Notes log

Record dated results here as steps are actually tried.

- 2026-09-25, step 2: llvm-mingw (UCRT, Linux release) unpacked to
  `/opt/llvm-mingw` on Fedora 44. `hi.c` built with
  `aarch64-w64-mingw32-clang`; `file hi.exe` reported
  `PE32+ executable for MS Windows 6.00 (console), ARM64, 12 sections`.
- 2026-09-25, steps 3-4: toolchain file written; zlib 1.3.2, xz 5.8.4,
  zstd 1.5.7 and bzip2 1.0.8 built into `/opt/woa64/sysroot`.
  `$SR/bin`: `libbz2-1.dll liblzma.dll libz.dll libzstd.dll`;
  `$SR/lib`: `libbz2.dll.a liblzma.dll.a libz.dll.a libzstd.dll.a`.
  zlib's DLL came out as `libz.dll`, not the `libzlib.dll` these notes first
  expected.
- 2026-09-25, step 5a: Fedora 44 has `qt6-qtbase-6.11.2-2.fc44.x86_64`, the
  same Qt version MSYS2 builds the x64 package with; 6.11.2 chosen.
- 2026-09-25, step 5b: Fedora's `qt6-qtbase-devel` has all six host tools in
  `/usr/lib64/qt6/libexec` (`cmake_automoc_parser moc rcc syncqt tracepointgen
  uic`) and both `/usr/lib64/cmake/Qt6HostInfo` and `Qt6CoreTools`, so it is
  used as the host Qt; no host build.
- 2026-09-25, step 5c.1: qtbase 6.11.2 configured with no errors against the
  Fedora host Qt (`-qt-host-path /usr`, `QT_HOST_PATH_CMAKE_DIR=/usr/lib64/cmake`);
  configure took 7.7 s, summary in `b-qtbase/config.summary`. One warning:
  "Basic cpp/winrt support missing" -- llvm-mingw has no C++/WinRT headers,
  so Qt's optional WinRT-based extras (accent colour, network status and the
  like) use their fallbacks. Not needed by the app; MinGW Qt builds generally
  lack it.
  Summary: Qt Gui and Widgets yes; OpenSSL, ICU no; zlib, PCRE2, FreeType,
  HarfBuzz, JPEG, PNG all bundled (system: no); DirectWrite and DirectWrite 3
  yes; Styles "Fusion Windows WindowsVista". The Styles line never names
  Windows 11 -- in 6.11.2 `src/widgets/configure.cmake` its summary list is
  `style-fusion style-mac style-windows style-windowsvista style-android` --
  but `style-windows11` has the same condition as `style-windowsvista`, so it
  is on; `grep FEATURE_style_windows11 CMakeCache.txt` confirms. MSYS2's x64
  Qt has it too (`QT_FEATURE_style_windows11 1`).
- 2026-09-25, step 5c.2: qtbase built and installed into `/opt/woa64/sysroot`.
  `bin/Qt6Core.dll` and `plugins/platforms/qwindows.dll` are both
  `PE32+ executable for MS Windows 6.00 (DLL), ARM64, 15 sections`. Qt used
  the standard Windows layout (`plugins/`, `translations/` under the prefix),
  not Fedora's `lib/qt6/plugins`; see section 6.
- 2026-09-25, step 5d.2: qtsvg failed to link `bin/Qt6Svg.dll`:
  `ld.lld: error: undefined symbol: inflateInit2_` (and `inflate`,
  `inflateReset`, `inflateEnd`), from `qsvgdocument.cpp` (`.svgz` support).
  Cause: qtbase was configured `-qt-zlib`, but qtsvg compiled against the
  step-4 `zlib.h` in `$SR/include`, which nothing then linked. Fix: 5c.1 now
  uses `-system-zlib`; qtbase to be reconfigured from an empty `b-qtbase`,
  rebuilt and reinstalled, then qtsvg rebuilt from an empty `b-qtsvg`.
- 2026-09-25, step 5d.3: `qt-configure-module` for qttranslations configured
  with clang 23.1.2 but skipped the build: `Skipping the build as the
  condition "Qt6Linguist_FOUND" is not met` (it needs the target Qt's
  Linguist package, i.e. a cross-built qttools). `cmake --install` installed
  nothing, so `$SR/translations/` did not exist. 5d.3 now compiles
  `qtbase_*.ts` from the tarball with the host `lrelease-qt6` instead: `.qm`
  files have no machine code, so they need no cross build.
- 2026-09-25, steps 5c-5d done: qtbase rebuilt with `-system-zlib`, then qtsvg
  built cleanly. `bin/Qt6Svg.dll` (14 sections), `plugins/imageformats/qsvg.dll`
  and `plugins/iconengines/qsvgicon.dll` (15 each) are all PE32+ ARM64 DLLs.
  `lrelease-qt6` compiled all 31 `qtbase_*.ts` into `$SR/translations/`
  (`qtbase_de.qm`, `qtbase_fr.qm`, `qtbase_ja.qm` among them). Section 5 is
  complete: Qt 6.11.2 for Windows on ARM64, cross-built on Fedora 44.
- 2026-09-25, step 6.3: the app configured (ZLIB 1.3.2, LibLZMA 5.8.4 and
  BZip2 1.0.8 all found in `/opt/woa64/sysroot`) and built in 27 steps with
  clang 23.1.2: `file WinDiskImager.exe` reports
  `PE32+ executable for MS Windows 6.00 (GUI), ARM64, 14 sections`. The 11
  translations and the icon (`mkicon`, 8 images, 50049 bytes) were generated
  as in the x64 build. No errors; warnings as listed under 6.3 (WINVER
  redefined, `closeEvent` missing `override`).
- 2026-09-25, step 6.4: `deploy-cross.sh` with `OBJDUMP=llvm-objdump
  STRIP=llvm-strip` produced `/opt/woa64/dist-arm64` with the exe, the four
  Qt DLLs (Core, Gui, Widgets, Svg), `platforms/ styles/ imageformats/
  iconengines/ translations/`, `libz.dll liblzma.dll libbz2-1.dll
  libzstd.dll`, and `libc++.dll libunwind.dll`, plus the docs and licences.
  None of the x64 package's ICU, glib, harfbuzz, freetype, pcre2, libpng,
  brotli, iconv, gettext or graphite2 DLLs. It ended with the expected
  `error: no LGPL-3.0 text under /opt/woa64/dist-arm64/licenses; Qt 6 requires
  it.` -- the licence-attribution gap in section 6, not a packaging fault.
