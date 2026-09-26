#!/usr/bin/env bash
# llvm-mingw-qt6, the Windows cross toolkit, in one place: what
# tools/Containerfile.toolkit builds, where its sources come from, and how the
# cross build uses it. One recipe for both targets -- x64 and arm64 differ
# only in the target triple and the directory the toolkit is installed in.
#
# Qt is built for a Qt Widgets application and no more: qtbase, qtsvg and
# qttranslations' qtbase_*.qm, without ICU, OpenSSL, D-Bus or SQL, with
# zlib, xz, zstd and bzip2 beside it. Nothing in it is particular to
# WinDiskImager; tools/toolkit-publish.sh publishes the image as
# TOOLKIT_REGISTRY for any project that needs the same.
#
# Everything the app links is built here: llvm-mingw (clang, lld and the
# mingw-w64 runtime), zlib, xz, zstd and bzip2, and Qt 6 (qtbase, qtsvg, and
# qttranslations' qtbase_*.qm), all into the target's sysroot. Each records
# itself in that sysroot's licence manifest (manifest_add in
# tools/build-env.sh), so the package lists every library with its version,
# licence and source. arm64qtcross.md has the same steps worked through by
# hand, with what each one showed.
#
# Nothing is pinned. The libraries and Qt are built from the upstream
# tarballs in Fedora's own source RPMs (srpm_fetch in tools/build-env.sh), so
# they are the versions the Fedora release ships, updates included: Qt the
# same build as the host Qt it cross-compiles with; zlib, zstd and bzip2 what
# Fedora's mingw64 packages are built from; xz Fedora's own. llvm-mingw is its
# newest GitHub release, unless TOOLKIT_LLVM_MINGW_PIN names one. The toolkit
# records what it was built from, and `stale` compares that with what is
# available now, so the container build rebuilds it when any of it has been
# updated.
#
# The two targets share everything but their sysroots: the host packages and
# llvm-mingw, which targets every Windows architecture from one install, are
# the "host" part, and each target's sysroot is built on it. One image
# (tools/Containerfile.toolkit) holds both. It runs the same on any Fedora
# host as in the image:
#
#   sudo tools/toolkit-env.sh ARCH install     # host part and ARCH's sysroot
#   sudo tools/toolkit-env.sh all install      # host part and both sysroots
#   sudo tools/toolkit-env.sh host install     # the host part alone
#   sudo tools/toolkit-env.sh ARCH sysroot     # ARCH's sysroot, on an installed host part
#   tools/toolkit-env.sh ARCH|all check        # assert it is all there
#   tools/toolkit-env.sh ARCH|all stale        # have updates come out since?
#   tools/toolkit-env.sh ARCH env              # the exports a cross build needs
#   tools/toolkit-env.sh ARCH print NAME       # one value (IMAGE, SYSROOT, TOOLCHAIN, ...)
#
# ARCH is x64 or arm64. tools/build-cross.sh and tools/deploy-cross.sh take
# the same ARCH and set up the rest themselves (toolkit_use).
#
# Copyright (C) 2026 peacepenguin, GPL-2.0-or-later.

. "$(dirname "${BASH_SOURCE[0]}")/build-env.sh"

# -------------------------------------------------------------- the target ---

# Every target the toolkit is built for.
TOOLKIT_TARGETS="x64 arm64"

# toolkit_usage -- the commands, from the comment above.
toolkit_usage()
{
    sed -n '/as in the image:$/,/^# ARCH is/p' "${BASH_SOURCE[0]}" | sed '1d;$d' |
        sed 's/^# \{0,1\}//'
}

# Sourced with TOOLKIT_ARCH set, or run with ARCH as the first argument.
# "all" and "host" are both targets, or the part they share: the image, but
# no one target's paths.
if [ "${BASH_SOURCE[0]}" = "$0" ]; then
    TOOLKIT_ARCH=${1:-}
    shift || true
fi
case "${TOOLKIT_ARCH:-}" in
    all|host)
        ;;
    x64)
        TOOLKIT_TRIPLE=x86_64-w64-mingw32
        TOOLKIT_PROCESSOR=AMD64         # CMAKE_SYSTEM_PROCESSOR, as Windows names it
        TOOLKIT_FILE_ARCH='x86-64'      # what file(1) says of the exe
        ;;
    arm64)
        TOOLKIT_TRIPLE=aarch64-w64-mingw32
        TOOLKIT_PROCESSOR=ARM64
        TOOLKIT_FILE_ARCH='ARM64|Aarch64'
        ;;
    *)
        echo "error: the target is x64 or arm64, not '${TOOLKIT_ARCH:-}'" >&2
        if [ "${BASH_SOURCE[0]}" = "$0" ]; then
            toolkit_usage >&2
            exit 2
        fi
        return 2
        ;;
esac

# ------------------------------------------------------------------ sources ---

# The base image: Fedora, whatever its release. The host Qt and the Qt source
# are one build (toolkit_build_qt), so they cannot disagree.
TOOLKIT_BASE_IMAGE="quay.io/fedora/fedora-minimal:latest"

# A release tag (e.g. 20260922) to hold llvm-mingw at, when the newest one
# breaks something; empty for the newest.
TOOLKIT_LLVM_MINGW_PIN=""
TOOLKIT_LLVM_MINGW_REPO=mstorsjo/llvm-mingw

# Host packages. The host Qt: moc, rcc and uic for the cross builds, and
# tools/mkicon, which renders the icon during the app's build (with gcc-c++).
# qt6-linguist: lrelease-qt6 for the translations, lupdate-qt6 for
# tools/lupdate.sh. cpio for srpm_fetch, and gzip and xz for the tarballs it
# unpacks; python3 to read GitHub's release list; file for build-cross.sh's
# check of what it built.
TOOLKIT_PACKAGES="cmake ninja-build file findutils curl tar gzip xz cpio make perl-interpreter python3
                  gcc-c++
                  qt6-qtbase-devel qt6-qtsvg-devel qt6-linguist"

# ------------------------------------------------------------------- paths ---

# llvm-mingw targets every Windows architecture from one install, so the two
# toolkits share it; each has its own sysroot.
TOOLKIT_LLVM_MINGW="${TOOLKIT_LLVM_MINGW:-/opt/llvm-mingw}"
TOOLKIT_ROOT="${TOOLKIT_ROOT:-/opt/win-$TOOLKIT_ARCH}"
TOOLKIT_SYSROOT="$TOOLKIT_ROOT/sysroot"
# The toolchain the libraries and Qt are built with ...
TOOLKIT_TOOLCHAIN="$TOOLKIT_ROOT/toolchain-$TOOLKIT_TRIPLE.cmake"
# ... and the one Qt installs for building against it, which loads that one
# and adds the target Qt and the host tools. The app is built with this.
TOOLKIT_QT_TOOLCHAIN="$TOOLKIT_SYSROOT/lib/cmake/Qt6/qt.toolchain.cmake"

# What this toolkit was built from, for toolkit_stale: the source RPMs
# (srpm_fetch writes SRPM_LOCK) and the llvm-mingw release.
SRPM_LOCK="$TOOLKIT_ROOT/sources.lock"
TOOLKIT_LLVM_MINGW_STAMP="$TOOLKIT_ROOT/llvm-mingw.version"

# TOOLKIT_IMAGE_REVISION counts changes to what the image holds or how it is
# laid out -- the install steps below and in tools/build-env.sh, the
# manifest's place -- so a build never runs in an image made for another
# revision (3: the manifest moved to share/llvm-mingw-qt6/).
TOOLKIT_IMAGE_REVISION=3

# The published image (tools/toolkit-publish.sh), which the container build
# pulls: TOOLKIT_REGISTRY:rN is the newest build for revision N. A fork
# publishing its own points this at it.
TOOLKIT_REGISTRY="${TOOLKIT_REGISTRY:-ghcr.io/peacepenguin/llvm-mingw-qt6}"
TOOLKIT_PUBLISHED="$TOOLKIT_REGISTRY:r$TOOLKIT_IMAGE_REVISION"

# The image built here instead, when TOOLKIT_IMAGE_SOURCE=local or the
# published one cannot be had: tagged by what it is asked to be -- the
# targets, everything above and the revision. Updates to what it was built
# from rebuild it in place (container_stale). Override with TOOLKIT_IMAGE=...
TOOLKIT_IMAGE="${TOOLKIT_IMAGE:-llvm-mingw-qt6:$(printf '%s' \
    "$TOOLKIT_TARGETS$TOOLKIT_BASE_IMAGE$TOOLKIT_PACKAGES$CROSS_DNF_FLAGS$TOOLKIT_LLVM_MINGW_PIN$TOOLKIT_IMAGE_REVISION" \
    | cksum | cut -d' ' -f1)}"

# What each library was built from, for toolkit_record_sources: version and
# source address by package name, filled in by toolkit_got.
declare -A TOOLKIT_SRC_VERSION=() TOOLKIT_SRC_URL=()

# --------------------------------------------------------------- functions ---

# toolkit_llvm_mingw_release
#
# Sets TOOLKIT_LLVM_MINGW_VERSION, _URL and _SHA256 from GitHub's API: the
# newest release's tag (or TOOLKIT_LLVM_MINGW_PIN's), its Linux x86_64 UCRT
# build, and the SHA-256 GitHub computed for that file on upload. A release
# without one is refused, since the download could not then be checked.
#
# A GitHub token, when there is one, lifts the API's limit of 60 anonymous
# requests an hour: the build secret container_run passes into the image
# build, or GITHUB_TOKEN (the stale check, a Fedora host). It reaches curl on
# stdin, not its command line, where any process could read it.
toolkit_llvm_mingw_release()
{
    local api="https://api.github.com/repos/$TOOLKIT_LLVM_MINGW_REPO/releases/latest" json line
    [ -z "$TOOLKIT_LLVM_MINGW_PIN" ] ||
        api="https://api.github.com/repos/$TOOLKIT_LLVM_MINGW_REPO/releases/tags/$TOOLKIT_LLVM_MINGW_PIN"
    local tok=${GITHUB_TOKEN:-}
    [ ! -r /run/secrets/github_token ] || tok=$(cat /run/secrets/github_token)
    json=$( { [ -z "$tok" ] || printf 'header = "Authorization: Bearer %s"\n' "$tok"; } |
            curl -fsSL -K - "$api") || {
        echo "error: could not read llvm-mingw's release ${TOOLKIT_LLVM_MINGW_PIN:-latest} from $api" >&2
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
    read -r TOOLKIT_LLVM_MINGW_VERSION TOOLKIT_LLVM_MINGW_URL TOOLKIT_LLVM_MINGW_SHA256 <<< "$line"
}

# toolkit_got PACKAGE -- notes the source srpm_fetch just fetched as PACKAGE's.
toolkit_got()
{
    TOOLKIT_SRC_VERSION[$1]=$SRPM_VERSION
    TOOLKIT_SRC_URL[$1]=$SRPM_URL
}

# toolkit_licences PACKAGE FILE... -- keeps a library's licence files where
# deploy_write_licenses looks for them.
toolkit_licences()
{
    local pkg=${1:?usage: toolkit_licences PACKAGE FILE...}
    shift
    mkdir -p "$TOOLKIT_SYSROOT/share/licenses/$pkg"
    cp "$@" "$TOOLKIT_SYSROOT/share/licenses/$pkg/"
}

# toolkit_install_llvm_mingw
#
# llvm-mingw into TOOLKIT_LLVM_MINGW, in the current directory's scratch: the
# release toolkit_llvm_mingw_release names, unless that one is there already.
# .release and .release-url record which, for the sysroots built on it.
toolkit_install_llvm_mingw()
{
    toolkit_llvm_mingw_release
    local have=""
    [ ! -f "$TOOLKIT_LLVM_MINGW/.release" ] || have=$(cat "$TOOLKIT_LLVM_MINGW/.release")
    if [ "$have" != "$TOOLKIT_LLVM_MINGW_VERSION" ]; then
        local file=${TOOLKIT_LLVM_MINGW_URL##*/}
        curl -fsSL -o "$file" "$TOOLKIT_LLVM_MINGW_URL"
        echo "$TOOLKIT_LLVM_MINGW_SHA256  $file" | sha256sum -c --quiet -
        tar -xf "$file"
        rm -f "$file"
        rm -rf "$TOOLKIT_LLVM_MINGW"
        mv "${file%.tar.xz}" "$TOOLKIT_LLVM_MINGW"
        echo "$TOOLKIT_LLVM_MINGW_VERSION" > "$TOOLKIT_LLVM_MINGW/.release"
    fi
    echo "$TOOLKIT_LLVM_MINGW_URL" > "$TOOLKIT_LLVM_MINGW/.release-url"
}

# toolkit_llvm_mingw_installed -- sets TOOLKIT_LLVM_MINGW_VERSION and _URL
# from the installed llvm-mingw, for a sysroot built on it.
toolkit_llvm_mingw_installed()
{
    if [ ! -f "$TOOLKIT_LLVM_MINGW/.release" ] || [ ! -f "$TOOLKIT_LLVM_MINGW/.release-url" ]; then
        echo "error: no llvm-mingw in $TOOLKIT_LLVM_MINGW;" \
             "sudo tools/toolkit-env.sh host install puts it there" >&2
        return 1
    fi
    TOOLKIT_LLVM_MINGW_VERSION=$(cat "$TOOLKIT_LLVM_MINGW/.release")
    TOOLKIT_LLVM_MINGW_URL=$(cat "$TOOLKIT_LLVM_MINGW/.release-url")
}

# toolkit_target_toolchain
#
# This target's cmake toolchain file, for llvm-mingw's clang for its triple,
# and llvm-mingw's C++ runtime DLLs into its sysroot.
toolkit_target_toolchain()
{
    local bin="$TOOLKIT_LLVM_MINGW/bin/$TOOLKIT_TRIPLE"
    cat > "$TOOLKIT_TOOLCHAIN" <<EOF
# Cross-compile for Windows $TOOLKIT_ARCH with llvm-mingw; written by tools/toolkit-env.sh.
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR $TOOLKIT_PROCESSOR)

set(CMAKE_C_COMPILER   $bin-clang)
set(CMAKE_CXX_COMPILER $bin-clang++)
set(CMAKE_RC_COMPILER  $bin-windres)
set(CMAKE_AR           $TOOLKIT_LLVM_MINGW/bin/llvm-ar)
set(CMAKE_RANLIB       $TOOLKIT_LLVM_MINGW/bin/llvm-ranlib)

# Libraries, headers and CMake packages from the target sysroot (and the
# toolchain's own); programs from the host.
set(CMAKE_FIND_ROOT_PATH $TOOLKIT_SYSROOT $TOOLKIT_LLVM_MINGW/$TOOLKIT_TRIPLE)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
EOF

    # The C++ runtime everything built with clang++ needs, into the sysroot so
    # deploy-cross.sh finds it with the rest. LICENSE.TXT is LLVM's (libc++,
    # libunwind); the runtime COPYING covers the mingw-w64 startup code.
    local rt="$TOOLKIT_LLVM_MINGW/$TOOLKIT_TRIPLE"
    mkdir -p "$TOOLKIT_SYSROOT/bin"
    cp "$rt/bin/libc++.dll" "$rt/bin/libunwind.dll" "$TOOLKIT_SYSROOT/bin/"
    toolkit_licences llvm-mingw "$TOOLKIT_LLVM_MINGW/LICENSE.TXT" \
        "$rt/share/mingw32/COPYING.MinGW-w64-runtime.txt"
}

# toolkit_build_xz
#
# liblzma, from the source of Fedora's xz package: the version Fedora ships,
# with its updates. Only the library: none of the xz tools, translations,
# documentation or tests. Fedora's mingw64-xz is not the source: it has stayed
# at 5.2.4 (2018), where the multi-threaded decoder imagesource.cpp uses
# needs 5.4 (CROSS_XZ_MIN). Its licence comes from the same source tree --
# COPYING says which licence covers what, COPYING.0BSD is liblzma's.
toolkit_build_xz()
{
    srpm_fetch xz 'xz-*.tar.*'
    toolkit_got xz
    cmake -S "$SRPM_SRCDIR" -B b-xz "$@" \
        -DBUILD_SHARED_LIBS=ON -DBUILD_TESTING=OFF \
        -DXZ_NLS=OFF -DXZ_DOC=OFF \
        -DXZ_TOOL_XZ=OFF -DXZ_TOOL_XZDEC=OFF -DXZ_TOOL_LZMADEC=OFF -DXZ_TOOL_LZMAINFO=OFF
    cmake --build b-xz
    cmake --install b-xz
    toolkit_licences xz "$SRPM_SRCDIR/COPYING" "$SRPM_SRCDIR/COPYING.0BSD"
}

toolkit_build_libraries()
{
    local X=(-G Ninja -DCMAKE_TOOLCHAIN_FILE="$TOOLKIT_TOOLCHAIN" -DCMAKE_BUILD_TYPE=Release
             -DCMAKE_INSTALL_PREFIX="$TOOLKIT_SYSROOT")

    # zlib, zstd and bzip2 from the source RPMs of Fedora's mingw64 packages.
    # Not Fedora's native zlib: that is zlib-ng.
    srpm_fetch mingw64-zlib 'zlib-*.tar.*'
    toolkit_got zlib
    cmake -S "$SRPM_SRCDIR" -B b-zlib "${X[@]}"
    cmake --build b-zlib
    cmake --install b-zlib
    toolkit_licences zlib "$SRPM_SRCDIR/LICENSE"

    toolkit_build_xz "${X[@]}"

    srpm_fetch mingw64-zstd 'zstd-*.tar.*'
    toolkit_got zstd
    cmake -S "$SRPM_SRCDIR/build/cmake" -B b-zstd "${X[@]}" \
        -DZSTD_BUILD_PROGRAMS=OFF -DZSTD_BUILD_TESTS=OFF \
        -DZSTD_BUILD_STATIC=OFF -DZSTD_BUILD_SHARED=ON -DZSTD_MULTITHREAD_SUPPORT=ON
    cmake --build b-zstd
    cmake --install b-zstd
    toolkit_licences zstd "$SRPM_SRCDIR/LICENSE" "$SRPM_SRCDIR/COPYING"

    # 1.0.8 has only a Unix Makefile, so its seven sources are built
    # directly. bzlib.h's _WIN32 branch declares every function as a pointer
    # for loading the DLL by hand; MinGW builds (Fedora's, MSYS2's) take the
    # ordinary branch, and so does this one. LLD exports every function, as
    # GNU ld does for MinGW, since none is marked for export.
    srpm_fetch mingw64-bzip2 'bzip2-*.tar.*'
    toolkit_got bzip2
    local bz=$SRPM_SRCDIR cc="$TOOLKIT_LLVM_MINGW/bin/$TOOLKIT_TRIPLE-clang"
    (
        cd "$bz"
        grep -q '^#ifdef _WIN32$' bzlib.h
        sed -i 's/^#ifdef _WIN32$/#if defined(_WIN32) \&\& !defined(__MINGW32__)/' bzlib.h
        local f
        for f in blocksort huffman crctable randtable compress decompress bzlib; do
            "$cc" -O2 -D_FILE_OFFSET_BITS=64 -c "$f.c"
        done
        "$cc" -shared -o libbz2-1.dll \
            blocksort.o huffman.o crctable.o randtable.o compress.o decompress.o bzlib.o \
            -Wl,--out-implib,libbz2.dll.a
        mkdir -p "$TOOLKIT_SYSROOT/bin" "$TOOLKIT_SYSROOT/lib" "$TOOLKIT_SYSROOT/include"
        cp libbz2-1.dll "$TOOLKIT_SYSROOT/bin/"
        cp libbz2.dll.a "$TOOLKIT_SYSROOT/lib/"
        cp bzlib.h "$TOOLKIT_SYSROOT/include/"
    )
    toolkit_licences bzip2 "$bz/LICENSE"
}

toolkit_build_qt()
{
    # The source of the very builds the host Qt was installed from, so the
    # two are the same version by construction. qttranslations has no host
    # package here, so its newest is taken and checked against that version.
    local v base svg tr
    v=$(rpm -q --qf '%{VERSION}' qt6-qtbase)
    srpm_fetch qt6-qtbase 'qtbase-everywhere-*src-*.tar.*' "$(rpm -q --qf '%{VERSION}-%{RELEASE}' qt6-qtbase)"
    toolkit_got qt6-base
    base=$SRPM_SRCDIR
    srpm_fetch qt6-qtsvg 'qtsvg-everywhere-*src-*.tar.*' "$(rpm -q --qf '%{VERSION}-%{RELEASE}' qt6-qtsvg)"
    toolkit_got qt6-svg
    svg=$SRPM_SRCDIR
    srpm_fetch qt6-qttranslations 'qttranslations-everywhere-*src-*.tar.*'
    toolkit_got qt6-translations
    tr=$SRPM_SRCDIR
    local m
    for m in qt6-base qt6-svg qt6-translations; do
        if [ "${TOOLKIT_SRC_VERSION[$m]}" != "$v" ]; then
            echo "error: $m's source is ${TOOLKIT_SRC_VERSION[$m]}, but the host Qt is $v;" >&2
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
            -prefix "$TOOLKIT_SYSROOT" \
            -xplatform win32-clang-g++ \
            -qt-host-path /usr \
            -plugindir lib/qt6/plugins -translationdir share/qt6/translations \
            -release -shared \
            -nomake examples -nomake tests \
            -no-icu -no-openssl -no-dbus -no-sql-sqlite \
            -system-zlib -qt-libpng -qt-libjpeg -qt-harfbuzz -qt-freetype -qt-pcre \
            -- -DCMAKE_TOOLCHAIN_FILE="$TOOLKIT_TOOLCHAIN" \
               -DQT_HOST_PATH_CMAKE_DIR=/usr/lib64/cmake
        cmake --build . --parallel
        cmake --install .
    )
    toolkit_licences qt6-base "$base"/LICENSES/*

    # qtsvg, with the toolchain file the qtbase install wrote.
    mkdir -p b-qtsvg
    (
        cd b-qtsvg
        "$TOOLKIT_SYSROOT/bin/qt-configure-module" "$svg"
        cmake --build . --parallel
        cmake --install .
    )
    toolkit_licences qt6-svg "$svg"/LICENSES/*

    # qttranslations' qtbase_*.qm. Its own build wants the target Qt's
    # Linguist package, i.e. a cross-built qttools, but a .qm file has no
    # machine code: Fedora's lrelease-qt6 compiles the same sources to the
    # same files.
    local ts
    mkdir -p "$TOOLKIT_SYSROOT/share/qt6/translations"
    for ts in "$tr"/translations/qtbase_*.ts; do
        lrelease-qt6 -silent "$ts" -qm "$TOOLKIT_SYSROOT/share/qt6/translations/$(basename "${ts%.ts}").qm"
    done
    toolkit_licences qt6-translations "$tr"/LICENSES/*
}

# Records what each shipped file was built from, in the sysroot's licence
# manifest (manifest_add in tools/build-env.sh). Patterns match a file's path
# in the package; the first match wins, so qtsvg's come before qtbase's
# broader ones, which would otherwise take Qt6Svg.dll and the SVG plugins.
# Each package's licence files are the ones the build steps above copied into
# share/licenses/.
toolkit_record_sources()
{
    local r=$TOOLKIT_SYSROOT p f
    local qtlic="LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only"
    local -n V=TOOLKIT_SRC_VERSION U=TOOLKIT_SRC_URL
    for f in libc++.dll libunwind.dll; do
        manifest_add "$r" "$f" llvm-mingw "$TOOLKIT_LLVM_MINGW_VERSION" \
            "Apache-2.0 WITH LLVM-exception" "$TOOLKIT_LLVM_MINGW_URL"
    done
    manifest_add "$r" libz.dll zlib "${V[zlib]}" Zlib "${U[zlib]}"
    # liblzma is 0BSD; COPYING, shipped with it, says so.
    manifest_add "$r" liblzma.dll xz "${V[xz]}" 0BSD "${U[xz]}"
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

# toolkit_install_host
#
# What every target builds on: the host packages, Fedora's signing key for
# srpm_fetch, and llvm-mingw.
toolkit_install_host()
{
    # shellcheck disable=SC2086   # deliberate word splitting
    dnf -y install $CROSS_DNF_FLAGS $TOOLKIT_PACKAGES
    srpm_keys
    local work
    work=$(mktemp -d)
    ( set -e; cd "$work"; toolkit_install_llvm_mingw ) || { rm -rf "$work"; return 1; }
    rm -rf "$work"
}

# toolkit_install_sysroot
#
# This target's sysroot, from scratch, on the installed host part.
toolkit_install_sysroot()
{
    # A fresh sysroot: nothing left from an earlier install can be shipped
    # without the manifest knowing where it came from. TOOLKIT_ROOT can be
    # overridden, so it must at least be a directory of its own.
    case "$TOOLKIT_ROOT" in
        /?*/?*|/opt/?*) ;;
        *) echo "error: refusing to clear TOOLKIT_ROOT='$TOOLKIT_ROOT'" >&2; return 1 ;;
    esac
    rm -rf "$TOOLKIT_ROOT"
    mkdir -p "$TOOLKIT_SYSROOT" "$TOOLKIT_ROOT/src"
    (
        set -e
        cd "$TOOLKIT_ROOT/src"
        toolkit_llvm_mingw_installed
        toolkit_target_toolchain
        toolkit_build_libraries
        toolkit_build_qt
        toolkit_record_sources
        echo "$TOOLKIT_LLVM_MINGW_VERSION" > "$TOOLKIT_LLVM_MINGW_STAMP"
    )
    # Sources and build trees: several GB the image does not need.
    rm -rf "$TOOLKIT_ROOT/src"
    toolkit_check
}

# toolkit_install -- the host part, then this target's sysroot.
toolkit_install()
{
    toolkit_install_host
    toolkit_install_sysroot
}

# toolkit_use
#
# Points the cross build (tools/build-cross.sh, tools/deploy-cross.sh) at this
# target's toolkit: the variables tools/build-env.sh reads, and llvm-mingw on
# PATH for its objdump and strip.
toolkit_use()
{
    export PATH="$TOOLKIT_LLVM_MINGW/bin:$PATH"
    CROSS_TOOLCHAIN=$TOOLKIT_QT_TOOLCHAIN
    CROSS_SYSROOT=$TOOLKIT_SYSROOT
    CROSS_XZ_PREFIX=$TOOLKIT_SYSROOT
    OBJDUMP=llvm-objdump
    STRIP=llvm-strip
}

# Fail early and loudly if the toolkit is not what the cross build expects.
toolkit_check()
{
    local bad=0 f
    for f in "$TOOLKIT_LLVM_MINGW/bin/$TOOLKIT_TRIPLE-clang++" "$TOOLKIT_LLVM_MINGW/bin/llvm-objdump" \
             "$TOOLKIT_LLVM_MINGW/bin/llvm-strip" "$TOOLKIT_TOOLCHAIN" "$TOOLKIT_QT_TOOLCHAIN" \
             "$TOOLKIT_SYSROOT/bin/Qt6Core.dll" "$TOOLKIT_SYSROOT/bin/Qt6Svg.dll" \
             "$TOOLKIT_SYSROOT/lib/qt6/plugins/platforms/qwindows.dll" \
             "$TOOLKIT_SYSROOT/share/qt6/translations/qtbase_de.qm" \
             "$TOOLKIT_SYSROOT/bin/libz.dll" "$TOOLKIT_SYSROOT/bin/liblzma.dll" \
             "$TOOLKIT_SYSROOT/bin/libzstd.dll" "$TOOLKIT_SYSROOT/bin/libbz2-1.dll" \
             "$TOOLKIT_SYSROOT/bin/libc++.dll" "$TOOLKIT_SYSROOT/bin/libunwind.dll" \
             "$TOOLKIT_SYSROOT/$MANIFEST"; do
        [ -e "$f" ] || { echo "missing $f" >&2; bad=1; }
    done
    # The app's own cross_check, as build-cross.sh will run it.
    ( toolkit_use; cross_check ) || bad=1
    return $bad
}

# toolkit_stale
#
# Whether anything the toolkit was built from has been updated since: a newer
# source RPM for any library or Qt (srpm_stale), or a newer llvm-mingw release
# when none is pinned. Updates to the build tools alone (cmake, gcc, mesa) do
# not count: they do not end up in the package. Prints what changed.
# 0: current, 1: stale, 2: could not tell.
toolkit_stale()
{
    local rc=0 have
    srpm_stale || rc=$?
    [ "$rc" -le 1 ] || return 2
    if [ -z "$TOOLKIT_LLVM_MINGW_PIN" ]; then
        have=$(cat "$TOOLKIT_LLVM_MINGW_STAMP" 2>/dev/null) || have=""
        toolkit_llvm_mingw_release || return 2
        if [ "$have" != "$TOOLKIT_LLVM_MINGW_VERSION" ]; then
            echo "llvm-mingw: ${have:-none} -> $TOOLKIT_LLVM_MINGW_VERSION"
            rc=1
        fi
    fi
    return $rc
}

# toolkit_env -- toolkit_use's settings, as export lines, for a shell of
# one's own.
toolkit_env()
{
    cat <<EOF
export PATH=$TOOLKIT_LLVM_MINGW/bin:\$PATH
export CROSS_TOOLCHAIN=$TOOLKIT_QT_TOOLCHAIN
export CROSS_SYSROOT=$TOOLKIT_SYSROOT
export CROSS_XZ_PREFIX=$TOOLKIT_SYSROOT
export OBJDUMP=llvm-objdump
export STRIP=llvm-strip
EOF
}

# toolkit_container_run REPO COMMAND...
#
# container_run in the toolkit image: the same image for every target.
#
# By default the published one, TOOLKIT_PUBLISHED, which
# tools/toolkit-publish.sh keeps current. CONTAINER_REFRESH=1 (the build
# wrapper) pulls it first, which fetches nothing when it has not changed;
# W32DI_REFRESH=0 skips that. If it cannot be had -- offline with no copy
# here, or a revision not published yet -- the image is built here instead.
#
# TOOLKIT_IMAGE_SOURCE=local always builds it here, as TOOLKIT_IMAGE, from
# tools/Containerfile.toolkit: for working on the toolkit itself. Then
# CONTAINER_REFRESH=1 checks it for updates first, for every target.
toolkit_container_run()
{
    CONTAINER_BASE=$TOOLKIT_BASE_IMAGE
    CONTAINER_FILE=tools/Containerfile.toolkit
    CONTAINER_BUILD_ARGS=()
    CONTAINER_STALE=(bash /usr/local/lib/toolkit-env.sh all stale)

    if [ "${TOOLKIT_IMAGE_SOURCE:-published}" = published ] && command -v podman >/dev/null 2>&1; then
        local have=0
        podman image exists "$TOOLKIT_PUBLISHED" && have=1
        if [ "$have" = 0 ] || { [ "${CONTAINER_REFRESH:-0}" = 1 ] && [ "${W32DI_REFRESH:-1}" != 0 ]; }; then
            echo "pulling $TOOLKIT_PUBLISHED (W32DI_REFRESH=0 skips this)..." >&2
            if podman pull -q "$TOOLKIT_PUBLISHED" >/dev/null; then
                have=1
            elif [ "$have" = 1 ]; then
                echo "warning: could not pull $TOOLKIT_PUBLISHED; using the copy here." >&2
            else
                echo "warning: could not pull $TOOLKIT_PUBLISHED; building the toolkit here instead." >&2
            fi
        fi
        if [ "$have" = 1 ]; then
            CONTAINER_IMAGE=$TOOLKIT_PUBLISHED CONTAINER_REFRESH=0 container_run "$@"
            return
        fi
    fi
    CONTAINER_IMAGE=$TOOLKIT_IMAGE
    container_run "$@"
}

# toolkit_each COMMAND -- COMMAND for every target, as its own run of this
# script; returns the worst result (for stale: 0 current, 1 stale, 2 could
# not tell).
toolkit_each()
{
    local a r rc=0
    for a in $TOOLKIT_TARGETS; do
        r=0
        bash "${BASH_SOURCE[0]}" "$a" "$@" || r=$?
        [ "$r" -le "$rc" ] || rc=$r
    done
    return $rc
}

# ----------------------------------------------------------------- command ---

if [ "${BASH_SOURCE[0]}" = "$0" ]; then
    set -euo pipefail
    cmd=${1:-}
    shift || true
    case "$TOOLKIT_ARCH:$cmd" in
        host:install) toolkit_install_host ;;
        all:install)  toolkit_install_host; toolkit_each sysroot ;;
        all:check|all:stale) toolkit_each "$cmd" ;;
        all:print)
            case "${1:-}" in
                IMAGE)      echo "$TOOLKIT_IMAGE" ;;
                PUBLISHED)  echo "$TOOLKIT_PUBLISHED" ;;
                BASE_IMAGE) echo "$TOOLKIT_BASE_IMAGE" ;;
                *) echo "print: 'all' has only IMAGE, PUBLISHED and BASE_IMAGE" >&2; exit 2 ;;
            esac
            ;;
        host:*|all:*)
            toolkit_usage >&2
            exit 2
            ;;
        *:install) toolkit_install ;;
        *:sysroot) toolkit_install_sysroot ;;
        *:check)   toolkit_check ;;
        *:stale)   toolkit_stale ;;
        *:env)     toolkit_env ;;
        *:print)
            case "${1:-}" in
                IMAGE)      echo "$TOOLKIT_IMAGE" ;;
                PUBLISHED)  echo "$TOOLKIT_PUBLISHED" ;;
                BASE_IMAGE) echo "$TOOLKIT_BASE_IMAGE" ;;
                ROOT)       echo "$TOOLKIT_ROOT" ;;
                SYSROOT)    echo "$TOOLKIT_SYSROOT" ;;
                TOOLCHAIN)  echo "$TOOLKIT_QT_TOOLCHAIN" ;;
                TRIPLE)     echo "$TOOLKIT_TRIPLE" ;;
                *) echo "print: unknown name '${1:-}'" >&2; exit 2 ;;
            esac
            ;;
        *)
            toolkit_usage
            [ -n "$cmd" ] && exit 2
            exit 0
            ;;
    esac
fi
