#!/usr/bin/env bash
# Assemble a standalone Windows folder from a cross-compiled build.
#
#   tools/deploy-cross.sh x64                       # build-x64/ -> dist-x64/
#   tools/deploy-cross.sh arm64                     # build-arm64/ -> dist-arm64/
#   tools/deploy-cross.sh ARCH <build-dir> <dist-dir>
#
# windeployqt is itself a Windows binary and cannot run on the build host, so
# the Qt DLLs, plugins and translations are gathered by hand from the target's
# toolkit (tools/toolkit-env.sh) and the dependency closure is resolved with
# llvm-objdump. Every file shipped comes from that toolkit's sysroot, whose
# licence manifest says what each was built from.
#
# Copyright (C) 2026 peacepenguin, GPL-2.0-or-later.
set -euo pipefail

root="$(cd "$(dirname "$0")/.." && pwd)"
TOOLKIT_ARCH=${1:-}
shift || true
. "$root/tools/toolkit-env.sh" || exit 2
toolkit_use

build=${1:-$root/build-$TOOLKIT_ARCH}
dist=${2:-$root/dist-$TOOLKIT_ARCH}
sysroot=$CROSS_SYSROOT
objdump=$OBJDUMP
bin="$sysroot/bin"

[ -f "$build/WinDiskImager.exe" ] || {
    echo "error: no $build/WinDiskImager.exe -- build it first" >&2
    exit 1
}

# A TEST_NO_ADMIN build cannot write to a device and must never ship.
if grep -aq 'level="asInvoker"' "$build/WinDiskImager.exe"; then
    echo "error: $build/WinDiskImager.exe was built with TEST_NO_ADMIN=ON and" >&2
    echo "       cannot write to a device. Reconfigure without it before packaging." >&2
    exit 1
fi

rm -rf "$dist"
mkdir -p "$dist"
cp "$build/WinDiskImager.exe" "$dist/"
cp "$root"/Changelog.txt "$root"/README.md "$root"/License.txt "$root"/THIRD-PARTY-NOTICES.txt "$root"/GPL-2 "$dist/"

# Qt plugins. Only the ones a widgets app on Windows actually loads: no
# "generic" (the TUIO touch-table plugin, which is what needs Qt6Network), and
# no JPEG, GIF or ICO reader since the app draws nothing but SVG. deploy.sh
# leaves out the same.
qtplugins="$sysroot/lib/qt6/plugins"
[ -d "$qtplugins" ] || qtplugins="$sysroot/share/qt6/plugins"
for group in platforms styles imageformats iconengines; do
    if [ -d "$qtplugins/$group" ]; then
        mkdir -p "$dist/$group"
        cp "$qtplugins/$group"/*.dll "$dist/$group/" 2>/dev/null || true
    fi
done
# Debug variants of the plugins would double the size for nothing. Scoped to
# the plugin directories: at the top level "*d.dll" would also match innocent
# names such as libzstd.dll.
for group in platforms styles imageformats iconengines; do
    [ -d "$dist/$group" ] && find "$dist/$group" -name '*d.dll' -delete 2>/dev/null
done
true
# qminimal/qoffscreen are headless platform plugins; useless in a shipped GUI.
rm -f "$dist/platforms/qminimal.dll" "$dist/platforms/qoffscreen.dll"
rm -f "$dist/imageformats/qjpeg.dll" "$dist/imageformats/qgif.dll" "$dist/imageformats/qico.dll"

# Qt's own translations, trimmed to the languages the app ships. Read from
# CMakeLists so this list cannot drift from the one the build compiles. Only
# qtbase_<lang>.qm: Fedora's qt_<lang>.qm is a stub naming qtbase and
# qtmultimedia, which main.cpp never needs once qtbase has loaded.
LANGUAGES=$(sed -n 's/^set(LANGUAGES \(.*\))$/\1/p' "$root/src/CMakeLists.txt")
[ -n "$LANGUAGES" ] || { echo "error: no LANGUAGES in src/CMakeLists.txt" >&2; exit 1; }
qttr="$sysroot/share/qt6/translations"
if [ ! -d "$qttr" ]; then
    echo "error: no Qt translations at $qttr (sudo tools/toolkit-env.sh $TOOLKIT_ARCH install)" >&2
    exit 1
fi
mkdir -p "$dist/translations"
for l in $LANGUAGES; do
    cp "$qttr/qtbase_$l.qm" "$dist/translations/" 2>/dev/null || true
done
if [ -z "$(ls -A "$dist/translations")" ]; then
    echo "error: $qttr contained none of the expected qtbase_*.qm files" >&2
    exit 1
fi

# A missing objdump is silent: no runtime DLLs get copied and the folder is
# reported ready around an executable that cannot start.
command -v "$objdump" >/dev/null 2>&1 || {
    echo "error: $objdump not found, so the DLLs the build depends on could not" >&2
    echo "       be resolved. It comes with llvm-mingw; see tools/toolkit-env.sh." >&2
    exit 1
}

deploy_resolve_closure "$objdump" "$bin" "$dist"
if [ ! -f "$dist/liblzma.dll" ]; then
    echo "error: liblzma.dll was not found in $bin, so the package cannot start." >&2
    echo "       sudo tools/toolkit-env.sh $TOOLKIT_ARCH install builds it." >&2
    exit 1
fi

# Everything is in the sysroot's manifest, so rpm is never asked; a file that
# is not there stops the deploy (deploy_write_licenses).
deploy_write_licenses rpm "$dist" "$bin" "$qtplugins" "$qttr"

# Which toolkit it was built with, exactly: the image, by digest, when this
# runs in one (container_run sets W32DI_IMAGE), or the host's llvm-mingw and
# Qt otherwise.
{
    printf '\nThe libraries above are from the llvm-mingw-qt6 toolkit'
    if [ -n "${W32DI_IMAGE:-}" ]; then
        printf ', in the image\n%s.\n' "$W32DI_IMAGE"
    else
        printf ' built on this host: llvm-mingw %s, Qt %s.\n' \
            "$(cat "$TOOLKIT_LLVM_MINGW/.release" 2>/dev/null || echo unknown)" \
            "$(rpm -q --qf '%{VERSION}' qt6-qtbase 2>/dev/null || echo unknown)"
    fi
} >> "$dist/THIRD-PARTY-NOTICES.txt"

# Symbols the release build left in are dead weight in a package. strip is
# checked for and its errors left visible, so a missing or failing strip
# cannot quietly ship them.
strip_tool=$STRIP
command -v "$strip_tool" >/dev/null 2>&1 || {
    echo "error: $strip_tool not found, so the package would ship its debug" >&2
    echo "       symbols. It comes with llvm-mingw; see tools/toolkit-env.sh." >&2
    exit 1
}
find "$dist" \( -name '*.dll' -o -name '*.exe' \) \
    -exec "$strip_tool" --strip-unneeded {} +

deploy_check_dist "$dist"

echo "dist: $(find "$dist" -type f | wc -l) files, $(du -sh "$dist" | cut -f1)"
