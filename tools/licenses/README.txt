Licence texts the packages leave out.

tools/deploy.sh and tools/deploy-cross.sh ship each library's licence from the
package manager that supplied it (see deploy_write_licenses in
tools/build-env.sh). Anything in tools/licenses/<package>/ is shipped for that
package as well, for packages whose own licence files are missing or only
point at a text they do not include. The folder name is the exact package name
as pacman or rpm reports it.

All of these are Fedora's; the texts are the upstream ones, as MSYS2's packages
of the same libraries install them.

mingw64-zlib installs no licence file. LICENSE is the zlib licence as zlib
itself distributes it.

mingw64-icu installs only license.html, which says the licence has moved to
LICENSE. LICENSE is ICU's.

mingw64-freetype installs LICENSE.TXT, which offers a choice of the FreeType
License or GPLv2 and points at docs/FTL.TXT for the former. FTL.TXT is it.

mingw64-pcre2 installs COPYING, which says to see LICENCE. LICENCE.md is
PCRE2's.

mingw64-gettext installs COPYING, the GPL-3 that covers gettext's tools, but
the libintl-8.dll that ships is LGPL-2.1-or-later. COPYING.LIB is that
licence, from gettext's intl directory.

mingw64-zstd installs COPYING, the GPL-2 half of zstd's dual licence. LICENSE
is the BSD half, the one the MSYS2 package ships, so both packages carry it.
