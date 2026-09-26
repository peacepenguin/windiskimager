#!/usr/bin/env bash
# Cross-compile a Windows binary in the toolkit's container, the way CI does.
#
#   tools/build-container.sh x64          # configure (if needed) and build
#   tools/build-container.sh arm64        # the same, for Windows on ARM
#   tools/build-container.sh ARCH clean   # drop the build dir first
#   tools/build-container.sh ARCH test    # a binary that asks for no elevation
#
# Runs tools/build-cross.sh, with all its arguments, inside the toolkit image
# (tools/Containerfile.toolkit, both targets in one), built on first use and checked
# for updates on every run after (container_stale). For a host that is not
# Fedora or would rather not install the toolkit. Output lands in build-ARCH/.
#
# Copyright (C) 2026 peacepenguin, GPL-2.0-or-later.
set -euo pipefail

REPO=$(cd "$(dirname "$0")/.." && pwd)
TOOLKIT_ARCH=${1:-}
shift || true
. "$REPO/tools/toolkit-env.sh" || exit 2

# Checked for updates first; the deploy wrapper never is.
CONTAINER_REFRESH=1 toolkit_container_run "$REPO" /src/tools/build-cross.sh "$TOOLKIT_ARCH" "$@"
