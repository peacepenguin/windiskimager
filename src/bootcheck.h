/**********************************************************************
 *  This program is free software; you can redistribute it and/or     *
 *  modify it under the terms of the GNU General Public License       *
 *  as published by the Free Software Foundation; either version 2    *
 *  of the License, or (at your option) any later version.            *
 *                                                                    *
 *  This program is distributed in the hope that it will be useful,   *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of    *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the     *
 *  GNU General Public License for more details.                      *
 *                                                                    *
 *  You should have received a copy of the GNU General Public License *
 *  along with this program; if not, see http://gnu.org/licenses/     *
 *  ---                                                               *
 *  Copyright (C) 2026 peacepenguin (fork not affiliated    *
 *  with the upstream ImageWriter project)                            *
 *  https://github.com/peacepenguin/windiskimager                   *
 **********************************************************************/

#ifndef BOOTCHECK_H
#define BOOTCHECK_H

// "Check Boot": does the Windows boot configuration (BCD) on a disk name the
// disk as it is now? Windows gives a disk a new disk GUID, or MBR disk
// signature, when it is attached where a disk with the same one already is
// -- a Disk2vhd VHDX mounted on the machine it was taken from -- and the BCD,
// which names its partitions by disk ID and partition GUID (or offset), then
// no longer finds them, and the copy does not boot.
//
// Everything here only reads, through a sector reader, so it works on a
// disk Windows has taken offline, and on an image file in the harness. The
// result is a report, with the commands to fix the BCD -- and Windows' own
// record of its drive letters, MountedDevices, which names partitions the
// same way -- with Windows' own tools, on this computer with the disk
// attached, or inside the VM.

#include <QByteArray>
#include <QList>
#include <QMap>
#include <QString>
#include <QStringList>
#include <functional>
#include "combine.h"

// Reads `count` sectors of the disk from sector `first` into buf.
typedef std::function<bool(unsigned long long first, unsigned long long count, char *buf)>
    BootSectorReader;

// A file from the FAT or NTFS filesystem in [first, first + sectors) of the
// disk, by its path: {"EFI", "Microsoft", "Boot", "BCD"}. Names match
// without regard to case, long FAT names included. False, with *why, when
// the filesystem or the file cannot be read.
bool fatReadFile(const BootSectorReader &read, unsigned long long sectorsize,
                 unsigned long long first, unsigned long long sectors,
                 const QStringList &path, QByteArray *data, QString *why);
bool ntfsReadFile(const BootSectorReader &read, unsigned long long sectorsize,
                  unsigned long long first, unsigned long long sectors,
                  const QStringList &path, QByteArray *data, QString *why);

// One partition a BCD element names.
struct BcdDeviceRef
{
    QString object;                 // the object's GUID, "{...}", lower case
    unsigned int objectType;        // its Description\Type: 0x10200003 an OS loader, ...
    QString description;            // its 12000004 element, if it has one
    unsigned int element;           // 0x11000001 (device), 0x21000001 (osdevice), ...
    bool gpt;
    // GPT: the partition's unique GUID and the disk GUID, 16 bytes each, as a
    // GPT stores them. MBR: the partition's offset in bytes, 8, and the disk
    // signature, 4.
    QByteArray partition;
    QByteArray disk;
    // Inside another device -- a ramdisk's, which loads a file from the
    // partition -- rather than the element's device itself.
    bool nested;
    QString path;                   // a ramdisk's file, as found in the element
    QString options;                // a ramdisk's options object, "{...}"
};

// Every partition the BCD's elements name. *objects is how many objects it
// holds. False, with *why, when the hive cannot be read.
bool parseBcdHive(const QByteArray &hive, QList<BcdDeviceRef> *refs, int *objects,
                  QString *why);

// What Check Boot knows of the disk beyond its partition table.
struct BootCheckDisk
{
    int number = -1;                // as diskpart numbers it here
    unsigned long long sectorsize = 512;
    unsigned long long sectors = 0;  // its size; 0: where its last partition ends
    // Windows' own partition numbers (diskpart's), by first sector; the
    // table's order is used for any not in it.
    QMap<unsigned long long, int> partitionNumbers;
    // Another disk on this computer that has disk ID `id` (16 bytes, or 4 for
    // an MBR signature), named as "Disk 0", or empty.
    std::function<QString(const QByteArray &id)> diskWithId;
    // Letters the disk's partitions have on this computer, by first sector,
    // and the letters free here ("DEFG..."), for the fix's commands. Empty:
    // none have one, and S:, W:, R: ... are taken to be free.
    QMap<unsigned long long, QChar> partitionLetters;
    QString freeLetters;
};

struct BootCheckResult
{
    bool bcdFound = false;
    bool ok = false;                // every partition the BCD names is this disk's, as it is now
    QString report;
};

// Finds the BCD on the disk (the EFI system partition's
// \EFI\Microsoft\Boot\BCD, or a BIOS system partition's \Boot\BCD), checks
// every partition it names against the disk, and reports, with the
// commands to fix what does not match.
BootCheckResult checkBoot(const ImageLayout &layout, const BootSectorReader &read,
                          const BootCheckDisk &disk);

#endif // BOOTCHECK_H
