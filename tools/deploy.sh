#!/usr/bin/env bash
# Package build/WinDiskImager.exe into a self-contained dist/ folder.
# Run in the MSYS2 UCRT64 shell.
#
# Copyright (C) 2026 peacepenguin, GPL-2.0-or-later.
set -euo pipefail

root="$(cd "$(dirname "$0")/.." && pwd)"
cd "$root"
. "$root/tools/build-env.sh"

# windeployqt6 brings the Qt payload; objdump resolves everything it leaves out.
# A missing objdump is silent: no runtime DLLs get copied and the folder is
# reported ready around an executable that cannot start.
need_msys2_tools windeployqt6 objdump

# Qt's DLLs and the MinGW runtime sit beside the tools, so the prefix is asked
# for rather than written down.
MSYS2_BIN=$(dirname "$(command -v objdump)")

[ -f build/WinDiskImager.exe ] || {
    echo "error: no build/WinDiskImager.exe -- build it first" >&2
    exit 1
}

# A TEST_NO_ADMIN build cannot write to a device and must never ship.
if grep -aq 'level="asInvoker"' build/WinDiskImager.exe; then
    echo "error: build/WinDiskImager.exe was built with TEST_NO_ADMIN=ON and" >&2
    echo "       cannot write to a device. Reconfigure without it before packaging." >&2
    exit 1
fi

# Empty dist rather than deleting it: on Windows an open Explorer window or a
# shell sitting in the directory locks the directory node itself, while its
# contents still delete fine.
mkdir -p dist
rm -rf dist/* dist/.[!.]* 2>/dev/null || true
cp build/WinDiskImager.exe dist/
cp Changelog.txt README.md License.txt THIRD-PARTY-NOTICES.txt GPL-2 dist/

# Languages the app itself ships translations for; Qt's own translations are
# trimmed to match instead of shipping all ~40 of them. Read from CMakeLists so
# this list cannot drift from the one the build compiles.
LANGUAGES=$(sed -n 's/^set(LANGUAGES \(.*\))$/\1/p' src/CMakeLists.txt)
[ -n "$LANGUAGES" ] || { echo "error: no LANGUAGES in src/CMakeLists.txt" >&2; exit 1; }

# Qt DLLs, plugins and translations. The app is 2D-only, so skip the
# software-OpenGL and D3D compiler payloads windeployqt adds by default.
(cd dist && windeployqt6 --release \
    --no-opengl-sw \
    --no-system-d3d-compiler \
    WinDiskImager.exe)

# Qt6Network itself has to stay: generic/qtuiotouchplugin.dll imports it, so
# the dependency scan below would copy it straight back. Its plugins are loaded
# by name, not imported, so deleting them sticks. The app opens no sockets.
rm -rf dist/tls dist/networkinformation

# ${f##*/} rather than basename: this loop runs hundreds of times and a process
# per iteration cost seconds.
for f in dist/translations/*.qm; do
    keep=""
    name=${f##*/}
    for l in $LANGUAGES; do
        case "$name" in *_"$l".qm) keep=1;; esac
    done
    [ -n "$keep" ] || rm -f "$f"
done

# windeployqt chooses what to write here and under what names, so a change on
# its side or to the trim above could silently leave every Qt string in English.
# Emptiness is the test, not one file per language: Qt has no translation for
# some of the app's languages (ta_IN among them). deploy-cross.sh checks the same.
if [ -z "$(ls -A dist/translations 2>/dev/null)" ]; then
    echo "error: no Qt translations in dist/translations." >&2
    echo "       windeployqt writes them and the loop above trims them to" >&2
    echo "       LANGUAGES; see its output above for what it actually did." >&2
    exit 1
fi

# windeployqt does not pull in the MinGW runtime or Qt's third-party
# dependencies on MSYS2; see deploy_resolve_closure in tools/build-env.sh.
deploy_resolve_closure objdump "$MSYS2_BIN" dist

deploy_write_licenses pacman dist "$MSYS2_BIN" \
    "$(dirname "$MSYS2_BIN")/share/qt6/plugins" "$(dirname "$MSYS2_BIN")/share/qt6/translations"

deploy_check_dist dist

echo "dist/ is ready ($(du -sh dist | cut -f1))"
