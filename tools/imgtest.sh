#!/usr/bin/env bash
# Build and run the image-decoder tests.
#
#   tools/imgtest.sh              # build (if needed) and run
#   tools/imgtest.sh clean        # drop the build dir first
#
# Exits non-zero if any check fails, so it can gate a commit or a release.
#
# Sources in tools/imgtest/ (tests src/imagesource.cpp); see harness_run in
# tools/build-env.sh.
#
# Copyright (C) 2026 peacepenguin, GPL-2.0-or-later.
set -euo pipefail

REPO=$(cd "$(dirname "$0")/.." && pwd)
. "$REPO/tools/build-env.sh"

harness_run "$REPO" imgtest "$@"
