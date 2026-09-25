# Cross-compiling WinDiskImager for Windows on ARM64 from Fedora 44

Working notes, not a supported build. Nothing in the real build uses any of
this yet, and none of it has been run end to end: treat every command as a
starting point to verify, and record what actually happened next to it.

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
    qt6-qtbase-devel qt6-qtsvg-devel qt6-linguist
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
what the x64 package ships. The versions below are known-good starting
points; newer ones should work the same way.

**4.1** Shorthand for every configure below (set it again in a new shell,
along with `PATH` from step 2):

```bash
export PATH=/opt/llvm-mingw/bin:$PATH
TC=/opt/woa64/toolchain-aarch64-mingw.cmake
SR=/opt/woa64/sysroot
X="-G Ninja -DCMAKE_TOOLCHAIN_FILE=$TC -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$SR"
cd /opt/woa64/src
```

**4.2 zlib** -- the simplest of the four, so it is the test that the
toolchain file works:

```bash
wget https://github.com/madler/zlib/releases/download/v1.3.1/zlib-1.3.1.tar.gz
tar xf zlib-1.3.1.tar.gz
cmake -S zlib-1.3.1 -B b-zlib $X
cmake --build b-zlib
cmake --install b-zlib
file $SR/bin/*zlib*.dll      # expect: PE32+ ... (DLL) ... ARM64
```

zlib's CMake names the DLL `libzlib.dll` rather than Fedora's `zlib1.dll`;
CMake's `FindZLIB` finds either.

**4.3 xz** (liblzma; CMake since 5.4, and 5.4+ is needed for the
multi-threaded decoder the app uses). Only the library is wanted, not the
command-line tools:

```bash
wget https://github.com/tukaani-project/xz/releases/download/v5.8.1/xz-5.8.1.tar.xz
tar xf xz-5.8.1.tar.xz
cmake -LH -S xz-5.8.1 -B b-xz $X | grep -E '^(XZ_|BUILD_)'   # the options this version has
cmake -S xz-5.8.1 -B b-xz $X -DBUILD_SHARED_LIBS=ON -DXZ_NLS=OFF \
    -DXZ_TOOL_XZ=OFF -DXZ_TOOL_XZDEC=OFF -DXZ_TOOL_LZMADEC=OFF \
    -DXZ_TOOL_LZMAINFO=OFF -DXZ_TOOL_SCRIPTS=OFF
cmake --build b-xz
cmake --install b-xz
file $SR/bin/liblzma*.dll
```

If an option in the second `cmake` is not in the `-LH` list, drop it.

**4.4 zstd** (its CMake project lives under `build/cmake`):

```bash
wget https://github.com/facebook/zstd/releases/download/v1.5.7/zstd-1.5.7.tar.gz
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

```bash
wget https://sourceware.org/pub/bzip2/bzip2-1.0.8.tar.gz
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
# bin: libzlib.dll liblzma*.dll libzstd.dll libbz2-1.dll
# lib: libzlib.dll.a liblzma.dll.a libzstd.dll.a libbz2.dll.a
```

## 5. Qt

Qt 6 cross-compiles with CMake, but needs a **host Qt of the same version**
for the tools that run during the build (`moc`, `rcc`, `uic`, `syncqt`, and
`lrelease` for `qttranslations`). That is the one piece the x64 build never
had to think about, because Fedora built both halves.

### 5a. Pick the version

Take the Qt version Fedora 44 ships (`rpm -q qt6-qtbase`), or one close to
MSYS2's, and download the matching submodule tarballs from
`https://download.qt.io/official_releases/qt/6.X/6.X.Y/submodules/`:
`qtbase`, `qtsvg`, `qttools` (host only, for `lrelease`), `qttranslations`.

### 5b. Host Qt

Either point `QT_HOST_PATH` at Fedora's own Qt when the versions match
exactly -- worth trying first, as it saves an hour:

```
-DQT_HOST_PATH=/usr -DQT_HOST_PATH_CMAKE_DIR=/usr/lib64/cmake
```

or, if CMake complains about the host installation, build a small host Qt
from the same tarballs:

```bash
cmake -S qtbase-everywhere-src-6.X.Y -B b-host-qtbase -G Ninja \
    -DCMAKE_INSTALL_PREFIX=/opt/woa64/qt-host -DCMAKE_BUILD_TYPE=Release \
    -DQT_BUILD_EXAMPLES=OFF -DQT_BUILD_TESTS=OFF
cmake --build b-host-qtbase && cmake --install b-host-qtbase
# then qttools the same way, with -DCMAKE_PREFIX_PATH=/opt/woa64/qt-host,
# for lrelease/lconvert
```

### 5c. Target qtbase

`win32-clang-g++` is Qt's mkspec for clang targeting MinGW, which llvm-mingw
is. The `configure` script passes everything after `--` to CMake:

```bash
mkdir b-qtbase && cd b-qtbase
../qtbase-everywhere-src-6.X.Y/configure \
    -prefix /opt/woa64/sysroot \
    -xplatform win32-clang-g++ \
    -qt-host-path /opt/woa64/qt-host \
    -release -shared \
    -nomake examples -nomake tests \
    -no-icu -no-openssl -no-dbus -no-sql-sqlite \
    -qt-zlib -qt-libpng -qt-libjpeg -qt-harfbuzz -qt-freetype -qt-pcre \
    -- -DCMAKE_TOOLCHAIN_FILE=/opt/woa64/toolchain-aarch64-mingw.cmake
cmake --build . --parallel
cmake --install .
```

Choices worth noting:

- **`-no-icu`** drops the ~35 MB `icudt*.dll` both current packages carry.
  Qt falls back to Windows' own APIs for what the app needs. Worth trying on
  x64 too, separately.
- **`-qt-*`** uses Qt's bundled copies of its third-party libraries, so none of
  harfbuzz, freetype, libpng, pcre2 and so on have to be cross-built first.
  Their licences are then Qt's, listed in "Third-Party Code Used in Qt".
- **`-no-openssl`**: the app does no networking (2.0.2 already dropped
  Qt6Network from the package).
- The `qtbase` configure summary is the thing to read: it says which Windows
  features (the `qwindows` platform plugin, `qmodernwindowsstyle`, the SVG
  image plugins once `qtsvg` is built) actually came out.

For reference, MSYS2's `mingw-w64-qt6-base` PKGBUILD shows the flags a
working CLANGARM64 Qt is built with, and is the best cross-check when something
fails to configure.

### 5d. qtsvg and qttranslations

Built against the target Qt just installed, with the same toolchain file:

```bash
/opt/woa64/sysroot/bin/qt-configure-module ../qtsvg-everywhere-src-6.X.Y \
    -- -DCMAKE_TOOLCHAIN_FILE=/opt/woa64/toolchain-aarch64-mingw.cmake
cmake --build . && cmake --install .
```

(and the same for `qttranslations`, which only needs the host `lrelease`).
`qt-configure-module` is a shell script installed by the cross build, so it
runs on the host; if it has trouble, plain `cmake -S ... -DCMAKE_TOOLCHAIN_FILE=... -DCMAKE_PREFIX_PATH=/opt/woa64/sysroot -DQT_HOST_PATH=...`
does the same.

## 6. Building WinDiskImager against it

The existing scripts already take everything that differs as a variable, so
nothing in them needs to change to try this:

```bash
export PATH=/opt/llvm-mingw/bin:$PATH
export CROSS_TOOLCHAIN=/opt/woa64/toolchain-aarch64-mingw.cmake
export CROSS_SYSROOT=/opt/woa64/sysroot
bash tools/build-env.sh configure src build-arm64
cmake --build build-arm64
```

`cross_configure` adds `-DLRELEASE_EXECUTABLE=/usr/bin/lrelease-qt6`, and
`.ts` to `.qm` compilation does not depend on the target, so Fedora's native
lrelease is fine. `tools/mkicon` is built with the native Qt as today.

Then package with the same deploy script, pointed at the LLVM tools:

```bash
OBJDUMP=llvm-objdump STRIP=llvm-strip \
    bash tools/deploy-cross.sh build-arm64 dist-arm64 /opt/woa64/sysroot
```

What should come out, compared with the x64 package:

- `libc++.dll` and `libunwind.dll` (from `/opt/llvm-mingw/aarch64-w64-mingw32/bin`)
  instead of `libstdc++-6.dll` and `libgcc_s_seh-1.dll`. llvm-mingw also has
  winpthreads, so `libwinpthread-1.dll` appears only if something still links
  it; libc++, Qt and zstd use Windows threads directly.
  `deploy_resolve_closure` searches only the sysroot's `bin`, so these either
  get copied into `/opt/woa64/sysroot/bin` first, or the closure needs a second
  search directory.
- No ICU, no fontconfig/freetype/harfbuzz DLLs if Qt bundled them.

### The one part that will not work as-is: licences

`deploy_write_licenses` finds each shipped DLL's licence by asking the
package manager who owns it (`pacman -Qo` / `rpm -qf`). Nothing in this
sysroot is owned by a package, so it stops with "no package owns ...". A
third backend would be needed: a small manifest (DLL name → project,
version, licence, licence file, source URL) filled in as each library above
is built. That is the only script change this route really requires.

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

- Does `-qt-host-path /usr` work with Fedora's packaged Qt, or is a host build
  of the same version required?
- Which `objdump` wrapper does llvm-mingw ship, and does `llvm-objdump -p` print
  `DLL Name:` lines the way `deploy_resolve_closure` expects?
- Does `qmodernwindowsstyle` build for win32-clang-g++ ARM64, or does it fall
  back to `qwindowsvistastyle`?
- Does the app compile with clang unchanged? It builds with GCC today; clang
  is stricter about a few things (format strings, implicit conversions).
- libzstd multithreading under llvm-mingw: it uses Windows threads directly,
  which should be fine, but check `ZSTD_c_nbWorkers` is accepted (the app
  quietly falls back to one thread if not).

## Notes log

Record dated results here as steps are actually tried.

- 2026-09-25, step 2: llvm-mingw (UCRT, Linux release) unpacked to
  `/opt/llvm-mingw` on Fedora 44. `hi.c` built with
  `aarch64-w64-mingw32-clang`; `file hi.exe` reported
  `PE32+ executable for MS Windows 6.00 (console), ARM64, 12 sections`.
