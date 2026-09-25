<p align="center">
  <img src="src/images/WinDiskImager.svg" alt="WinDiskImager icon" width="112" height="112">
</p>

<h1 align="center">WinDiskImager</h1>

<p align="center">
  Write, read and verify raw disk images on Windows, without breaking the partition table.
</p>

WinDiskImager copies raw images to and from any Windows block device: USB flash
drives, SD cards, SATA and NVMe disks, drive enclosures and mounted VHDX files.
Run it as Administrator, choose an image and a device, and click **Write**,
**Read** or **Verify**.

It is a fork of Win32DiskImager. Its main purpose is to stop Windows from
corrupting the GPT of images like ARM board images. See
[The Windows GPT problem](#the-windows-gpt-problem).

> This program comes with no warranty. The authors take no responsibility for
> lost or damaged data.

## Features

- **Write** a raw image (`.img`, `.raw`, `.bin`) or one compressed with gzip,
  xz, bzip2 or zstd (`.gz`, `.xz`, `.bz2`, `.zst`) to a device. The format is
  recognised from the file's contents, and compressed images are decompressed
  as they stream, so no expanded copy is ever written to disk.
- **Read** a device to an `.img`, or with **Compress during Read** to an
  `.img.gz` (the default), `.img.xz`, `.img.bz2` or `.img.zst`.
  - **Skip unpartitioned space** leaves out the space outside the partitions.
  - **Choose partitions to read** leaves out whole partitions you pick.
- **Verify** a device against an image byte for byte, compressed images
  included. It also checks the partition table and can repair one that
  Windows has broken.
- **Drop** an image file anywhere on the window to use it, instead of browsing
  for it.
- **Hash** the image with MD5, SHA1 or SHA256. SHA256 is selected by default,
  since that is what publishers usually quote.
- **Fix GPT after write** makes the partition table match the device, so
  Windows has nothing to rewrite.

## Choosing a device

Devices are listed as physical disks, not drive letters. A card holding a Linux
image often gets no drive letter at all, but it still appears here. Each entry
shows the model, size and any drive letters.

Removable and USB/SD/MMC devices are always listed. **Show all devices** adds
fixed disks, for internal card readers that present the card as non-removable.
The disk Windows runs from is never listed. The list is rescanned each time you
open it, because inserting a card into some readers sends Windows no
notification.

Before writing, every volume on the target disk is locked and dismounted. That
includes volumes mounted as folders or with no drive letter.

## Smaller images on Read

By default, Read copies the whole device, sector by sector.

**Skip unpartitioned space** reads the device's MBR or GPT and packs the partitions
back to back, removing the unpartitioned space between and after them.
The space before the first partition is where board images keep their
bootloader, and nothing in the partition table reliably says how far it runs
(`FirstUsableLBA` often stops well short of it). So when there is 32 MB or less
of it after the partition table, all of it is kept and the first partition
does not move. When there is more, the first 32 MB is kept exactly as it is
and the rest is removed; every known bootloader layout fits in 16 MB. A
GPT's `FirstUsableLBA` is always honoured. Partitions are aligned to 1 MiB,
the same default as Windows, `parted` and `sgdisk`, and never move later on
the disk.
For GPT, the backup table is rebuilt at the new end of the image. A device with
no partition table, or nothing to remove, is read in full, and so is a GPT
device whose GPT cannot be repacked: its MBR is never repacked in its place.

**Choose partitions to read** lists the device's partitions before reading.
Partitions are numbered as `diskpart` numbers them and show their drive letter
if they have one. Anything you uncheck is removed from the image and from its
partition table. This always skips unpartitioned space too; leaving out the
first partition moves the next one to where it started. If the partitions
cannot be repacked, the read stops rather than including the ones you left out.

Both options work together with **Compress during Read**.

## The Windows GPT problem

An image smaller than the card leaves its backup GPT where the image ends, not
where the device ends. Windows rewrites the table the next time it scans the
disk, which happens every time the card is plugged in. Most of that rewrite is
correct: it moves the backup GPT to the end of the device.

It also recomputes the primary header's `PartitionEntryLBA` as
`FirstUsableLBA - 32`. On an ordinary image `FirstUsableLBA` is 34, so the
result happens to be correct. ARM board images such as rk3588 reserve space
before the first partition, so `FirstUsableLBA` is higher and the pointer lands
on empty space. The primary table is then corrupt and the board no longer boots.
Windows still shows a healthy disk, because it only checks the backup copy it
wrote itself. No data is lost; only the table breaks.

This is how Windows behaves, not a bug in any one tool; Rufus triggers it too.
[TESTING-GPT-BUG.md](TESTING-GPT-BUG.md) explains it in full and includes a
48 MB reproducer that shows the damage in about a minute, with no SD card
needed.

**Fix GPT after write** decides how a write deals with this:

- **Checked (default):** after the write, the backup GPT is moved to the last
  sector of the device and the header is updated, as `sgdisk -e` does. Windows
  finds nothing to rewrite, and the card can be handled normally.
- **Unchecked:** the card is left byte-identical to the image, like `dd` on
  Linux. It is taken offline and ejected before Windows can scan it. Plugging it
  back into Windows triggers the rewrite.

**Verify** also catches a table Windows has already broken. The data still
matches the image in that case, so a byte comparison alone would pass. Verify
reports the damage and offers to repair it by pointing `PartitionEntryLBA` back
at the entry array, without touching any data.

Every write also zeroes the first and last 34 sectors first, so no table left
from a previous, larger image survives. `tools/gptdump.py` decodes and checks
both GPT headers of an image or device.

## More

| | |
|---|---|
| Building from source | [BUILD.md](BUILD.md) |
| Release notes | [Changelog.txt](Changelog.txt) |
| Translating | [readme-translations.txt](readme-translations.txt) |
| The GPT problem in depth | [TESTING-GPT-BUG.md](TESTING-GPT-BUG.md) |
| Bugs and questions | [github.com/peacepenguin/windiskimager](https://github.com/peacepenguin/windiskimager/issues) |

## About this fork

WinDiskImager is a fork of Win32DiskImager (the ImageWriter project). It is not
affiliated with the original project or its maintainers. It has its own name so
that its builds are not mistaken for theirs. Please report problems to this
repository, not to SourceForge.

## License

Licensed under the GNU General Public License, version 2 or (at your option)
any later version; see [License.txt](License.txt) and [GPL-2](GPL-2). The
changes in this fork are Copyright (C) 2026 peacepenguin under the same license,
and each modified file says what changed.

The Windows package includes Qt 6, under the GNU LGPL version 3, and other
open-source libraries under their own licences. Its copy of
[THIRD-PARTY-NOTICES.txt](THIRD-PARTY-NOTICES.txt) lists every library it
ships, with its version, licence and source, and its `licenses` folder holds
their licence texts.

Originally developed by Justin Davis <tuxdavis@gmail.com> and maintained by the
ImageWriter developers (https://sourceforge.net/projects/win32diskimager).
