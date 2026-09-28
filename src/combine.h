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

#ifndef COMBINE_H
#define COMBINE_H

// "Custom Partitioning": partitions taken from several image files, laid out on
// one device in an order of the user's choosing, under a partition table
// built for them.
//
// Nothing here touches a file or a device. An image is described by the first
// sectors of it (parseImageLayout), which is all a partition table needs: the
// images are only read in full when the plan is written. The plan says which
// sectors of which image go where, and what the table sectors hold, so the
// writer only copies.

#include <QByteArray>
#include <QList>
#include <QString>
#include <QStringList>

enum CombineTable { COMBINE_TABLE_NONE, COMBINE_TABLE_MBR, COMBINE_TABLE_GPT };

// One partition an image offers.
struct CombinePartition
{
    // GPT entry index or MBR primary slot (0-3); -1 for an image with no
    // partition table, which is one partition from sector 0.
    int slot;
    unsigned long long first;       // in the image
    unsigned long long sectors;     // 0: not known yet (see ImageLayout::wholeImage)
    QString name;                   // GPT partition name; empty otherwise
    QString typeLabel;              // for display: "EFI system", "FAT32 (0x0C)", ...
    // The entry as the image has it: for GPT the first 128 bytes (type GUID,
    // unique GUID, LBAs, attributes, name), for MBR the 16-byte entry.
    // Empty for an image with no table.
    QByteArray entry;
    // For an image with no table: the type it is given, from its filesystem.
    unsigned char mbrType;
    QByteArray gptType;             // 16 bytes, GPT (mixed-endian) order
};

// What parseImageLayout() found in an image.
struct ImageLayout
{
    CombineTable table;
    QList<CombinePartition> partitions;     // in on-disk order
    // First sector after the partition table: 1 for MBR, the end of the
    // entry array for GPT; 0 without a table.
    unsigned long long tableEnd;
    // GPT FirstUsableLBA; tableEnd for MBR.
    unsigned long long firstUsable;
    // Where the first partition starts: [tableEnd, leadEnd) is the space a
    // lead-in copies. 0 without a table.
    unsigned long long leadEnd;
    // GPT geometry, kept so a lead-in's table sits exactly where its image
    // had it.
    unsigned long long entryLba, numEntries, entrySize;
    QByteArray diskGuid;            // GPT, 16 bytes
    // Sectors [0, tableEnd) as the image has them: the MBR, or the protective
    // MBR, GPT header and entry array. A lead-in's table is built over these,
    // so its boot code, and anything else it keeps among them, survives.
    QByteArray tableRegion;
    // An image with no partition table: its one partition is the whole image,
    // whose size is known only when the image records it or has been scanned.
    bool wholeImage;
};

// Reads an image's layout from its first sectors (head). A GPT needs its
// entry array: if head ends before it, returns false with *needsectors set to
// how many are needed, and the caller reads that many and tries again.
// Otherwise false means an unusable table, with *detail saying why.
// imagesectors is the image's size when known, else 0; for an image with no
// table it becomes the one partition's size (0: to be found by a full scan).
bool parseImageLayout(const QByteArray &head, unsigned long long sectorsize,
                      unsigned long long imagesectors, ImageLayout *layout,
                      unsigned long long *needsectors, QString *detail);

// One partition chosen for the device: partition index into
// images[image].partitions.
struct CombineChoice
{
    int image;
    int partition;
};

// A run of sectors to copy: [srcfirst, srcfirst + length) of image `image`
// to [dstfirst, dstfirst + length) on the device.
struct CombineRange
{
    int image;
    unsigned long long srcfirst;
    unsigned long long dstfirst;
    unsigned long long length;
};

// One partition as the device will have it, for the preview.
struct CombinePlaced
{
    int image;                      // -1 for the lead-in
    int partition;
    int slot;                       // entry index on the device
    unsigned long long first;
    unsigned long long sectors;
};

// What planCombine() produces.
struct CombinePlan
{
    CombineTable table;
    // Sectors [0, headersectors): the new partition table -- the MBR, or the
    // protective MBR, GPT header and entry array.
    QByteArray headerregion;
    unsigned long long headersectors;
    // The lead-in (if any) first, then each partition, in device order.
    QList<CombineRange> ranges;
    // GPT only: the backup entry array and header, at
    // [backupfirst, backupfirst + backupregion.size() / sectorsize), the end
    // of the device. Empty for MBR.
    QByteArray backupregion;
    unsigned long long backupfirst;
    // The first sector past the last partition.
    unsigned long long usedsectors;
    // The whole device, or for an image file (devicesectors 0) the image:
    // usedsectors, then the backup GPT if there is one.
    unsigned long long totalsectors;
    QList<CombinePlaced> placed;
    // Unique partition GUIDs that two or more chosen GPT partitions share,
    // as text, each once. Empty after planning with newguids set.
    QStringList duplicateGuids;
};

// Plans the device: the chosen partitions in the order given, each aligned to
// alignsectors except, with a lead-in, the first, which goes right where the
// lead-in ends (the lead-in image's own first-partition start, or 32 MiB past
// its table when it has more space than that: see LEADING_RESERVE_BYTES in
// disk.cpp). leadimage is an index into images, or -1.
//
// The table is the lead-in image's kind; without a lead-in, MBR when every
// chosen partition comes from an MBR image or an image with no table and
// there are at most four, GPT otherwise. GPT entries keep their type, unique
// GUID, attributes and name; MBR entries their type and boot flag. Partitions
// crossing between the two kinds have their type translated, where it is one
// the translation knows.
//
// newguids gives each chosen GPT partition whose unique GUID another chosen
// one already has a new one; the first of them keeps its own.
//
// devicesectors 0 plans an image file instead of a device: exactly as big as
// the layout, its backup GPT, if any, in its last sectors.
//
// Returns false, with *detail saying why, when the choices cannot be laid out
// or do not fit devicesectors.
bool planCombine(const QList<ImageLayout> &images, const QList<CombineChoice> &choices,
                 int leadimage, unsigned long long sectorsize,
                 unsigned long long devicesectors, unsigned long long alignsectors,
                 bool newguids, CombinePlan *plan, QString *detail);

// A GPT GUID (16 bytes, mixed-endian) as text, as the tools print it.
QString gptGuidText(const QByteArray &guid);

#endif // COMBINE_H
