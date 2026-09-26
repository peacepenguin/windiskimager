#!/usr/bin/env bash
# Build llvm-mingw-qt6 and publish it, when anything it is built from has
# changed.
#
#   tools/toolkit-publish.sh             # only if the published one has fallen behind
#   tools/toolkit-publish.sh --force     # regardless
#
# .github/workflows/toolkit.yml runs this daily and whenever the toolkit's
# files change. By hand it needs "podman login" to the registry first.
#
# The published image is TOOLKIT_PUBLISHED (tools/toolkit-env.sh), and
# "fallen behind" is what the container build means by it (container_stale):
# a newer Fedora release, or a newer source RPM or llvm-mingw for either
# target. A new image is built from scratch, then used to build and package
# WinDiskImager for both targets -- a toolkit that cannot build the app is
# not published -- and pushed as
#
#   rN-YYYYMMDD   this build, kept for good
#   rN            the newest build for revision N: what the container build pulls
#   latest        the newest build
#
# where N is TOOLKIT_IMAGE_REVISION.
#
# Copyright (C) 2026 peacepenguin, GPL-2.0-or-later.
set -euo pipefail

REPO=$(cd "$(dirname "$0")/.." && pwd)
TOOLKIT_ARCH=all
. "$REPO/tools/toolkit-env.sh"

force=0
case "${1:-}" in
    --force) force=1 ;;
    "")      ;;
    *) echo "usage: ${0##*/} [--force]" >&2; exit 2 ;;
esac
command -v podman >/dev/null 2>&1 || { echo "error: podman not found" >&2; exit 1; }

if [ "$force" = 0 ]; then
    if podman pull -q "$TOOLKIT_PUBLISHED" >/dev/null 2>&1; then
        if ! W32DI_REFRESH=1 container_stale "$TOOLKIT_PUBLISHED" "$TOOLKIT_BASE_IMAGE" \
                bash /usr/local/lib/toolkit-env.sh all stale; then
            echo "$TOOLKIT_PUBLISHED is current; nothing to publish."
            exit 0
        fi
    else
        echo "$TOOLKIT_PUBLISHED is not published yet; building it."
    fi
fi

stamp=$(date -u +%Y%m%d)
version="r$TOOLKIT_IMAGE_REVISION-$stamp"
tag="$TOOLKIT_REGISTRY:$version"
source_repo="https://github.com/${GITHUB_REPOSITORY:-peacepenguin/windiskimager}"
commit=$(git -C "$REPO" rev-parse HEAD 2>/dev/null) || commit=unknown

# The GitHub token, if any, as a build secret (see container_run).
secret=()
[ -z "${GITHUB_TOKEN:-}" ] || secret=(--secret id=github_token,env=GITHUB_TOKEN)

# The labels are the registry's description of the image; image.source is
# also what links the package to this repository on GitHub.
podman build --pull=newer --no-cache ${secret[@]+"${secret[@]}"} \
    --label org.opencontainers.image.title=llvm-mingw-qt6 \
    --label org.opencontainers.image.description="Cross toolkits for Windows x64 and arm64 on Fedora: llvm-mingw, and Qt 6 (qtbase, qtsvg, qttranslations; no ICU, OpenSSL, D-Bus or SQL) with zlib, xz, zstd and bzip2, built from Fedora's source RPMs. Each component's licence is in /opt/win-*/sysroot/share/licenses." \
    --label org.opencontainers.image.source="$source_repo" \
    --label org.opencontainers.image.revision="$commit" \
    --label org.opencontainers.image.version="$version" \
    --label org.opencontainers.image.created="$(date -u +%Y-%m-%dT%H:%M:%SZ)" \
    --build-arg BASE="$TOOLKIT_BASE_IMAGE" \
    -t "$tag" -f "$REPO/tools/Containerfile.toolkit" "$REPO"

# What went into it, for the log.
podman run --rm "$tag" bash -c \
    'echo "Fedora $(rpm -E %fedora), Qt $(rpm -q --qf "%{VERSION}" qt6-qtbase), llvm-mingw $(cat /opt/llvm-mingw/.release)"'

# The smoke test: the app, built and packaged for both targets in the new
# image, in scratch directories inside it so nothing in the repo is touched.
CONTAINER_IMAGE=$tag CONTAINER_BASE=$TOOLKIT_BASE_IMAGE CONTAINER_FILE=tools/Containerfile.toolkit \
    container_run "$REPO" bash -c '
        set -e
        for a in x64 arm64; do
            BUILD_DIR=/tmp/build-$a /src/tools/build-cross.sh "$a"
            /src/tools/deploy-cross.sh "$a" /tmp/build-$a /tmp/dist-$a
        done'

for t in "$tag" "$TOOLKIT_PUBLISHED" "$TOOLKIT_REGISTRY:latest"; do
    [ "$t" = "$tag" ] || podman tag "$tag" "$t"
    podman push "$t"
done
echo "published $tag, as $TOOLKIT_PUBLISHED and $TOOLKIT_REGISTRY:latest"
