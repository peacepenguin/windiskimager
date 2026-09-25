#!/usr/bin/env bash
# Cross-compile a win64 binary on a Fedora host, using the toolchain installed
# there directly. No container.
#
#   tools/build-cross.sh              # configure (if needed) and build
#   tools/build-cross.sh clean        # drop the build dir first
#   tools/build-cross.sh test         # a binary that asks for no elevation
#   tools/build-cross.sh test clean
#
# Install the toolchain once with:
#   sudo bash tools/build-env.sh install
#
# CI and tools/build-container.sh run this same script inside the Fedora image.
# Every route builds into build/ (see drop_foreign_cache in tools/build-env.sh);
# set BUILD_DIR=... for a build kept aside.
#
# Copyright (C) 2026 peacepenguin, GPL-2.0-or-later.
set -euo pipefail

REPO=$(cd "$(dirname "$0")/.." && pwd)
. "$REPO/tools/build-env.sh"

build_parse_args "$@" || { echo "usage: ${0##*/} [test] [clean]" >&2; exit 2; }
build=${BUILD_DIR:-$REPO/build}

command -v cmake >/dev/null 2>&1 || { echo "error: cmake not found" >&2; exit 1; }
if ! cross_check; then
    echo "error: the MinGW cross toolchain is not installed, or not where the" >&2
    echo "       build expects it. On Fedora:" >&2
    echo "         sudo bash tools/build-env.sh install" >&2
    echo "       On any other distribution there are no MinGW Qt6 packages," >&2
    echo "       so use the container instead: tools/build-container.sh" >&2
    exit 1
fi

build_prepare "$build" "$CROSS_TOOLCHAIN"
build_run "$REPO/src" "$build" cross_configure

# A host compiler picked up by mistake produces an ELF binary that looks like a
# successful build. Checked here so every route is covered, not only CI.
if ! file "$build/WinDiskImager.exe" | grep -q 'PE32+'; then
    echo "error: $build/WinDiskImager.exe is not a win64 PE binary:" >&2
    file "$build/WinDiskImager.exe" >&2
    exit 1
fi
file "$build/WinDiskImager.exe"

if [ -n "${W32DI_IN_CONTAINER:-}" ]; then
    # Run by tools/build-container.sh: the host has no toolchain to deploy with.
    # The ARM64 image names its own wrapper (tools/Containerfile.arm64).
    build_report "$build" "${W32DI_DEPLOY_HINT:-tools/deploy-container.sh}"
elif [ "$build" = "$REPO/build" ]; then
    build_report "$build" "tools/deploy-cross.sh"
else
    # BUILD_DIR was overridden, so the defaults would not find this build.
    build_report "$build" "tools/deploy-cross.sh $build dist"
fi
