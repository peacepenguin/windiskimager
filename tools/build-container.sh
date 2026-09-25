#!/usr/bin/env bash
# Cross-compile a win64 binary in the Fedora container, the way CI does.
#
#   tools/build-container.sh              # configure (if needed) and build
#   tools/build-container.sh clean        # drop the build dir first
#   tools/build-container.sh test         # a binary that asks for no elevation
#
# Runs tools/build-cross.sh, with all its arguments, inside the image (built on
# first use). For a host that is not Fedora or would rather not install the
# toolchain. Output lands in build/; see drop_foreign_cache in tools/build-env.sh.
#
# Copyright (C) 2026 peacepenguin, GPL-2.0-or-later.
set -euo pipefail

REPO=$(cd "$(dirname "$0")/.." && pwd)
. "$REPO/tools/build-env.sh"

# Checked for updates first (container_stale); the deploy wrapper never is.
CONTAINER_REFRESH=1 container_run "$REPO" env BUILD_DIR=/src/build /src/tools/build-cross.sh "$@"
