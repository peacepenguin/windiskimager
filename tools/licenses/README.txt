Licence texts for packages that install none of their own.

tools/deploy.sh and tools/deploy-cross.sh ship each library's licence from the
package manager that supplied it (see deploy_write_licenses in
tools/build-env.sh). A few packages carry no licence file or README at all;
for those, the files in tools/licenses/<package>/ are shipped instead. The
folder name is the exact package name as pacman or rpm reports it.

mingw64-zlib (Fedora) installs no licence file. Its LICENSE here is the zlib
licence as zlib itself distributes it.
