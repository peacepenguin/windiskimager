#!/usr/bin/env bash
# Cross-compile a Windows-on-ARM64 binary in the ARM64 Fedora container.
#
#   tools/build-container-arm64.sh              # configure (if needed) and build
#   tools/build-container-arm64.sh clean        # drop the build dir first
#   tools/build-container-arm64.sh test         # a binary that asks for no elevation
#
# The ARM64 counterpart of tools/build-container.sh: the same
# tools/build-cross.sh, run in the image tools/Containerfile.arm64 builds
# (on first use; it takes a while). Output lands in build-arm64/, apart from
# the x64 build's build/.
#
# Copyright (C) 2026 peacepenguin, GPL-2.0-or-later.
set -euo pipefail

REPO=$(cd "$(dirname "$0")/.." && pwd)
. "$REPO/tools/woa64-env.sh"

CONTAINER_IMAGE=$WOA64_IMAGE
CONTAINER_BASE=$WOA64_BASE_IMAGE
CONTAINER_FILE=tools/Containerfile.arm64
CONTAINER_STALE=/usr/local/lib/woa64-env.sh
# Checked for updates first (container_stale); the deploy wrapper never is.
CONTAINER_REFRESH=1 container_run "$REPO" env BUILD_DIR=/src/build-arm64 /src/tools/build-cross.sh "$@"
