#!/usr/bin/env bash
# Package a tools/build-container.sh build into dist/, in the same container.
#
#   tools/deploy-container.sh             # build/ -> dist/
#
# Runs tools/deploy-cross.sh inside the image, where its toolchain and rpm
# live; the host needs only podman. dist/ lands in the repo through the same
# bind mount the build uses.
#
# Copyright (C) 2026 peacepenguin, GPL-2.0-or-later.
set -euo pipefail

REPO=$(cd "$(dirname "$0")/.." && pwd)
. "$REPO/tools/build-env.sh"

container_run "$REPO" /src/tools/deploy-cross.sh /src/build /src/dist
