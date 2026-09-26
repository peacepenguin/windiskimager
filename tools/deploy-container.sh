#!/usr/bin/env bash
# Package a tools/build-container.sh build, in the same container.
#
#   tools/deploy-container.sh x64         # build-x64/ -> dist-x64/
#   tools/deploy-container.sh arm64       # build-arm64/ -> dist-arm64/
#
# Runs tools/deploy-cross.sh inside the toolkit image, where the target's
# sysroot and licence manifest are; the host needs only podman. The folder
# lands in the repo through the same bind mount the build uses. The image is
# not checked for updates here, so a package is always made from the image
# its build used.
#
# Copyright (C) 2026 peacepenguin, GPL-2.0-or-later.
set -euo pipefail

REPO=$(cd "$(dirname "$0")/.." && pwd)
TOOLKIT_ARCH=${1:-}
shift || true
. "$REPO/tools/toolkit-env.sh" || exit 2

toolkit_container_run "$REPO" /src/tools/deploy-cross.sh "$TOOLKIT_ARCH"
