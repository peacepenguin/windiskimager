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
 *  Copyright (C) 2009, Justin Davis <tuxdavis@gmail.com>             *
 *  Copyright (C) 2009-2017 ImageWriter developers                    *
 *                 https://sourceforge.net/projects/win32diskimager/  *
 *  ---                                                               *
 *  Modified 2026 by peacepenguin (fork not affiliated      *
 *  with the upstream ImageWriter project):                           *
 *  GPT repair, device enumeration, volume locking                    *
 *  https://github.com/peacepenguin/windiskimager                   *
 **********************************************************************/

#ifndef DISK_H
#define DISK_H

#ifndef WINVER
#define WINVER 0x0601
#endif

#include <QtWidgets>
#include <QString>
#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <winioctl.h>
#ifndef FSCTL_IS_VOLUME_MOUNTED
#define FSCTL_IS_VOLUME_MOUNTED  CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 10, METHOD_BUFFERED, FILE_ANY_ACCESS)
#endif // FSCTL_IS_VOLUME_MOUNTED

// Not declared by the MinGW headers.
#ifndef IOCTL_DISK_SET_DISK_ATTRIBUTES
#define IOCTL_DISK_SET_DISK_ATTRIBUTES  CTL_CODE(IOCTL_DISK_BASE, 0x003d, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define DISK_ATTRIBUTE_OFFLINE          0x0000000000000001
#define DISK_ATTRIBUTE_READ_ONLY        0x0000000000000002
typedef struct _SET_DISK_ATTRIBUTES
{
    DWORD      Version;
    BOOLEAN    Persist;
    BYTE       Reserved1[3];
    DWORDLONG  Attributes;
    DWORDLONG  AttributesMask;
    DWORD      Reserved2[4];
} SET_DISK_ATTRIBUTES, *PSET_DISK_ATTRIBUTES;
#endif // IOCTL_DISK_SET_DISK_ATTRIBUTES

// IOCTL control code
#define IOCTL_STORAGE_QUERY_PROPERTY   CTL_CODE(IOCTL_STORAGE_BASE, 0x0500, METHOD_BUFFERED, FILE_ANY_ACCESS)

// One physical disk, as offered in the device list. Enumerated by
// \\.\PhysicalDriveN, not drive letter: a card holding a Linux image has no
// filesystem Windows can mount, so it gets no letter.
struct PhysicalDevice
{
    ULONG deviceNumber;             // N in \\.\PhysicalDriveN
    QString description;            // vendor + product, as the device reports it
    QString letters;                // "E:, F:", empty when nothing is mounted
    unsigned long long sizeBytes;
    bool removable;                 // non-SATA removable media, or on the USB/SD/MMC bus
};

// Every physical disk that could plausibly be a target. Removable devices are
// always listed; the rest only when includeFixed is set (internal card readers
// can present the card as a fixed disk). Never lists the Windows system disk
// or a device reporting no size (e.g. an empty card reader).
QList<PhysicalDevice> enumeratePhysicalDevices(bool includeFixed);

// Mounted volumes on physical disk deviceID as "E:, F:"; empty when none.
QString driveLettersOnDevice(ULONG deviceID);

// True if path (which need not exist yet) is on a volume with any extent on
// physical disk deviceID. Resolved through the path's mount point, so a
// partition mounted as a folder is found too.
bool pathIsOnDisk(const QString &path, ULONG deviceID);

HANDLE getHandleOnFile(LPCWSTR filelocation, DWORD access);
HANDLE getHandleOnDevice(int device, DWORD access);
bool removeLockOnVolume(HANDLE handle);
bool unmountVolume(HANDLE handle);

// Locks every volume on a disk, not just one: any volume left unlocked keeps
// its filesystem driver live, free to flush cached metadata over the image.
class LockedVolumes
{
public:
    LockedVolumes() {}
    ~LockedVolumes() { release(); }
    // Lock and dismount every volume with an extent on physical disk deviceID.
    bool lockAll(DWORD deviceID);
    void release();
private:
    QList<HANDLE> handles;
};

// ---------------------------------------------------------------------------
// GPT
//
// When a disk's backup GPT is not at its last LBA, Windows rewrites the table
// on rescan and sets PartitionEntryLBA to FirstUsableLBA minus the entry-array
// length. That is only correct when the array ends right at FirstUsableLBA
// (the ordinary layout); on images that reserve space ahead of the first
// partition (ARM boards) it corrupts the primary table. See README.md and
// TESTING-GPT-BUG.md. The functions below avoid provoking that rewrite,
// predict its harm, or undo it.
// ---------------------------------------------------------------------------

// Zero both ends of the device before writing an image, so a previous larger
// image's backup GPT cannot survive at the end and be reconciled against the
// new one.
bool wipePartitionTables(HANDLE hRawDisk, unsigned long long sectorsize,
                         unsigned long long devicesectors);

// Result of relocateBackupGPT().
enum GptFixResult
{
    GPT_FIX_OK,          // table rewritten to match the device
    GPT_FIX_NOT_NEEDED,  // already consistent with the device size
    GPT_FIX_DISABLED,    // not attempted; "Fix GPT after write" is unchecked
    GPT_FIX_NO_GPT,      // the device holds no GPT at all; nothing to repair
    GPT_FIX_BAD_GPT,     // a GPT is present but malformed; nothing was touched
    GPT_FIX_FAILED       // I/O error, or the table cannot be fitted to the device
};

// Move the backup GPT to the device's last LBA and update AlternateLBA,
// LastUsableLBA and the protective MBR to match, as "sgdisk -e" does, then
// clear the image's stale backup copy. A table already consistent with the
// device leaves Windows nothing to rewrite.
GptFixResult relocateBackupGPT(HANDLE hRawDisk, unsigned long long sectorsize,
                               unsigned long long devicesectors, QString *detail);

// True when sector 0 has an MBR boot signature and at least one entry whose
// type is neither empty nor 0xEE (GPT protective). Used only to tell an MBR
// image from one with no partition table when there is no GPT to repair.
bool deviceHasMbrTable(HANDLE hRawDisk, unsigned long long sectorsize);

// Whether this table would survive the rewrite.
enum GptRewriteRisk
{
    GPT_RISK_UNKNOWN,   // could not be determined
    GPT_RISK_NO_GPT,    // no GPT on the device; nothing to rewrite
    GPT_RISK_SAFE,      // the rewrite would land on the correct value anyway
    GPT_RISK_AFFECTED   // the rewrite would invalidate the primary table
};

// Compare the real PartitionEntryLBA against the value Windows would compute
// for it.
GptRewriteRisk gptRewriteRisk(HANDLE hRawDisk, unsigned long long sectorsize);

// What state the device's primary GPT is in, judged against itself.
enum GptPrimaryState
{
    GPT_PRIMARY_OK,       // the header and the entry array it points at agree
    GPT_PRIMARY_NO_GPT,   // there is no GPT here
    GPT_PRIMARY_UNKNOWN,  // unreadable, or damaged in some other way
    GPT_PRIMARY_BROKEN    // the header checks out but points at the wrong entries
};

// Detect the rewrite's damage: Windows recomputes the header CRC over the
// pointer it wrote, so the header passes its own check while
// PartitionEntryArrayCRC32 no longer matches what PartitionEntryLBA points at.
GptPrimaryState gptPrimaryState(HANDLE hRawDisk, unsigned long long sectorsize,
                                unsigned long long devicesectors);

// Point PartitionEntryLBA back at LBA 2 and rebuild the header CRC. The
// rewrite leaves PartitionEntryArrayCRC32 alone, so it is used to confirm the
// real entries are at LBA 2 first. Only the header sector is written. Returns
// false, with the reason in *detail, when the damage is not this shape.
bool repairPrimaryGpt(HANDLE hRawDisk, unsigned long long sectorsize,
                      unsigned long long devicesectors, QString *detail);

// Where the image's own backup GPT sits, read from the image's sector 1
// (lba1) because the device no longer records it once the fix has zeroed that
// copy. A verify expects those sectors to differ. Returns false if the image
// holds no usable GPT.
bool gptImageBackupRange(const unsigned char *lba1, unsigned long long sectorsize,
                         unsigned long long *first, unsigned long long *last);

// Sectors "Fix GPT after write" may rewrite, so a verify can tell them from a
// bad card: front [0, *frontend) is the protective MBR (of which only the
// protective entry's size field, bytes 458-461, changes) and the primary
// header; tail [*tailstart, devicesectors) is the relocated backup array and
// header. The primary entry array is never rewritten, so it is not included.
// Returns false, setting neither, if the device holds no usable GPT.
bool gptOwnedSectors(HANDLE hRawDisk, unsigned long long sectorsize,
                     unsigned long long devicesectors,
                     unsigned long long *frontend, unsigned long long *tailstart);

// A run of sectors planGptShrink()/planMbrShrink() copy: [srcfirst,
// srcfirst + length) on the device becomes [dstfirst, dstfirst + length) in
// the image.
struct ShrinkCopyRange
{
    unsigned long long srcfirst;
    unsigned long long dstfirst;
    unsigned long long length;
};

// A "Skip unpartitioned space" plan from planGptShrink() or planMbrShrink(); both
// are driven the same way. The image is headerregion, then each range at its
// dstfirst, then backupregion, totalsectors long.
struct PartitionShrinkPlan
{
    // Sectors [0, headersectors): the partition table, patched to describe
    // the repacked partitions -- the boot sector for MBR; the protective MBR,
    // primary header and entry array for GPT.
    QByteArray headerregion;
    unsigned long long headersectors;
    // For GPT, first the reserved area from the end of the table to
    // FirstUsableLBA, copied where it is: an image that stores its bootloader
    // outside the partitions reserves it this way (genimage, for one, sets
    // FirstUsableLBA to the end of such data). Then the kept partitions in
    // on-disk order, each aligned to alignsectors but never moved later than
    // it was. Anything else outside the partitions is dropped, which can
    // leave an image unbootable if its creator did not reserve it -- MBR
    // cannot, so on MBR it always goes. Gaps between ranges must be written
    // as explicit zeros: the output may not be a sparse file that zero-fills
    // skipped regions.
    QList<ShrinkCopyRange> ranges;
    // Sectors [totalsectors - backupsectors, totalsectors): the backup entry
    // array and header for the repacked layout, precomputed so the image is
    // written strictly in order with no read-modify-write afterward, which a
    // compressed output stream requires. Empty, with backupsectors 0, for MBR.
    QByteArray backupregion;
    unsigned long long backupsectors;
    unsigned long long totalsectors;
};

// Plan a GPT "Skip unpartitioned space" read that removes the unpartitioned space
// above FirstUsableLBA -- before, between and after the partitions -- and any
// excluded partition. The reserved area below FirstUsableLBA is kept (see
// PartitionShrinkPlan). Pass
// alignsectors = 1048576 / sectorsize so every partition starts on a 1MiB
// boundary (the Windows/parted/sgdisk default, a multiple of any real sector
// or erase-block size). Returns false, with *plan untouched, if the device
// holds no usable GPT, a partition's range makes no sense or overlaps
// another, no partitions remain, or there is nothing to gain.
// excludeSlots, if non-NULL, lists slots (GPT entry index, or MBR primary
// entry index 0-3) to drop: the table entry is zeroed and the data not
// copied.
bool planGptShrink(HANDLE hRawDisk, unsigned long long sectorsize,
                   unsigned long long devicesectors, unsigned long long alignsectors,
                   PartitionShrinkPlan *plan, QString *detail,
                   const QList<int> *excludeSlots = NULL);

// planGptShrink() for a legacy MBR, packing from right after the boot sector.
// Only the four primary entries are walked; an extended partition moves as a
// whole with its logical ones inside.
// Returns false, with *plan untouched, if the device holds no MBR, an entry
// describes an impossible or overlapping range, a repacked start exceeds 32
// bits, no partitions remain, or there is nothing to gain.
bool planMbrShrink(HANDLE hRawDisk, unsigned long long sectorsize,
                   unsigned long long devicesectors, unsigned long long alignsectors,
                   PartitionShrinkPlan *plan, QString *detail,
                   const QList<int> *excludeSlots = NULL);

// One partition as offered for "choose partitions to read". slot matches
// excludeSlots above; name is empty for MBR.
struct PartitionInfo
{
    int slot;
    unsigned long long firstSector;
    unsigned long long sectors;
    QString name;
};

// Drive letters mounted on physical disk deviceID, keyed by the starting byte
// offset of each volume's first extent -- the only link between a mounted
// volume and a partition table entry.
QMap<unsigned long long, QString> driveLettersByOffset(ULONG deviceID);

// Windows' own partition numbers (diskpart's "Partition ###"), keyed by
// starting sector. These can differ from slot + 1: a partition created into a
// slot freed by an earlier deletion keeps the number it was given. Returns
// false if the drive layout cannot be read.
bool diskPartitionNumbers(HANDLE hRawDisk, unsigned long long sectorsize,
                          QMap<unsigned long long, int> *numbersBySector);

// List every in-use partition on a GPT device, sorted by starting sector
// (diskpart's order; slot order is unrelated to position on disk). Returns
// false if the device holds no usable GPT.
bool listGptPartitions(HANDLE hRawDisk, unsigned long long sectorsize,
                       unsigned long long devicesectors,
                       QList<PartitionInfo> *partitions, QString *detail);

// The MBR equivalent of listGptPartitions(), over the four primary entries
// only. Returns false if the device holds no usable MBR.
bool listMbrPartitions(HANDLE hRawDisk, unsigned long long sectorsize,
                       unsigned long long devicesectors,
                       QList<PartitionInfo> *partitions, QString *detail);

bool flushDevice(HANDLE handle);
bool setDiskOffline(HANDLE handle, bool offline);
bool ejectDevice(HANDLE handle);
char *readSectorDataFromHandle(HANDLE handle, unsigned long long startsector, unsigned long long numsectors, unsigned long long sectorsize);
bool writeSectorDataToHandle(HANDLE handle, char *data, unsigned long long startsector, unsigned long long numsectors, unsigned long long sectorsize);
// Sectors on the device, or 0. *reported is set when the failure has already
// been shown to the user, so the caller need not report it again.
unsigned long long getNumberOfSectors(HANDLE handle, unsigned long long *sectorsize,
                                      bool *reported = NULL);
unsigned long long getFileSizeInSectors(HANDLE handle, unsigned long long sectorsize);
// Whether the volume holding directory `location` has spaceneeded bytes free;
// true if that cannot be determined. Uses the wide API so non-ASCII paths work.
bool spaceAvailable(const QString &location, unsigned long long spaceneeded);

#endif // DISK_H
