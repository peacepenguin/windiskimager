#!/usr/bin/env bash
# The Windows-on-ARM64 cross toolkit, in one place: what tools/Containerfile.arm64
# builds, where its sources come from, and how the ARM64 build uses it.
#
# Fedora packages MinGW only for x86, so for ARM64 the toolkit the x64 build
# gets from dnf is built here instead: llvm-mingw (clang for aarch64 Windows),
# zlib, xz, zstd and bzip2, and Qt 6 (qtbase, qtsvg, and qttranslations'
# qtbase_*.qm), all into WOA64_SYSROOT. Each records itself in that sysroot's
# licence manifest (manifest_add in tools/build-env.sh), so the package lists
# every library with its version, licence and source. arm64qtcross.md has the
# same steps worked through by hand, with what each one showed.
#
# Nothing is pinned. The libraries and Qt are built from the upstream
# tarballs in Fedora's own source RPMs (srpm_fetch in tools/build-env.sh), so
# they are the versions the base image's Fedora ships, updates included: Qt
# the same build as the host Qt it cross-compiles with, zlib, zstd and bzip2
# what Fedora's mingw64 packages -- the x64 build's -- are built from, and xz
# what the x64 build's liblzma is. llvm-mingw is its newest GitHub release,
# unless WOA64_LLVM_MINGW_PIN names one. The image records what it was built
# from, and `stale` compares that with what is available now, so the build
# wrapper rebuilds it when any of it has been updated.
#
# The ARM64 build itself is the x64 one -- tools/build-cross.sh and
# tools/deploy-cross.sh, unchanged -- run with the variables `env` prints.
#
#   tools/woa64-env.sh install       # build the toolkit (what the image runs)
#   tools/woa64-env.sh check         # assert it is all there
#   tools/woa64-env.sh stale         # in the image: have updates come out?
#   tools/woa64-env.sh env           # the exports the ARM64 build needs
#   tools/woa64-env.sh print NAME    # one value (IMAGE, BASE_IMAGE, SYSROOT, TOOLCHAIN)
#
# Copyright (C) 2026 peacepenguin, GPL-2.0-or-later.

. "$(dirname "${BASH_SOURCE[0]}")/build-env.sh"

# --------------------------------------------------------------- versions ---

# The same Fedora as the x64 image. Whatever its release, the host Qt and the
# Qt source are one build (woa64_build_qt), so they cannot disagree.
WOA64_BASE_IMAGE="quay.io/fedora/fedora-minimal:latest"

# A release tag (e.g. 20260922) to hold llvm-mingw at, when the newest one
# breaks something; empty for the newest.
WOA64_LLVM_MINGW_PIN=""
WOA64_LLVM_MINGW_REPO=mstorsjo/llvm-mingw

# Host packages: the host Qt the cross build takes moc, rcc and uic from (and
# tools/mkicon and lrelease-qt6 for the app, as in the x64 build); cpio for
# srpm_fetch; python3 to read GitHub's release list.
WOA64_PACKAGES="cmake ninja-build file findutils curl tar xz cpio make perl-interpreter python3
                gcc-c++
                qt6-qtbase-devel qt6-qtsvg-devel qt6-linguist"

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

# Where the image records which llvm-mingw it was built with, for woa64_stale.
WOA64_LLVM_MINGW_STAMP="$WOA64_ROOT/llvm-mingw.version"

# The image tools/Containerfile.arm64 produces, tagged by what it is asked to
# be -- everything above, CROSS_IMAGE_REVISION for the install steps it shares
# with the x64 image, and WOA64_IMAGE_REVISION, bumped when the steps below
# change. Updates to what they fetch rebuild it in place (container_stale).
# Override with WOA64_IMAGE=...
WOA64_IMAGE_REVISION=2
WOA64_IMAGE="${WOA64_IMAGE:-w32di-build-arm64:$(printf '%s' \
    "$WOA64_BASE_IMAGE$WOA64_PACKAGES$WOA64_LLVM_MINGW_PIN$CROSS_IMAGE_REVISION$WOA64_IMAGE_REVISION" \
    | cksum | cut -d' ' -f1)}"

# What each library was built from, for woa64_record_sources: version and
# source address by package name, filled in by woa64_got.
declare -A WOA64_SRC_VERSION=() WOA64_SRC_URL=()

# -------------------------------------------------------------- functions ---

# woa64_llvm_mingw_release
#
# Sets WOA64_LLVM_MINGW_VERSION, _URL and _SHA256 from GitHub's API: the
# newest release's tag (or WOA64_LLVM_MINGW_PIN's), its Linux x86_64 UCRT
# build, and the SHA-256 GitHub computed for that file on upload. A release
# without one is refused, since the download could not then be checked.
woa64_llvm_mingw_release()
{
    local api="https://api.github.com/repos/$WOA64_LLVM_MINGW_REPO/releases/latest" json line
    [ -z "$WOA64_LLVM_MINGW_PIN" ] ||
        api="https://api.github.com/repos/$WOA64_LLVM_MINGW_REPO/releases/tags/$WOA64_LLVM_MINGW_PIN"
    json=$(curl -fsSL "$api") || {
        echo "error: could not read llvm-mingw's release ${WOA64_LLVM_MINGW_PIN:-latest} from $api" >&2
        return 1
    }
    line=$(printf '%s' "$json" | python3 -c '
import fnmatch, json, sys
r = json.load(sys.stdin)
a = sorted((x for x in r["assets"]
            if fnmatch.fnmatch(x["name"], "llvm-mingw-*-ucrt-ubuntu-*-x86_64.tar.xz")),
           key=lambda x: x["name"])
if not a or not (a[0].get("digest") or "").startswith("sha256:"):
    sys.exit("error: llvm-mingw %s has no checksummed ucrt-ubuntu x86_64 build"
             % r.get("tag_name", "?"))
print(r["tag_name"], a[0]["browser_download_url"], a[0]["digest"][7:])
') || return 1
    read -r WOA64_LLVM_MINGW_VERSION WOA64_LLVM_MINGW_URL WOA64_LLVM_MINGW_SHA256 <<< "$line"
}

# woa64_got PACKAGE -- notes the source srpm_fetch just fetched as PACKAGE's.
woa64_got()
{
    WOA64_SRC_VERSION[$1]=$SRPM_VERSION
    WOA64_SRC_URL[$1]=$SRPM_URL
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
    woa64_llvm_mingw_release
    local file=${WOA64_LLVM_MINGW_URL##*/}
    curl -fsSL -o "$file" "$WOA64_LLVM_MINGW_URL"
    echo "$WOA64_LLVM_MINGW_SHA256  $file" | sha256sum -c --quiet -
    tar -xf "$file"
    rm -rf "$WOA64_LLVM_MINGW"
    mv "${file%.tar.xz}" "$WOA64_LLVM_MINGW"

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

    # zlib, zstd and bzip2 from the source RPMs of the x64 build's mingw64
    # packages. Not Fedora's native zlib: that is zlib-ng.
    srpm_fetch mingw64-zlib 'zlib-*.tar.*'
    woa64_got zlib
    cmake -S "$SRPM_SRCDIR" -B b-zlib "${X[@]}"
    cmake --build b-zlib
    cmake --install b-zlib
    woa64_licences zlib "$SRPM_SRCDIR/LICENSE"

    # The x64 image's own build of it, into this sysroot; it records itself
    # in the manifest.
    CROSS_TOOLCHAIN="$WOA64_TOOLCHAIN" CROSS_XZ_PREFIX="$WOA64_SYSROOT" cross_build_xz

    srpm_fetch mingw64-zstd 'zstd-*.tar.*'
    woa64_got zstd
    cmake -S "$SRPM_SRCDIR/build/cmake" -B b-zstd "${X[@]}" \
        -DZSTD_BUILD_PROGRAMS=OFF -DZSTD_BUILD_TESTS=OFF \
        -DZSTD_BUILD_STATIC=OFF -DZSTD_BUILD_SHARED=ON -DZSTD_MULTITHREAD_SUPPORT=ON
    cmake --build b-zstd
    cmake --install b-zstd
    woa64_licences zstd "$SRPM_SRCDIR/LICENSE" "$SRPM_SRCDIR/COPYING"

    # 1.0.8 has only a Unix Makefile, so its seven sources are built
    # directly. bzlib.h's _WIN32 branch declares every function as a pointer
    # for loading the DLL by hand; MinGW builds (Fedora's, MSYS2's) take the
    # ordinary branch, and so does this one. LLD exports every function, as
    # GNU ld does for MinGW, since none is marked for export.
    srpm_fetch mingw64-bzip2 'bzip2-*.tar.*'
    woa64_got bzip2
    local bz=$SRPM_SRCDIR
    (
        cd "$bz"
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
    woa64_licences bzip2 "$bz/LICENSE"
}

woa64_build_qt()
{
    # The source of the very builds the host Qt was installed from, so the
    # two are the same version by construction. qttranslations has no host
    # package here, so its newest is taken and checked against that version.
    local v base svg tr
    v=$(rpm -q --qf '%{VERSION}' qt6-qtbase)
    srpm_fetch qt6-qtbase 'qtbase-everywhere-*src-*.tar.*' "$(rpm -q --qf '%{VERSION}-%{RELEASE}' qt6-qtbase)"
    woa64_got qt6-base
    base=$SRPM_SRCDIR
    srpm_fetch qt6-qtsvg 'qtsvg-everywhere-*src-*.tar.*' "$(rpm -q --qf '%{VERSION}-%{RELEASE}' qt6-qtsvg)"
    woa64_got qt6-svg
    svg=$SRPM_SRCDIR
    srpm_fetch qt6-qttranslations 'qttranslations-everywhere-*src-*.tar.*'
    woa64_got qt6-translations
    tr=$SRPM_SRCDIR
    local m
    for m in qt6-base qt6-svg qt6-translations; do
        if [ "${WOA64_SRC_VERSION[$m]}" != "$v" ]; then
            echo "error: $m's source is ${WOA64_SRC_VERSION[$m]}, but the host Qt is $v;" >&2
            echo "       they must be the same version." >&2
            return 1
        fi
    done

    # qtbase.
    # -system-zlib: the sysroot has zlib already, and every module built
    # after qtbase finds its zlib.h first -- with Qt's bundled copy, qtsvg
    # then fails to link. -plugindir and -translationdir give Fedora's
    # layout, which deploy-cross.sh expects. -no-icu leaves out ICU's 35 MB;
    # the rest of Qt's third-party code is its bundled copies.
    mkdir -p b-qtbase
    (
        cd b-qtbase
        "$base/configure" \
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
    woa64_licences qt6-base "$base"/LICENSES/*

    # qtsvg, with the toolchain file the qtbase install wrote.
    mkdir -p b-qtsvg
    (
        cd b-qtsvg
        "$WOA64_SYSROOT/bin/qt-configure-module" "$svg"
        cmake --build . --parallel
        cmake --install .
    )
    woa64_licences qt6-svg "$svg"/LICENSES/*

    # qttranslations' qtbase_*.qm. Its own build wants the target Qt's
    # Linguist package, i.e. a cross-built qttools, but a .qm file has no
    # machine code: Fedora's lrelease-qt6 compiles the same sources to the
    # same files.
    local ts
    mkdir -p "$WOA64_SYSROOT/share/qt6/translations"
    for ts in "$tr"/translations/qtbase_*.ts; do
        lrelease-qt6 -silent "$ts" -qm "$WOA64_SYSROOT/share/qt6/translations/$(basename "${ts%.ts}").qm"
    done
    woa64_licences qt6-translations "$tr"/LICENSES/*
}

# Records what each shipped file was built from, in the sysroot's licence
# manifest (manifest_add in tools/build-env.sh; xz is recorded by
# cross_build_xz). Patterns match a file's path in the package; the first
# match wins, so qtsvg's come before qtbase's broader ones, which would
# otherwise take Qt6Svg.dll and the SVG plugins. Each package's licence files
# are the ones the build steps above copied into share/licenses/.
woa64_record_sources()
{
    local r=$WOA64_SYSROOT p f
    local qtlic="LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only"
    local -n V=WOA64_SRC_VERSION U=WOA64_SRC_URL
    for f in libc++.dll libunwind.dll; do
        manifest_add "$r" "$f" llvm-mingw "$WOA64_LLVM_MINGW_VERSION" \
            "Apache-2.0 WITH LLVM-exception" "$WOA64_LLVM_MINGW_URL"
    done
    manifest_add "$r" libz.dll zlib "${V[zlib]}" Zlib "${U[zlib]}"
    manifest_add "$r" libzstd.dll zstd "${V[zstd]}" "BSD-3-Clause OR GPL-2.0-only" "${U[zstd]}"
    manifest_add "$r" libbz2-1.dll bzip2 "${V[bzip2]}" bzip2-1.0.6 "${U[bzip2]}"
    for p in 'Qt6Svg*.dll' 'imageformats/qsvg*.dll' 'iconengines/qsvgicon*.dll'; do
        manifest_add "$r" "$p" qt6-svg "${V[qt6-svg]}" "$qtlic" "${U[qt6-svg]}"
    done
    for p in 'Qt6*.dll' 'platforms/*' 'styles/*' 'imageformats/*' 'iconengines/*' 'generic/*'; do
        manifest_add "$r" "$p" qt6-base "${V[qt6-base]}" "$qtlic" "${U[qt6-base]}"
    done
    manifest_add "$r" 'translations/qtbase_*.qm' qt6-translations "${V[qt6-translations]}" \
        "GPL-3.0-only WITH Qt-GPL-exception-1.0" "${U[qt6-translations]}"
}

woa64_install()
{
    # shellcheck disable=SC2086   # deliberate word splitting
    dnf -y install $CROSS_DNF_FLAGS $WOA64_PACKAGES
    srpm_keys

    mkdir -p "$WOA64_SYSROOT" "$WOA64_ROOT/src"
    (
        set -e
        cd "$WOA64_ROOT/src"
        woa64_install_toolchain
        woa64_build_libraries
        woa64_build_qt
        woa64_record_sources
        echo "$WOA64_LLVM_MINGW_VERSION" > "$WOA64_LLVM_MINGW_STAMP"
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

# woa64_stale
#
# Whether anything the toolkit was built from has been updated since: a newer
# source RPM for any library or Qt (srpm_stale), or a newer llvm-mingw release
# when none is pinned. Updates to the build tools alone (cmake, gcc) do not
# count: they do not end up in the package, and the rebuild is long. Prints
# what changed. 0: current, 1: stale, 2: could not tell.
woa64_stale()
{
    local rc=0 have
    srpm_stale || rc=$?
    [ "$rc" -le 1 ] || return 2
    if [ -z "$WOA64_LLVM_MINGW_PIN" ]; then
        have=$(cat "$WOA64_LLVM_MINGW_STAMP" 2>/dev/null) || have=""
        woa64_llvm_mingw_release || return 2
        if [ "$have" != "$WOA64_LLVM_MINGW_VERSION" ]; then
            echo "llvm-mingw: ${have:-none} -> $WOA64_LLVM_MINGW_VERSION"
            rc=1
        fi
    fi
    return $rc
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
        stale)   woa64_stale ;;
        env)     woa64_env ;;
        print)
            case "${1:-}" in
                IMAGE)      echo "$WOA64_IMAGE" ;;
                BASE_IMAGE) echo "$WOA64_BASE_IMAGE" ;;
                SYSROOT)    echo "$WOA64_SYSROOT" ;;
                TOOLCHAIN)  echo "$WOA64_QT_TOOLCHAIN" ;;
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
