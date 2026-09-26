#!/usr/bin/env bash
# Cross-compile a Windows binary on a Fedora host, against the toolkit
# installed there. No container.
#
#   tools/build-cross.sh x64              # configure (if needed) and build
#   tools/build-cross.sh arm64            # the same, for Windows on ARM
#   tools/build-cross.sh ARCH clean       # drop the build dir first
#   tools/build-cross.sh ARCH test        # a binary that asks for no elevation
#
# Install the target's toolkit once with:
#   sudo tools/toolkit-env.sh ARCH install
#
# CI and tools/build-container.sh run this same script inside the toolkit's
# image. Each target builds into build-ARCH/ (see drop_foreign_cache in
# tools/build-env.sh); set BUILD_DIR=... for a build kept aside.
#
# Copyright (C) 2026 peacepenguin, GPL-2.0-or-later.
set -euo pipefail

REPO=$(cd "$(dirname "$0")/.." && pwd)
TOOLKIT_ARCH=${1:-}
shift || true
. "$REPO/tools/toolkit-env.sh" || exit 2
toolkit_use

build_parse_args "$@" || { echo "usage: ${0##*/} x64|arm64 [test] [clean]" >&2; exit 2; }
build=${BUILD_DIR:-$REPO/build-$TOOLKIT_ARCH}

command -v cmake >/dev/null 2>&1 || { echo "error: cmake not found" >&2; exit 1; }
if ! toolkit_check >/dev/null 2>&1; then
    toolkit_check || true
    echo "error: the $TOOLKIT_ARCH toolkit is not installed, or not where the build" >&2
    echo "       expects it. On Fedora:" >&2
    echo "         sudo tools/toolkit-env.sh $TOOLKIT_ARCH install" >&2
    echo "       Anywhere podman runs: tools/build-container.sh $TOOLKIT_ARCH" >&2
    exit 1
fi

build_prepare "$build" "$CROSS_TOOLCHAIN"
build_run "$REPO/src" "$build" cross_configure

# A host compiler picked up by mistake produces an ELF binary that looks like a
# successful build, and a wrong target one for the other architecture. Checked
# here so every route is covered, not only CI.
if ! file "$build/WinDiskImager.exe" | grep -Eq "PE32\+.*($TOOLKIT_FILE_ARCH)"; then
    echo "error: $build/WinDiskImager.exe is not a Windows $TOOLKIT_ARCH binary:" >&2
    file "$build/WinDiskImager.exe" >&2
    exit 1
fi
file "$build/WinDiskImager.exe"

if [ -n "${W32DI_IN_CONTAINER:-}" ]; then
    # Run by tools/build-container.sh: the host has no toolkit to deploy with.
    build_report "$build" "tools/deploy-container.sh $TOOLKIT_ARCH"
elif [ "$build" = "$REPO/build-$TOOLKIT_ARCH" ]; then
    build_report "$build" "tools/deploy-cross.sh $TOOLKIT_ARCH"
else
    # BUILD_DIR was overridden, so the defaults would not find this build.
    build_report "$build" "tools/deploy-cross.sh $TOOLKIT_ARCH $build dist-$TOOLKIT_ARCH"
fi
