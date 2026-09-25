#!/usr/bin/env bash
# The cross-build environment, in one place: which Fedora packages the toolchain
# needs, where that toolchain lives, and how cmake is invoked against it.
#
# Everything that cross-builds reads this file, so none of it is written down
# twice: tools/Containerfile.build, tools/build-cross.sh, tools/build-container.sh,
# tools/deploy-cross.sh, tools/deploy-container.sh, tools/lupdate.sh and
# .github/workflows/build.yml.
# BUILD.md points here rather than repeating the values.
#
# Use it either way. As a library:
#
#   . tools/build-env.sh
#   cross_configure /src/src /src/build
#
# or as a command:
#
#   tools/build-env.sh install            # dnf install the toolchain
#   tools/build-env.sh check              # assert the layout is as expected
#   tools/build-env.sh configure SRC BUILD [extra cmake args...]
#   tools/build-env.sh packages           # print the package list
#   tools/build-env.sh packages-msys2     # print the MSYS2 list, for a native build
#   tools/build-env.sh print NAME         # print one value (SYSROOT, TOOLCHAIN, ...)
#
# Copyright (C) 2026 peacepenguin, GPL-2.0-or-later.

# ---------------------------------------------------------------- toolchain ---

# Fedora is not a preference: Debian and Ubuntu ship no MinGW Qt6 packages, so
# there is nothing to link against there. The container and CI both build from
# this. A local image is cached under a tag derived from this string, so it
# keeps the Fedora it was built from until it is removed and rebuilt; CI builds
# its image fresh each run.
CROSS_BASE_IMAGE="fedora:latest"

# qt6-linguist is the *native* Linguist: lrelease-qt6 compiles lang/*.ts for the
# build, lupdate-qt6 serves tools/lupdate.sh. The mingw64-qt6-qttools copies are
# Windows .exe files and cannot run here. gcc-c++ and the native qt6 -devel
# packages are only for tools/mkicon, which renders the icon during the build
# and so must run on the build host.
CROSS_PACKAGES="cmake ninja-build file findutils binutils curl tar xz
                mingw64-gcc-c++ mingw64-qt6-qtbase mingw64-qt6-qttools
                mingw64-qt6-qttranslations mingw64-qt6-qtsvg
                mingw64-zlib mingw64-bzip2 mingw64-zstd
                qt6-linguist
                gcc-c++ qt6-qtbase-devel qt6-qtsvg-devel"

# liblzma for the cross build is built from xz's own release instead of taken
# from Fedora: mingw64-xz has stayed at 5.2.4 (2018), and the multi-threaded
# decoder imagesource.cpp uses needs 5.4. It goes in a prefix of its own, so
# no file an rpm owns is overwritten, and it names its DLL liblzma.dll where
# Fedora's is liblzma-5.dll, so the two cannot be mistaken for each other.
# cross_build_xz builds it; cross_configure and the deploy functions find it
# there. The checksum is the release tarball's, as GitHub publishes it.
CROSS_XZ_VERSION=5.8.4
CROSS_XZ_SHA256=4ce24038fd4221e0d13bc1a2de7a4db56e90b92b3bf75321f6c14be73f65de4b
CROSS_XZ_URL="https://github.com/tukaani-project/xz/releases/download/v$CROSS_XZ_VERSION/xz-$CROSS_XZ_VERSION.tar.xz"
CROSS_XZ_PREFIX="${CROSS_XZ_PREFIX:-/opt/mingw64-xz}"

# The MSYS2 UCRT64 packages for a native Windows build. Nothing here installs
# them -- that is done by hand, once -- but the list belongs with the others.
MSYS2_PACKAGES="mingw-w64-ucrt-x86_64-gcc
                mingw-w64-ucrt-x86_64-cmake
                mingw-w64-ucrt-x86_64-ninja
                mingw-w64-ucrt-x86_64-qt6-base
                mingw-w64-ucrt-x86_64-qt6-tools
                mingw-w64-ucrt-x86_64-qt6-translations
                mingw-w64-ucrt-x86_64-qt6-svg
                mingw-w64-ucrt-x86_64-zlib
                mingw-w64-ucrt-x86_64-xz
                mingw-w64-ucrt-x86_64-bzip2
                mingw-w64-ucrt-x86_64-zstd"

# Where Fedora's mingw64 packages put things; each can be overridden from the
# environment.
CROSS_TOOLCHAIN="${CROSS_TOOLCHAIN:-/usr/share/mingw/toolchain-mingw64.cmake}"
CROSS_LRELEASE="${CROSS_LRELEASE:-/usr/bin/lrelease-qt6}"
CROSS_LUPDATE="${CROSS_LUPDATE:-/usr/bin/lupdate-qt6}"
CROSS_SYSROOT="${CROSS_SYSROOT:-/usr/x86_64-w64-mingw32/sys-root/mingw}"

# The host Qt's cmake packages, which tools/mkicon builds against.
CROSS_NATIVE_QT="${CROSS_NATIVE_QT:-/usr/lib64/cmake/Qt6/Qt6Config.cmake}"
CROSS_NATIVE_QTSVG="${CROSS_NATIVE_QTSVG:-/usr/lib64/cmake/Qt6Svg/Qt6SvgConfig.cmake}"

# tsflags= undoes the nodocs Fedora's container images set: some packages
# (mingw64-bzip2) file their licence as %doc, and deploy_write_licenses needs
# it on disk to ship it.
CROSS_DNF_FLAGS="--setopt=tsflags="

# The image tools/Containerfile.build produces. Override with IMAGE=...
# The tag is a checksum of the base image, package list, install flags and
# the xz release built from source:
# container_run only builds an image that does not exist yet, so a changed
# toolchain needs a new name.
CROSS_IMAGE="${IMAGE:-w32di-build:$(printf '%s' "$CROSS_BASE_IMAGE$CROSS_PACKAGES$CROSS_DNF_FLAGS$CROSS_XZ_VERSION$CROSS_XZ_SHA256" | cksum | cut -d' ' -f1)}"

# Extra "podman run" arguments a caller wants, as an array.
CONTAINER_ENV=()

# ---------------------------------------------------------------- functions ---

cross_packages()
{
    # shellcheck disable=SC2086   # deliberate word splitting: one per line
    echo $CROSS_PACKAGES
}

cross_install()
{
    # shellcheck disable=SC2046
    dnf -y install $CROSS_DNF_FLAGS $(cross_packages)
    cross_build_xz
    cross_check
}

# cross_build_xz
#
# Builds liblzma CROSS_XZ_VERSION for win64 into CROSS_XZ_PREFIX, from the
# release tarball, checked against CROSS_XZ_SHA256 before anything in it runs.
# Only the library: none of the xz tools, translations, documentation or tests.
# Its licence comes from the same source tree -- COPYING says which licence
# covers what, COPYING.0BSD is liblzma's -- and is kept where an rpm would put
# it, in share/licenses/xz/, for deploy_write_licenses to ship.
cross_build_xz()
{
    local work src
    work=$(mktemp -d)
    src="$work/xz-$CROSS_XZ_VERSION"
    curl -fsSL -o "$work/xz.tar.xz" "$CROSS_XZ_URL"
    echo "$CROSS_XZ_SHA256  $work/xz.tar.xz" | sha256sum -c --quiet -
    tar -xf "$work/xz.tar.xz" -C "$work"
    cmake -S "$src" -B "$work/build" -G Ninja \
        -DCMAKE_TOOLCHAIN_FILE="$CROSS_TOOLCHAIN" \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX="$CROSS_XZ_PREFIX" \
        -DBUILD_SHARED_LIBS=ON -DBUILD_TESTING=OFF \
        -DXZ_NLS=OFF -DXZ_DOC=OFF \
        -DXZ_TOOL_XZ=OFF -DXZ_TOOL_XZDEC=OFF -DXZ_TOOL_LZMADEC=OFF -DXZ_TOOL_LZMAINFO=OFF
    cmake --build "$work/build"
    cmake --install "$work/build"
    mkdir -p "$CROSS_XZ_PREFIX/share/licenses/xz"
    cp "$src/COPYING" "$src/COPYING.0BSD" "$CROSS_XZ_PREFIX/share/licenses/xz/"
    rm -rf "$work"
}

# Fail early and loudly if the layout is not what the build and deploy expect.
cross_check()
{
    local bad=0
    [ -f "$CROSS_TOOLCHAIN" ] || { echo "missing $CROSS_TOOLCHAIN" >&2; bad=1; }
    [ -x "$CROSS_LRELEASE" ]  || { echo "missing $CROSS_LRELEASE" >&2; bad=1; }
    [ -x "$CROSS_LUPDATE" ]   || { echo "missing $CROSS_LUPDATE" >&2; bad=1; }
    [ -d "$CROSS_SYSROOT" ]   || { echo "missing $CROSS_SYSROOT" >&2; bad=1; }
    [ -f "$CROSS_NATIVE_QT" ] || {
        echo "missing $CROSS_NATIVE_QT (qt6-qtbase-devel), which tools/mkicon needs" >&2; bad=1; }
    [ -f "$CROSS_NATIVE_QTSVG" ] || {
        echo "missing $CROSS_NATIVE_QTSVG (qt6-qtsvg-devel), which tools/mkicon needs" >&2; bad=1; }
    # The header's version, so a prefix left over from an older
    # CROSS_XZ_VERSION is not taken for this one.
    local xzh="$CROSS_XZ_PREFIX/include/lzma/version.h" xzv
    xzv=$(sed -n 's/^#define LZMA_VERSION_\(MAJOR\|MINOR\|PATCH\) \([0-9]*\)$/\2/p' "$xzh" 2>/dev/null \
          | paste -sd.)
    [ "$xzv" = "$CROSS_XZ_VERSION" ] && [ -f "$CROSS_XZ_PREFIX/lib/liblzma.dll.a" ] \
        && [ -f "$CROSS_XZ_PREFIX/share/licenses/xz/COPYING" ] || {
        echo "missing liblzma $CROSS_XZ_VERSION in $CROSS_XZ_PREFIX (found '${xzv:-none}');" \
             "tools/build-env.sh install builds it" >&2; bad=1; }
    return $bad
}

# cross_configure SRCDIR BUILDDIR [extra cmake args...]
#
# LRELEASE_EXECUTABLE matters: Qt6::lrelease from a MinGW Qt is a Windows .exe
# and cannot run on the build host, so the native lrelease-qt6 compiles the
# translations instead.
cross_configure()
{
    local src=${1:?usage: cross_configure SRCDIR BUILDDIR [cmake args...]}
    local build=${2:?}
    shift 2
    # liblzma from cross_build_xz, named outright: FindLibLZMA then searches
    # nowhere, so Fedora's older copy cannot be picked up instead.
    cmake -S "$src" -B "$build" -G Ninja \
        -DCMAKE_TOOLCHAIN_FILE="$CROSS_TOOLCHAIN" \
        -DCMAKE_BUILD_TYPE=Release \
        -DLRELEASE_EXECUTABLE="$CROSS_LRELEASE" \
        -DLIBLZMA_INCLUDE_DIR="$CROSS_XZ_PREFIX/include" \
        -DLIBLZMA_LIBRARY="$CROSS_XZ_PREFIX/lib/liblzma.dll.a" \
        "$@"
}

# lupdate_path
#
# Prints the path to a Qt 6 lupdate, or nothing. Each platform names it
# differently: Fedora's qt6-linguist installs lupdate-qt6, MSYS2 UCRT64 installs
# plain lupdate. A bare "lupdate" on a Fedora host is usually Qt 5's, which
# writes .ts files the Qt 6 lrelease then has to make sense of, so every
# candidate is asked its version and only 6 is accepted.
lupdate_path()
{
    local c p
    for c in "$CROSS_LUPDATE" lupdate-qt6 lupdate6 lupdate; do
        if [ -x "$c" ]; then
            p=$c
        else
            p=$(command -v "$c" 2>/dev/null) || continue
        fi
        case "$("$p" -version 2>/dev/null)" in
            *"lupdate version 6"*) echo "$p"; return 0 ;;
        esac
    done
    return 1
}

# drop_foreign_cache BUILDDIR [EXPECTED_TOOLCHAIN]
#
# build/ is shared by the native build, the cross build and the container (which
# sees this tree as /src). A cmake cache is tied to the absolute path and the
# toolchain it was generated with, so drop one left by another route and say
# why, rather than letting cmake fail with a message about a moved directory.
drop_foreign_cache()
{
    local build=$1 want=${2:-}
    local cache="$build/CMakeCache.txt"
    [ -f "$cache" ] || return 0

    local dir tc here why=""
    dir=$(sed -n 's/^CMAKE_CACHEFILE_DIR:INTERNAL=//p' "$cache" | tr -d '\r')
    tc=$(sed -n 's/^CMAKE_TOOLCHAIN_FILE:FILEPATH=//p' "$cache" | tr -d '\r')

    # cmake writes Windows paths as c:/..., which is what cygpath -m produces
    # apart from the drive letter's case -- and there, case does not distinguish
    # two paths. Compare folded, or an unchanged directory reads as a moved one
    # and every build starts from scratch.
    if command -v cygpath >/dev/null 2>&1; then
        here=$(cygpath -m "$build" | tr 'A-Z' 'a-z')
        dir=$(printf '%s' "$dir" | tr 'A-Z' 'a-z')
    else
        here=$build
    fi

    if [ -n "$dir" ] && [ "$dir" != "$here" ]; then
        why="it was generated for $dir"
    elif [ "$tc" != "$want" ]; then
        why="it was generated with toolchain '${tc:-none}', wanted '${want:-none}'"
    fi
    [ -n "$why" ] || return 0

    echo "dropping the cmake cache in $build: $why"
    rm -rf "$build"
}

# container_run REPO COMMAND...
#
# Runs COMMAND in the Fedora image with REPO mounted at /src, building the image
# first if it is not there yet.
#
# W32DI_IN_CONTAINER tells the script inside where it is, so one that falls back
# to the container cannot recurse forever when the image is missing something.
container_run()
{
    local repo=${1:?usage: container_run REPO COMMAND...}
    shift

    command -v podman >/dev/null 2>&1 || {
        echo "error: podman not found, and this needs a container." >&2
        echo "       On Fedora you can install the toolchain instead:" >&2
        echo "         sudo bash tools/build-env.sh install" >&2
        return 1
    }
    if ! podman image exists "$CROSS_IMAGE"; then
        echo "building $CROSS_IMAGE (one time)..." >&2
        podman build -t "$CROSS_IMAGE" --build-arg BASE="$CROSS_BASE_IMAGE" \
            -f "$repo/tools/Containerfile.build" "$repo"
    fi
    # label=disable: on an SELinux host (Fedora, RHEL) the bind-mounted repo
    # is otherwise unreadable in the container. The ${a[@]+...} form: bash
    # before 4.4 (macOS's) treats an empty array as unset under set -u.
    podman run --rm -v "$repo:/src" -w /src --security-opt label=disable \
        -e W32DI_IN_CONTAINER=1 \
        ${CONTAINER_ENV[@]+"${CONTAINER_ENV[@]}"} \
        "$CROSS_IMAGE" "$@"
}

# ------------------------------------------------------------------- build ---

# Steps the native and the cross build share.

# need_msys2_tools TOOL...
#
# Assert the native Windows toolchain is on the path, and say how to get it if
# it is not.
need_msys2_tools()
{
    local tool
    for tool in "$@"; do
        command -v "$tool" >/dev/null 2>&1 || {
            echo "error: $tool not found. In the MSYS2 UCRT64 shell, install the" >&2
            echo "       toolchain with:" >&2
            echo "         pacman -S --needed \$(bash tools/build-env.sh packages-msys2)" >&2
            exit 1
        }
    done
}

# harness_run REPO NAME [clean]
#
# Build and run the standalone test harness in tools/NAME/. Each harness
# compiles the real source file from src/ (so the code under test is the
# shipped code, not a copy) and drives it against a plain file -- no card, no
# VM, no UAC prompt. MSYS2 UCRT64 only: it is Win32 code.
#
# Each gets its own build-NAME/ directory, being a separate cmake project that
# would otherwise fight the application's cache, and runs from there because it
# writes scratch files into the working directory.
harness_run()
{
    local repo=${1:?usage: harness_run REPO NAME [clean]}
    local name=${2:?}
    local build="$repo/build-$name"
    case "${3:-}" in
        clean) rm -rf "$build" ;;
        "")    ;;
        *)     echo "usage: ${0##*/} [clean]" >&2; exit 2 ;;
    esac
    need_msys2_tools cmake ninja g++
    if [ ! -f "$build/CMakeCache.txt" ]; then
        cmake -S "$repo/tools/$name" -B "$build" -G Ninja
    fi
    cmake --build "$build"
    cd "$build"
    "./$name.exe"
}

# build_parse_args [test] [clean]
#
# Sets BUILD_MODE, BUILD_CLEAN and BUILD_EXTRA. Returns 2 on an unknown word, so
# the caller can print its own usage. The -D flag is always stated rather than
# left to whatever the cache happens to hold, so switching between a normal and
# a test build is just a matter of the argument.
build_parse_args()
{
    BUILD_MODE=""
    BUILD_CLEAN=0
    local arg
    for arg in "$@"; do
        case "$arg" in
            test)  BUILD_MODE=test ;;
            clean) BUILD_CLEAN=1 ;;
            *)     return 2 ;;
        esac
    done
    # A test build asks for no elevation: no UAC prompt on every launch, and no
    # ability to open a device either. For working on the interface, never for
    # shipping -- both deploy scripts refuse to package one.
    if [ "$BUILD_MODE" = test ]; then
        BUILD_EXTRA=(-DTEST_NO_ADMIN=ON)
    else
        BUILD_EXTRA=(-DTEST_NO_ADMIN=OFF)
    fi
    return 0
}

# build_prepare BUILDDIR [TOOLCHAIN]
#
# Honour "clean", then drop_foreign_cache.
build_prepare()
{
    local build=${1:?usage: build_prepare BUILDDIR [TOOLCHAIN]}
    local toolchain=${2-}
    if [ "${BUILD_CLEAN:-0}" = 1 ]; then
        rm -rf "$build"
    fi
    drop_foreign_cache "$build" "$toolchain"
}

# native_configure SRCDIR BUILDDIR [cmake args...]
#
# The first configure with the host's own compiler; cross_configure's
# counterpart.
native_configure()
{
    local src=${1:?usage: native_configure SRCDIR BUILDDIR [cmake args...]}
    local build=${2:?}
    shift 2
    cmake -S "$src" -B "$build" -G Ninja -DCMAKE_BUILD_TYPE=Release "$@"
}

# build_run SRCDIR BUILDDIR CONFIGURE
#
# Configure if there is no cache yet, then build. Configuring costs far more
# than an incremental build, and ninja re-runs cmake by itself when
# CMakeLists.txt changes. BUILD_EXTRA is passed either way, so a switch between
# a normal and a test build reconfigures on its own.
build_run()
{
    local src=${1:?usage: build_run SRCDIR BUILDDIR CONFIGURE}
    local build=${2:?}
    local configure=${3:?}
    if [ ! -f "$build/CMakeCache.txt" ]; then
        "$configure" "$src" "$build" "${BUILD_EXTRA[@]}"
    else
        cmake -S "$src" -B "$build" "${BUILD_EXTRA[@]}" >/dev/null
    fi
    cmake --build "$build"
}

# build_report BUILDDIR PACKAGE_HINT
#
# The closing lines. A test build says what it cannot do instead of suggesting
# how to package it, because packaging one is exactly what must not happen.
build_report()
{
    local build=${1:?usage: build_report BUILDDIR PACKAGE_HINT}
    local hint=${2:?}
    echo
    echo "built $build/WinDiskImager.exe"
    if [ "${BUILD_MODE:-}" = test ]; then
        echo "this build asks for no elevation and CANNOT write to a device"
    else
        echo "package it with: $hint"
    fi
}

# deploy_resolve_closure OBJDUMP BINDIR DIST
#
# Copy into DIST every DLL its binaries import that is in BINDIR and not yet in
# DIST, repeating while a pass finds binaries not yet read (a copied DLL has
# imports of its own). Windows' own DLLs are skipped by not being in BINDIR.
# deploy_write_licenses pacman|rpm DIST BINDIR PLUGINDIR TRDIR
#
# Ship the licence of every third-party file in DIST, taken from the package
# manager that installed it: each file is traced back to its package, the
# package's licence files are copied into DIST/licenses/<package>/, and an
# entry naming the package, version, licence and source is appended to
# DIST/THIRD-PARTY-NOTICES.txt. BINDIR, PLUGINDIR and TRDIR are where the
# deploy took the DLLs, Qt plugins and Qt translations from. Only the lookups
# differ between the two backends, so both packages come out the same shape.
# Fails if a file has no owning package or a package ships no licence file --
# a package missing a licence must not be released.
deploy_write_licenses()
{
    local backend=${1:?usage: deploy_write_licenses pacman|rpm DIST BINDIR[:BINDIR...] PLUGINDIR TRDIR}
    local dist=${2:?} plugindir=${4:?} trdir=${5:?}
    local notices="$dist/THIRD-PARTY-NOTICES.txt"
    local rel src pkg line d
    declare -A files_of=() owner_of=() ver_of=() lic_of=() list_of=() built_of=()
    local -a rels=() srcs=() bindirs
    # In the order deploy_resolve_closure searched them, so each DLL is traced
    # to the copy that was shipped.
    IFS=: read -r -a bindirs <<< "${3:?}"

    while IFS= read -r rel; do
        case "$rel" in
            translations/*) src="$trdir/${rel#translations/}" ;;
            */*)            src="$plugindir/$rel" ;;
            *)
                src="${bindirs[0]}/$rel"
                for d in "${bindirs[@]}"; do
                    if [ -f "$d/$rel" ]; then src="$d/$rel"; break; fi
                done
                ;;
        esac
        rels+=("$rel")
        srcs+=("$src")
    done < <(cd "$dist" && find . \( -name '*.dll' -o -name '*.qm' \) | sed 's|^\./||' | sort)

    # pacman takes half a second to start on MSYS2, so it is asked everything
    # in three calls -- owners, then versions and licences, then file lists --
    # rather than once per file and several times per package. rpm is fast
    # enough to ask as it goes.
    if [ "$backend" = pacman ] && [ "${#srcs[@]}" -gt 0 ]; then
        while IFS= read -r line; do
            case "$line" in
                *" is owned by "*)
                    pkg=${line#* is owned by }
                    owner_of[${line%% is owned by *}]=${pkg%% *} ;;
            esac
        done < <(pacman -Qo "${srcs[@]}" 2>/dev/null || true)
    fi

    local i
    for i in "${!rels[@]}"; do
        rel=${rels[$i]}
        src=${srcs[$i]}
        if [ -n "${CROSS_XZ_PREFIX:-}" ] && [ "${src#"$CROSS_XZ_PREFIX"/}" != "$src" ]; then
            # Built from xz's own release by cross_build_xz; no package owns it.
            pkg=xz
            built_of[$pkg]=1
        else
            case "$backend" in
                pacman) pkg=${owner_of[$src]:-} ;;
                rpm)    pkg=$(rpm -qf --qf '%{NAME}\n' "$src" 2>/dev/null || true) ;;
            esac
        fi
        if [ -z "$pkg" ]; then
            echo "error: no package owns $src, so its licence is unknown." >&2
            return 1
        fi
        files_of[$pkg]="${files_of[$pkg]:+${files_of[$pkg]}, }$rel"
    done

    if [ "$backend" = pacman ] && [ "${#files_of[@]}" -gt 0 ]; then
        local name=""
        while IFS= read -r line; do
            case "$line" in
                "Name "*)     name=${line#*: } ;;
                "Version "*)  ver_of[$name]=${line#*: } ;;
                "Licenses "*) lic_of[$name]=${line#*: } ;;
            esac
        done < <(pacman -Qi "${!files_of[@]}")
        # Qt alone lists thousands of files; keep only what could be a
        # licence or README, which the copy loop below narrows further.
        while IFS= read -r line; do
            pkg=${line%% *}
            list_of[$pkg]+="${line#* }"$'\n'
        done < <(pacman -Ql "${!files_of[@]}" |
                     grep -iE '/share/licenses/.+[^/]$|/(LICENSE|LICENCE|COPYING)[^/]*$|/README[^/]*$' || true)
    fi

    local mgr="MSYS2 (UCRT64)"
    [ "$backend" = rpm ] && mgr="Fedora MinGW"
    {
        printf '\n\nLibraries shipped with this build\n'
        printf '=================================\n\n'
        printf 'Every DLL, Qt plugin and Qt translation in this folder comes from the\n'
        printf '%s package named below. Each package'"'"'s licence files are in\n' "$mgr"
        printf 'licenses/<package>/, and its source is at the address given. The\n'
        printf 'program'"'"'s own source is at https://github.com/peacepenguin/windiskimager.\n'
        if [ "${#built_of[@]}" -gt 0 ]; then
            printf '\nThe exceptions, marked "built from source", were compiled for this build\n'
            printf 'from the project'"'"'s own release, given as their source.\n'
        fi
        printf '\nQt contains third-party code of its own, listed with its copyright notices\n'
        printf 'in "Third-Party Code Used in Qt": https://doc.qt.io/qt-6/licenses-used-in-qt.html\n'
    } >> "$notices"

    local version licence url lf dest n
    for pkg in $(printf '%s\n' "${!files_of[@]}" | sort); do
        local via=$backend
        [ -n "${built_of[$pkg]:-}" ] && via=built
        case "$via" in
            built)
                # liblzma is 0BSD; COPYING, shipped with it, says so.
                version="$CROSS_XZ_VERSION (built from source)"
                licence="0BSD"
                url=$CROSS_XZ_URL
                ;;
            pacman)
                version=${ver_of[$pkg]:-}
                licence=${lic_of[$pkg]:-}
                url="https://packages.msys2.org/package/$pkg"
                ;;
            rpm)
                version=$(rpm -q --qf '%{VERSION}-%{RELEASE}' "$pkg")
                licence=$(rpm -q --qf '%{LICENSE}' "$pkg")
                url="https://src.fedoraproject.org/rpms/$(rpm -q --qf '%{SOURCERPM}' "$pkg" | sed 's/-[^-]*-[^-]*\.src\.rpm$//')"
                ;;
        esac
        # Licence files first. Failing those, the README: a public-domain
        # project has no licence file and says so there (win-iconv does).
        # Then, always, any text kept in tools/licenses/<package>/: for a
        # package that ships none (Fedora's mingw64-zlib), or whose licence
        # file only points at one it leaves out (Fedora's icu, pcre2, ...).
        local kind want rest
        local -A made=()
        n=0
        for kind in licence readme repo; do
            [ "$kind" = readme ] && [ "$n" -gt 0 ] && continue
            # Forks are slow on MSYS2, so the loop matches and names files
            # with bash itself; only cp runs per file.
            case "$kind" in
                licence) want='/share/licenses/.+[^/]$|/(LICENSE|LICENCE|COPYING)[^/]*$' ;;
                readme)  want='/share/doc/.*/README[^/]*$' ;;
                repo)    want='' ;;
            esac
            while IFS= read -r lf; do
                [ -n "$lf" ] || continue
                if [ "$via" = pacman ] && [ -n "$want" ]; then
                    shopt -s nocasematch
                    if [[ $lf =~ $want ]]; then rest=; else rest=no; fi
                    shopt -u nocasematch
                    [ -z "$rest" ] || continue
                fi
                [ -f "$lf" ] || continue
                case "$lf" in
                    */share/licenses/*/*)
                        rest=${lf##*/share/licenses/}
                        dest="$dist/licenses/$pkg/${rest#*/}" ;;
                    *)  dest="$dist/licenses/$pkg/${lf##*/}" ;;
                esac
                if [ -z "${made[${dest%/*}]:-}" ]; then
                    mkdir -p "${dest%/*}"
                    made[${dest%/*}]=1
                fi
                cp "$lf" "$dest"
                n=$((n + 1))
            done < <(case "$via:$kind" in
                         # Copied from the source tree by cross_build_xz.
                         built:licence)  find "$CROSS_XZ_PREFIX/share/licenses/$pkg" -type f 2>/dev/null || true ;;
                         built:readme)   ;;
                         pacman:licence|pacman:readme) printf '%s' "${list_of[$pkg]:-}" ;;
                         # Some packages file their licence as %doc, not %license.
                         rpm:licence)    { rpm -qL "$pkg"; rpm -qd "$pkg" | grep -iE '/(LICENSE|LICENCE|COPYING)[^/]*$'; } || true ;;
                         rpm:readme)     rpm -qd "$pkg" | grep -iE '/README[^/]*$' || true ;;
                         *:repo)         find "$(dirname "${BASH_SOURCE[0]}")/licenses/$pkg" -type f 2>/dev/null || true ;;
                     esac)
        done
        if [ "$n" -eq 0 ]; then
            echo "error: package $pkg ships no licence file or README to include." >&2
            [ "$backend" = rpm ] && echo "       If rpm -qd $pkg lists one, it was installed without docs (tsflags=nodocs)." >&2
            return 1
        fi
        {
            printf '\n%s %s\n' "$pkg" "$version"
            printf '  Licence: %s\n' "$licence"
            printf '  Files:   %s\n' "${files_of[$pkg]}"
            printf '  Source:  %s\n' "$url"
        } >> "$notices"
    done
}

# deploy_check_dist DIR
#
# Fail unless DIR holds what the exe cannot start or draw its icons without.
# Plugin copies and objdump are allowed to fail quietly above, so a broken
# package would otherwise be zipped and shipped.
deploy_check_dist()
{
    local dist=${1:?usage: deploy_check_dist DIR}
    local f missing=0
    for f in WinDiskImager.exe Qt6Core.dll Qt6Gui.dll Qt6Widgets.dll Qt6Svg.dll \
             platforms/qwindows.dll iconengines/qsvgicon.dll imageformats/qsvg.dll; do
        if [ ! -f "$dist/$f" ]; then
            echo "error: $dist/$f is missing from the package." >&2
            missing=1
        fi
    done
    # Licences: the program's, and the LGPL-3.0 Qt 6 is under -- found by
    # content, since the two package managers name the file differently.
    for f in License.txt GPL-2 THIRD-PARTY-NOTICES.txt; do
        if [ ! -f "$dist/$f" ]; then
            echo "error: $dist/$f is missing from the package." >&2
            missing=1
        fi
    done
    if ! grep -rlqs "GNU LESSER GENERAL PUBLIC LICENSE" "$dist/licenses" \
         || ! grep -rlqs "Version 3, 29 June 2007" "$dist/licenses"; then
        echo "error: no LGPL-3.0 text under $dist/licenses; Qt 6 requires it." >&2
        missing=1
    fi
    return $missing
}

# Both deploy scripts use this so what a finished folder contains cannot drift
# between them, whatever else differs in how they gather the Qt payload.
#
# objdump because it comes with the compiler on both hosts; ntldd is a Windows
# executable and cannot run in a cross build. One objdump call per pass, over
# only the binaries not yet read: starting objdump is nearly all the cost.
deploy_resolve_closure()
{
    local objdump=${1:?usage: deploy_resolve_closure OBJDUMP BINDIR[:BINDIR...] DIST}
    local dist=${3:?}
    local f dll d
    local -a bins pending dirs
    local -A seen=()
    # Searched in order, so a DLL built from source (see cross_build_xz) wins
    # over a packaged one of the same name.
    IFS=: read -r -a dirs <<< "${2:?}"

    while :; do
        mapfile -t bins < <(find "$dist" \( -name '*.exe' -o -name '*.dll' \))
        pending=()
        for f in "${bins[@]}"; do
            [ -n "${seen[$f]:-}" ] || pending+=("$f")
        done
        [ ${#pending[@]} -eq 0 ] && break
        for f in "${pending[@]}"; do seen[$f]=1; done

        # "|| true" because this runs under pipefail: one binary objdump cannot
        # read would otherwise abort the caller, and it is handed dozens.
        { "$objdump" -p "${pending[@]}" 2>/dev/null || true; } \
            | awk '/DLL Name:/ {print $3}' | sort -u | while read -r dll; do
            [ -f "$dist/$dll" ] && continue
            for d in "${dirs[@]}"; do
                if [ -f "$d/$dll" ]; then
                    cp "$d/$dll" "$dist/"
                    break
                fi
            done
        done
    done
}

# ------------------------------------------------------------------ command ---

# Only when run, not when sourced.
if [ "${BASH_SOURCE[0]}" = "$0" ]; then
    set -euo pipefail
    cmd=${1:-}
    shift || true
    case "$cmd" in
        install)         cross_install ;;
        check)           cross_check ;;
        configure)       cross_configure "$@" ;;
        packages)        cross_packages ;;
        packages-msys2)  echo $MSYS2_PACKAGES ;;
        print)
            case "${1:-}" in
                BASE_IMAGE) echo "$CROSS_BASE_IMAGE" ;;
                IMAGE)     echo "$CROSS_IMAGE" ;;
                TOOLCHAIN) echo "$CROSS_TOOLCHAIN" ;;
                LRELEASE)  echo "$CROSS_LRELEASE" ;;
                LUPDATE)   echo "$CROSS_LUPDATE" ;;
                SYSROOT)   echo "$CROSS_SYSROOT" ;;
                *) echo "print: unknown name '${1:-}'" >&2; exit 2 ;;
            esac
            ;;
        *)
            sed -n '2,25p' "$0" | sed 's/^# \{0,1\}//'
            [ -n "$cmd" ] && exit 2
            exit 0
            ;;
    esac
fi
