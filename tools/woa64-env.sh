#!/usr/bin/env bash
# The Windows-on-ARM64 cross toolkit, in one place: what tools/Containerfile.arm64
# builds, pinned to exact versions and checksums, and how the ARM64 build uses it.
#
# Fedora packages MinGW only for x86, so for ARM64 the toolkit the x64 build
# gets from dnf is built here instead: llvm-mingw (clang for aarch64 Windows),
# zlib, xz, zstd and bzip2, and Qt 6 (qtbase, qtsvg, and qttranslations'
# qtbase_*.qm), all into WOA64_SYSROOT. Each records itself in that sysroot's
# licence manifest (manifest_add in tools/build-env.sh), so the package lists
# every library with its version, licence and source. arm64qtcross.md has the
# same steps worked through by hand, with what each one showed.
#
# The ARM64 build itself is the x64 one -- tools/build-cross.sh and
# tools/deploy-cross.sh, unchanged -- run with the variables `env` prints.
#
#   tools/woa64-env.sh install       # build the toolkit (what the image runs)
#   tools/woa64-env.sh check         # assert it is all there
#   tools/woa64-env.sh env           # the exports the ARM64 build needs
#   tools/woa64-env.sh print NAME    # one value (IMAGE, BASE_IMAGE, SYSROOT, ...)
#
# Copyright (C) 2026 peacepenguin, GPL-2.0-or-later.

. "$(dirname "${BASH_SOURCE[0]}")/build-env.sh"

# --------------------------------------------------------------- versions ---

# Fedora 44, not :latest: the host Qt below must be the same version as the
# Qt source built for the target, and 44's release repository keeps
# qt6-qtbase-devel 6.11.2 whatever its updates later bring.
WOA64_BASE_IMAGE="fedora:44"

WOA64_LLVM_MINGW_VERSION=20260922      # LLVM 23.1.2, UCRT
WOA64_LLVM_MINGW_SHA256=bb7bb7654b33d5aa8712acb837c963b2e0c56352560c76105270a3268c665c21
WOA64_LLVM_MINGW_URL="https://github.com/mstorsjo/llvm-mingw/releases/download/$WOA64_LLVM_MINGW_VERSION/llvm-mingw-$WOA64_LLVM_MINGW_VERSION-ucrt-ubuntu-22.04-x86_64.tar.xz"

# The libraries, at the versions MSYS2 builds the x64 package with. xz is
# CROSS_XZ_VERSION, built by the same cross_build_xz the x64 image uses.
WOA64_ZLIB_VERSION=1.3.2
WOA64_ZLIB_SHA256=bb329a0a2cd0274d05519d61c667c062e06990d72e125ee2dfa8de64f0119d16
WOA64_ZLIB_URL="https://github.com/madler/zlib/releases/download/v$WOA64_ZLIB_VERSION/zlib-$WOA64_ZLIB_VERSION.tar.gz"
WOA64_ZSTD_VERSION=1.5.7
WOA64_ZSTD_SHA256=eb33e51f49a15e023950cd7825ca74a4a2b43db8354825ac24fc1b7ee09e6fa3
WOA64_ZSTD_URL="https://github.com/facebook/zstd/releases/download/v$WOA64_ZSTD_VERSION/zstd-$WOA64_ZSTD_VERSION.tar.gz"
# sourceware publishes SHA-512 for bzip2.
WOA64_BZIP2_VERSION=1.0.8
WOA64_BZIP2_SHA512=083f5e675d73f3233c7930ebe20425a533feedeaaa9d8cc86831312a6581cefbe6ed0d08d2fa89be81082f2a5abdabca8b3c080bf97218a1bd59dc118a30b9f3
WOA64_BZIP2_URL="https://sourceware.org/pub/bzip2/bzip2-$WOA64_BZIP2_VERSION.tar.gz"

# Qt: the checksums are the SHA-256 on each file's .mirrorlist page on
# download.qt.io, which agree with the MD5 sums in its md5sums.txt.
WOA64_QT_VERSION=6.11.2
WOA64_QT_URL="https://download.qt.io/official_releases/qt/${WOA64_QT_VERSION%.*}/$WOA64_QT_VERSION/submodules"
WOA64_QTBASE_SHA256=5b2e00eccaf5a4d8c14134ffa0ea8dfd0a35ae1ffc7f8d87fa4305a1ed23cf22
WOA64_QTSVG_SHA256=d594337feca84c26fb67fe87b85e6a5c12fda404b611d905f9d138210c311876
WOA64_QTTRANSLATIONS_SHA256=021684c1a7937a9fabc3b056a6698ad5978794caf9ac190fd6cc11399e67c014

# Host packages. The Qt ones are pinned to WOA64_QT_VERSION: they are the
# host Qt the cross build takes moc, rcc and uic from (and tools/mkicon and
# lrelease-qt6 for the app, as in the x64 build).
WOA64_PACKAGES="cmake ninja-build file findutils curl tar xz make perl-interpreter python3
                gcc-c++
                qt6-qtbase-devel-$WOA64_QT_VERSION qt6-qtsvg-devel-$WOA64_QT_VERSION
                qt6-linguist-$WOA64_QT_VERSION"

# ------------------------------------------------------------------ paths ---

WOA64_TRIPLE=aarch64-w64-mingw32
WOA64_LLVM_MINGW="${WOA64_LLVM_MINGW:-/opt/llvm-mingw}"
WOA64_ROOT="${WOA64_ROOT:-/opt/woa64}"
WOA64_SYSROOT="$WOA64_ROOT/sysroot"
# The toolchain the libraries and Qt are built with ...
WOA64_TOOLCHAIN="$WOA64_ROOT/toolchain-aarch64-mingw.cmake"
# ... and the one Qt installs for building against it, which loads that one
# and adds the target Qt and the host tools. The app is built with this.
WOA64_QT_TOOLCHAIN="$WOA64_SYSROOT/lib/cmake/Qt6/qt.toolchain.cmake"

# The image tools/Containerfile.arm64 produces, tagged by everything above
# and WOA64_IMAGE_REVISION, bumped when install changes in ways they do not
# show. Override with WOA64_IMAGE=...
WOA64_IMAGE_REVISION=1
WOA64_IMAGE="${WOA64_IMAGE:-w32di-build-arm64:$(printf '%s' \
    "$WOA64_BASE_IMAGE$WOA64_PACKAGES$WOA64_LLVM_MINGW_SHA256$WOA64_ZLIB_SHA256$WOA64_ZSTD_SHA256$WOA64_BZIP2_SHA512$CROSS_XZ_SHA256$WOA64_QTBASE_SHA256$WOA64_QTSVG_SHA256$WOA64_QTTRANSLATIONS_SHA256$WOA64_IMAGE_REVISION" \
    | cksum | cut -d' ' -f1)}"

# -------------------------------------------------------------- functions ---

# woa64_fetch URL CHECKSUM -- downloads URL into the current directory and
# checks it before anything in it is used: SHA-256, or SHA-512 by length.
woa64_fetch()
{
    local url=${1:?usage: woa64_fetch URL CHECKSUM} sum=${2:?}
    local file=${url##*/}
    curl -fsSL -o "$file" "$url"
    if [ "${#sum}" -eq 128 ]; then
        echo "$sum  $file" | sha512sum -c --quiet -
    else
        echo "$sum  $file" | sha256sum -c --quiet -
    fi
}

# woa64_licences PACKAGE FILE... -- keeps a library's licence files where
# deploy_write_licenses looks for them.
woa64_licences()
{
    local pkg=${1:?usage: woa64_licences PACKAGE FILE...}
    shift
    mkdir -p "$WOA64_SYSROOT/share/licenses/$pkg"
    cp "$@" "$WOA64_SYSROOT/share/licenses/$pkg/"
}

woa64_install_toolchain()
{
    woa64_fetch "$WOA64_LLVM_MINGW_URL" "$WOA64_LLVM_MINGW_SHA256"
    tar -xf "${WOA64_LLVM_MINGW_URL##*/}"
    rm -rf "$WOA64_LLVM_MINGW"
    mv "llvm-mingw-$WOA64_LLVM_MINGW_VERSION-ucrt-ubuntu-22.04-x86_64" "$WOA64_LLVM_MINGW"

    # The ARM64 counterpart of Fedora's toolchain-mingw64.cmake.
    cat > "$WOA64_TOOLCHAIN" <<EOF
# Cross-compile for Windows on ARM64 with llvm-mingw; written by tools/woa64-env.sh.
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR ARM64)

set(CMAKE_C_COMPILER   $WOA64_LLVM_MINGW/bin/$WOA64_TRIPLE-clang)
set(CMAKE_CXX_COMPILER $WOA64_LLVM_MINGW/bin/$WOA64_TRIPLE-clang++)
set(CMAKE_RC_COMPILER  $WOA64_LLVM_MINGW/bin/$WOA64_TRIPLE-windres)
set(CMAKE_AR           $WOA64_LLVM_MINGW/bin/llvm-ar)
set(CMAKE_RANLIB       $WOA64_LLVM_MINGW/bin/llvm-ranlib)

# Libraries, headers and CMake packages from the target sysroot (and the
# toolchain's own); programs from the host.
set(CMAKE_FIND_ROOT_PATH $WOA64_SYSROOT $WOA64_LLVM_MINGW/$WOA64_TRIPLE)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
EOF

    # The C++ runtime everything built with clang++ needs, into the sysroot so
    # deploy-cross.sh finds it with the rest. LICENSE.TXT is LLVM's (libc++,
    # libunwind); the runtime COPYING covers the mingw-w64 startup code.
    local rt="$WOA64_LLVM_MINGW/$WOA64_TRIPLE"
    mkdir -p "$WOA64_SYSROOT/bin"
    cp "$rt/bin/libc++.dll" "$rt/bin/libunwind.dll" "$WOA64_SYSROOT/bin/"
    woa64_licences llvm-mingw "$WOA64_LLVM_MINGW/LICENSE.TXT" \
        "$rt/share/mingw32/COPYING.MinGW-w64-runtime.txt"
}

woa64_build_libraries()
{
    local X=(-G Ninja -DCMAKE_TOOLCHAIN_FILE="$WOA64_TOOLCHAIN" -DCMAKE_BUILD_TYPE=Release
             -DCMAKE_INSTALL_PREFIX="$WOA64_SYSROOT")

    woa64_fetch "$WOA64_ZLIB_URL" "$WOA64_ZLIB_SHA256"
    tar -xf "zlib-$WOA64_ZLIB_VERSION.tar.gz"
    cmake -S "zlib-$WOA64_ZLIB_VERSION" -B b-zlib "${X[@]}"
    cmake --build b-zlib
    cmake --install b-zlib
    woa64_licences zlib "zlib-$WOA64_ZLIB_VERSION/LICENSE"

    # The x64 image's own build of it, into this sysroot; it records itself
    # in the manifest.
    CROSS_TOOLCHAIN="$WOA64_TOOLCHAIN" CROSS_XZ_PREFIX="$WOA64_SYSROOT" cross_build_xz

    woa64_fetch "$WOA64_ZSTD_URL" "$WOA64_ZSTD_SHA256"
    tar -xf "zstd-$WOA64_ZSTD_VERSION.tar.gz"
    cmake -S "zstd-$WOA64_ZSTD_VERSION/build/cmake" -B b-zstd "${X[@]}" \
        -DZSTD_BUILD_PROGRAMS=OFF -DZSTD_BUILD_TESTS=OFF \
        -DZSTD_BUILD_STATIC=OFF -DZSTD_BUILD_SHARED=ON -DZSTD_MULTITHREAD_SUPPORT=ON
    cmake --build b-zstd
    cmake --install b-zstd
    woa64_licences zstd "zstd-$WOA64_ZSTD_VERSION/LICENSE" "zstd-$WOA64_ZSTD_VERSION/COPYING"

    # 1.0.8 has only a Unix Makefile, so its seven sources are built
    # directly. bzlib.h's _WIN32 branch declares every function as a pointer
    # for loading the DLL by hand; MinGW builds (Fedora's, MSYS2's) take the
    # ordinary branch, and so does this one. LLD exports every function, as
    # GNU ld does for MinGW, since none is marked for export.
    woa64_fetch "$WOA64_BZIP2_URL" "$WOA64_BZIP2_SHA512"
    tar -xf "bzip2-$WOA64_BZIP2_VERSION.tar.gz"
    (
        cd "bzip2-$WOA64_BZIP2_VERSION"
        grep -q '^#ifdef _WIN32$' bzlib.h
        sed -i 's/^#ifdef _WIN32$/#if defined(_WIN32) \&\& !defined(__MINGW32__)/' bzlib.h
        local f
        for f in blocksort huffman crctable randtable compress decompress bzlib; do
            "$WOA64_LLVM_MINGW/bin/$WOA64_TRIPLE-clang" -O2 -D_FILE_OFFSET_BITS=64 -c "$f.c"
        done
        "$WOA64_LLVM_MINGW/bin/$WOA64_TRIPLE-clang" -shared -o libbz2-1.dll \
            blocksort.o huffman.o crctable.o randtable.o compress.o decompress.o bzlib.o \
            -Wl,--out-implib,libbz2.dll.a
        mkdir -p "$WOA64_SYSROOT/bin" "$WOA64_SYSROOT/lib" "$WOA64_SYSROOT/include"
        cp libbz2-1.dll "$WOA64_SYSROOT/bin/"
        cp libbz2.dll.a "$WOA64_SYSROOT/lib/"
        cp bzlib.h "$WOA64_SYSROOT/include/"
    )
    woa64_licences bzip2 "bzip2-$WOA64_BZIP2_VERSION/LICENSE"
}

woa64_build_qt()
{
    local v=$WOA64_QT_VERSION
    woa64_fetch "$WOA64_QT_URL/qtbase-everywhere-src-$v.tar.xz" "$WOA64_QTBASE_SHA256"
    woa64_fetch "$WOA64_QT_URL/qtsvg-everywhere-src-$v.tar.xz" "$WOA64_QTSVG_SHA256"
    woa64_fetch "$WOA64_QT_URL/qttranslations-everywhere-src-$v.tar.xz" "$WOA64_QTTRANSLATIONS_SHA256"
    local m
    for m in qtbase qtsvg qttranslations; do
        tar -xf "$m-everywhere-src-$v.tar.xz"
    done

    # qtbase. The host Qt is Fedora's, the same version (checked in install).
    # -system-zlib: the sysroot has zlib already, and every module built
    # after qtbase finds its zlib.h first -- with Qt's bundled copy, qtsvg
    # then fails to link. -plugindir and -translationdir give Fedora's
    # layout, which deploy-cross.sh expects. -no-icu leaves out ICU's 35 MB;
    # the rest of Qt's third-party code is its bundled copies.
    mkdir -p b-qtbase
    (
        cd b-qtbase
        "../qtbase-everywhere-src-$v/configure" \
            -prefix "$WOA64_SYSROOT" \
            -xplatform win32-clang-g++ \
            -qt-host-path /usr \
            -plugindir lib/qt6/plugins -translationdir share/qt6/translations \
            -release -shared \
            -nomake examples -nomake tests \
            -no-icu -no-openssl -no-dbus -no-sql-sqlite \
            -system-zlib -qt-libpng -qt-libjpeg -qt-harfbuzz -qt-freetype -qt-pcre \
            -- -DCMAKE_TOOLCHAIN_FILE="$WOA64_TOOLCHAIN" \
               -DQT_HOST_PATH_CMAKE_DIR=/usr/lib64/cmake
        cmake --build . --parallel
        cmake --install .
    )
    woa64_licences qt6-base qtbase-everywhere-src-$v/LICENSES/*

    # qtsvg, with the toolchain file the qtbase install wrote.
    mkdir -p b-qtsvg
    (
        cd b-qtsvg
        "$WOA64_SYSROOT/bin/qt-configure-module" "../qtsvg-everywhere-src-$v"
        cmake --build . --parallel
        cmake --install .
    )
    woa64_licences qt6-svg qtsvg-everywhere-src-$v/LICENSES/*

    # qttranslations' qtbase_*.qm. Its own build wants the target Qt's
    # Linguist package, i.e. a cross-built qttools, but a .qm file has no
    # machine code: Fedora's lrelease-qt6 compiles the same sources to the
    # same files.
    local ts
    mkdir -p "$WOA64_SYSROOT/share/qt6/translations"
    for ts in "qttranslations-everywhere-src-$v"/translations/qtbase_*.ts; do
        lrelease-qt6 -silent "$ts" -qm "$WOA64_SYSROOT/share/qt6/translations/$(basename "${ts%.ts}").qm"
    done
    woa64_licences qt6-translations qttranslations-everywhere-src-$v/LICENSES/*
}

# Records what each shipped file was built from, in the sysroot's licence
# manifest (manifest_add in tools/build-env.sh; xz is recorded by
# cross_build_xz). Patterns match a file's path in the package; the first
# match wins, so qtsvg's come before qtbase's broader ones, which would
# otherwise take Qt6Svg.dll and the SVG plugins. Each package's licence files
# are the ones the build steps above copied into share/licenses/.
woa64_record_sources()
{
    local r=$WOA64_SYSROOT v=$WOA64_QT_VERSION p
    local qtlic="LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only"
    local f
    for f in libc++.dll libunwind.dll; do
        manifest_add "$r" "$f" llvm-mingw "$WOA64_LLVM_MINGW_VERSION" \
            "Apache-2.0 WITH LLVM-exception" "$WOA64_LLVM_MINGW_URL"
    done
    manifest_add "$r" libz.dll zlib "$WOA64_ZLIB_VERSION" Zlib "$WOA64_ZLIB_URL"
    manifest_add "$r" libzstd.dll zstd "$WOA64_ZSTD_VERSION" \
        "BSD-3-Clause OR GPL-2.0-only" "$WOA64_ZSTD_URL"
    manifest_add "$r" libbz2-1.dll bzip2 "$WOA64_BZIP2_VERSION" bzip2-1.0.6 "$WOA64_BZIP2_URL"
    for p in 'Qt6Svg*.dll' 'imageformats/qsvg*.dll' 'iconengines/qsvgicon*.dll'; do
        manifest_add "$r" "$p" qt6-svg "$v" "$qtlic" "$WOA64_QT_URL/qtsvg-everywhere-src-$v.tar.xz"
    done
    for p in 'Qt6*.dll' 'platforms/*' 'styles/*' 'imageformats/*' 'iconengines/*' 'generic/*'; do
        manifest_add "$r" "$p" qt6-base "$v" "$qtlic" "$WOA64_QT_URL/qtbase-everywhere-src-$v.tar.xz"
    done
    manifest_add "$r" 'translations/qtbase_*.qm' qt6-translations "$v" \
        "GPL-3.0-only WITH Qt-GPL-exception-1.0" \
        "$WOA64_QT_URL/qttranslations-everywhere-src-$v.tar.xz"
}

woa64_install()
{
    # shellcheck disable=SC2086   # deliberate word splitting
    dnf -y install $CROSS_DNF_FLAGS $WOA64_PACKAGES
    local host
    host=$(rpm -q --qf '%{VERSION}' qt6-qtbase-devel)
    if [ "$host" != "$WOA64_QT_VERSION" ]; then
        echo "error: the host Qt is $host, but the Qt source to build is $WOA64_QT_VERSION;" >&2
        echo "       they must be the same version." >&2
        return 1
    fi

    mkdir -p "$WOA64_SYSROOT" "$WOA64_ROOT/src"
    (
        set -e
        cd "$WOA64_ROOT/src"
        woa64_install_toolchain
        woa64_build_libraries
        woa64_build_qt
        woa64_record_sources
    )
    # Sources and build trees: several GB the image does not need.
    rm -rf "$WOA64_ROOT/src"
    woa64_check
}

# Fail early and loudly if the toolkit is not what the ARM64 build expects.
woa64_check()
{
    local bad=0 f
    for f in "$WOA64_LLVM_MINGW/bin/$WOA64_TRIPLE-clang++" "$WOA64_LLVM_MINGW/bin/llvm-objdump" \
             "$WOA64_LLVM_MINGW/bin/llvm-strip" "$WOA64_TOOLCHAIN" "$WOA64_QT_TOOLCHAIN" \
             "$WOA64_SYSROOT/bin/Qt6Core.dll" "$WOA64_SYSROOT/bin/Qt6Svg.dll" \
             "$WOA64_SYSROOT/lib/qt6/plugins/platforms/qwindows.dll" \
             "$WOA64_SYSROOT/share/qt6/translations/qtbase_de.qm" \
             "$WOA64_SYSROOT/bin/libz.dll" "$WOA64_SYSROOT/bin/liblzma.dll" \
             "$WOA64_SYSROOT/bin/libzstd.dll" "$WOA64_SYSROOT/bin/libbz2-1.dll" \
             "$WOA64_SYSROOT/bin/libc++.dll" "$WOA64_SYSROOT/bin/libunwind.dll" \
             "$WOA64_SYSROOT/share/windiskimager/sources.tsv"; do
        [ -e "$f" ] || { echo "missing $f" >&2; bad=1; }
    done
    # The app's own cross_check, as build-cross.sh will run it.
    CROSS_TOOLCHAIN="$WOA64_QT_TOOLCHAIN" CROSS_SYSROOT="$WOA64_SYSROOT" \
        CROSS_XZ_PREFIX="$WOA64_SYSROOT" cross_check || bad=1
    return $bad
}

# The environment the ARM64 build runs build-cross.sh and deploy-cross.sh
# in, as export lines: tools/Containerfile.arm64 sets the same as ENV.
woa64_env()
{
    cat <<EOF
export PATH=$WOA64_LLVM_MINGW/bin:\$PATH
export CROSS_TOOLCHAIN=$WOA64_QT_TOOLCHAIN
export CROSS_SYSROOT=$WOA64_SYSROOT
export CROSS_XZ_PREFIX=$WOA64_SYSROOT
export OBJDUMP=llvm-objdump
export STRIP=llvm-strip
EOF
}

# ---------------------------------------------------------------- command ---

if [ "${BASH_SOURCE[0]}" = "$0" ]; then
    set -euo pipefail
    cmd=${1:-}
    shift || true
    case "$cmd" in
        install) woa64_install ;;
        check)   woa64_check ;;
        env)     woa64_env ;;
        print)
            case "${1:-}" in
                IMAGE)      echo "$WOA64_IMAGE" ;;
                BASE_IMAGE) echo "$WOA64_BASE_IMAGE" ;;
                SYSROOT)    echo "$WOA64_SYSROOT" ;;
                TOOLCHAIN)  echo "$WOA64_QT_TOOLCHAIN" ;;
                QT_VERSION) echo "$WOA64_QT_VERSION" ;;
                *) echo "print: unknown name '${1:-}'" >&2; exit 2 ;;
            esac
            ;;
        *)
            sed -n '2,20p' "$0" | sed 's/^# \{0,1\}//'
            [ -n "$cmd" ] && exit 2
            exit 0
            ;;
    esac
fi
