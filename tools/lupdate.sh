#!/usr/bin/env bash
# Refresh src/lang/*.ts from the current sources.
#
#   tools/lupdate.sh           # update every .ts in src/lang
#   tools/lupdate.sh de fr     # only those languages
#
# Uses the host's Qt 6 lupdate (see lupdate_path in tools/build-env.sh), or
# runs itself in the Fedora container when there is none.
#
# Unlike the build, this rewrites files that are tracked in git. New strings
# arrive untranslated, and strings that no longer appear in the source are
# marked "vanished" rather than deleted, so translators keep their history.
# Review the diff before committing.
#
# Copyright (C) 2026 peacepenguin, GPL-2.0-or-later.
set -euo pipefail

REPO=$(cd "$(dirname "$0")/.." && pwd)
. "$REPO/tools/build-env.sh"

# Named languages, or all of them. Checked here so a typo fails immediately
# rather than after a container has started.
for lang in "$@"; do
    [ -f "$REPO/src/lang/diskimager_$lang.ts" ] \
        || { echo "error: no src/lang/diskimager_$lang.ts" >&2; exit 1; }
done

if ! LUPDATE=$(lupdate_path); then
    # Inside the container this means the image is broken; going round again
    # would only loop.
    if [ -n "${W32DI_IN_CONTAINER:-}" ]; then
        echo "error: no Qt 6 lupdate inside the container image." >&2
        echo "       Rebuild it: podman build -t $CROSS_IMAGE --build-arg BASE=$CROSS_BASE_IMAGE -f tools/Containerfile.build ." >&2
        exit 1
    fi
    if ! command -v podman >/dev/null 2>&1; then
        echo "error: no Qt 6 lupdate on this host." >&2
        echo "       MSYS2 UCRT64:  pacman -S --needed mingw-w64-ucrt-x86_64-qt6-tools" >&2
        echo "       Fedora:        sudo bash tools/build-env.sh install" >&2
        echo "       Anywhere else: install podman and this will use a container." >&2
        exit 1
    fi
    container_run "$REPO" /src/tools/lupdate.sh "$@"
    exit $?
fi

TSFILES=()
if [ "$#" -gt 0 ]; then
    for lang in "$@"; do
        TSFILES+=("lang/diskimager_$lang.ts")
    done
else
    for ts in "$REPO"/src/lang/*.ts; do
        TSFILES+=("lang/$(basename "$ts")")
    done
fi

# Run from src/ so the source files are found by the plain names below.
cd "$REPO/src"

# -locations none: line numbers do not affect the .qm, and recording them turns
# every added string into a large diff in each .ts file. Translations match by
# context and source text, so nothing is lost. For a local look at the
# references:  lupdate -locations relative *.cpp *.h *.ui -ts lang/foo.ts
"$LUPDATE" -locations none *.cpp *.h *.ui -ts "${TSFILES[@]}"
