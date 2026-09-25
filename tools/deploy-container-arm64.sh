#!/usr/bin/env bash
# Package a tools/build-container-arm64.sh build into dist-arm64/, in the same
# container.
#
#   tools/deploy-container-arm64.sh       # build-arm64/ -> dist-arm64/
#
# The ARM64 counterpart of tools/deploy-container.sh: the same
# tools/deploy-cross.sh, run in the ARM64 image, where llvm-objdump, llvm-strip
# and the toolkit's licence manifest are.
#
# Copyright (C) 2026 peacepenguin, GPL-2.0-or-later.
set -euo pipefail

REPO=$(cd "$(dirname "$0")/.." && pwd)
. "$REPO/tools/woa64-env.sh"

CONTAINER_IMAGE=$WOA64_IMAGE
CONTAINER_BASE=$WOA64_BASE_IMAGE
CONTAINER_FILE=tools/Containerfile.arm64
container_run "$REPO" /src/tools/deploy-cross.sh /src/build-arm64 /src/dist-arm64
