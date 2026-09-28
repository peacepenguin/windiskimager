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

#ifndef WINVER
#define WINVER 0x0601
#endif

#include "combine.h"
#include "disk.h"

#include <QObject>
#include <QSet>
#include <QUuid>
#include <algorithm>
#include <cstring>

// GPT header and entry fields, from the UEFI specification.
enum
{
    H_HEADERSIZE = 12, H_HEADERCRC = 16, H_MYLBA = 24, H_ALTLBA = 32,
    H_FIRSTUSABLE = 40, H_LASTUSABLE = 48, H_DISKGUID = 56, H_ENTRYLBA = 72,
    H_NUMENTRIES = 80, H_ENTRYSIZE = 84, H_ENTRIESCRC = 88, H_SIZE = 92
};
enum { E_TYPE = 0, E_UNIQUE = 16, E_FIRST = 32, E_LAST = 40, E_ATTRS = 48, E_NAME = 56, E_SIZE = 128 };
// GPT attribute bit 2: legacy BIOS bootable, the GPT counterpart of the MBR
// boot flag.
static const unsigned long long GPT_ATTR_LEGACY_BOOT = 1ull << 2;
// MBR entries, at 446 + 16 * slot.
enum { M_TABLE = 446, M_STATUS = 0, M_TYPE = 4, M_START = 8, M_COUNT = 12, M_SIZE = 16 };

static unsigned int rd16(const unsigned char *p, int o)
{
    return (unsigned int)p[o] | ((unsigned int)p[o + 1] << 8);
}
static unsigned long long rd32(const unsigned char *p, int o)
{
    return (unsigned long long)p[o] | ((unsigned long long)p[o + 1] << 8)
         | ((unsigned long long)p[o + 2] << 16) | ((unsigned long long)p[o + 3] << 24);
}
static unsigned long long rd64(const unsigned char *p, int o)
{
    unsigned long long v = 0;
    for (int i = 7; i >= 0; --i) v = (v << 8) | p[o + i];
    return v;
}
static void wr32(unsigned char *p, int o, unsigned long long v)
{
    for (int i = 0; i < 4; ++i) p[o + i] = (unsigned char)((v >> (8 * i)) & 0xFF);
}
static void wr64(unsigned char *p, int o, unsigned long long v)
{
    for (int i = 0; i < 8; ++i) p[o + i] = (unsigned char)((v >> (8 * i)) & 0xFF);
}

static unsigned long long alignUp(unsigned long long v, unsigned long long a)
{
    return ((v + a - 1) / a) * a;
}

// ------------------------------------------------------------------ GUIDs ---

// Text ("C12A7328-F81F-11D2-BA4B-00A0C93EC93B") to the 16 bytes a GPT holds,
// whose first three fields are little-endian.
static QByteArray guidBytes(const char *text)
{
    QByteArray rfc = QUuid::fromString(QLatin1String(text)).toRfc4122();
    QByteArray g(16, 0);
    for (int i = 0; i < 4; ++i) g[i] = rfc[3 - i];
    g[4] = rfc[5]; g[5] = rfc[4];
    g[6] = rfc[7]; g[7] = rfc[6];
    for (int i = 8; i < 16; ++i) g[i] = rfc[i];
    return g;
}

QString gptGuidText(const QByteArray &g)
{
    if (g.size() != 16)
    {
        return QString();
    }
    const unsigned char *b = (const unsigned char *)g.constData();
    return QString::asprintf("%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",
                             b[3], b[2], b[1], b[0], b[5], b[4], b[7], b[6],
                             b[8], b[9], b[10], b[11], b[12], b[13], b[14], b[15]);
}

// A new random (version 4) GUID, in GPT byte order.
static QByteArray newGuid()
{
    return guidBytes(QUuid::createUuid().toString(QUuid::WithoutBraces).toLatin1().constData());
}

// ------------------------------------------------------------------ types ---

// The partition types either table can hold, and how one maps to the other.
// mbr 0: no MBR counterpart; gpt NULL: no GPT counterpart.
struct PartType
{
    unsigned char mbr;
    const char *gpt;
    const char *label;
};

static const PartType GPT_TYPES[] =
{
    { 0xEF, "C12A7328-F81F-11D2-BA4B-00A0C93EC93B", "EFI system" },
    { 0x0C, "EBD0A0A2-B9E5-4433-87C0-68B6B72699C7", "Microsoft basic data" },
    { 0x83, "0FC63DAF-8483-4772-8E79-3D69D8477DE4", "Linux filesystem" },
    { 0x82, "0657FD6D-A4AB-43C4-84E5-0933C84B4F4F", "Linux swap" },
    { 0x8E, "E6D6D379-F507-44C2-A23C-238F2A3DF928", "Linux LVM" },
    { 0xFD, "A19D880F-05FC-4D3B-A006-743F0F84911E", "Linux RAID" },
    { 0x83, "4F68BCE3-E8CD-4DB1-96E7-FBCAF984B709", "Linux root (x86-64)" },
    { 0x83, "B921B045-1DF0-41C3-AF44-4C6F280D3FAE", "Linux root (ARM64)" },
    { 0x83, "69DAD710-2CE4-4E3C-B16C-21A1D49ABED3", "Linux root (ARM)" },
    { 0x83, "44479540-F297-41B2-9AF7-D131D5F0458A", "Linux root (x86)" },
    { 0x83, "BC13C2FF-59E6-4262-A352-B275FD6F7172", "Linux extended boot" },
    { 0x27, "DE94BBA4-06D1-4D40-A16A-BFD50179D6AC", "Windows recovery" },
    { 0x00, "21686148-6449-6E6F-744E-656564454649", "BIOS boot" },
    { 0x00, "E3C9E316-0B5C-4DB8-817D-F92DF00215AE", "Microsoft reserved" },
};

static const PartType MBR_TYPES[] =
{
    { 0x01, "EBD0A0A2-B9E5-4433-87C0-68B6B72699C7", "FAT12" },
    { 0x04, "EBD0A0A2-B9E5-4433-87C0-68B6B72699C7", "FAT16" },
    { 0x06, "EBD0A0A2-B9E5-4433-87C0-68B6B72699C7", "FAT16" },
    { 0x07, "EBD0A0A2-B9E5-4433-87C0-68B6B72699C7", "NTFS / exFAT" },
    { 0x0B, "EBD0A0A2-B9E5-4433-87C0-68B6B72699C7", "FAT32" },
    { 0x0C, "EBD0A0A2-B9E5-4433-87C0-68B6B72699C7", "FAT32" },
    { 0x0E, "EBD0A0A2-B9E5-4433-87C0-68B6B72699C7", "FAT16" },
    { 0x27, "DE94BBA4-06D1-4D40-A16A-BFD50179D6AC", "Windows recovery" },
    { 0x82, "0657FD6D-A4AB-43C4-84E5-0933C84B4F4F", "Linux swap" },
    { 0x83, "0FC63DAF-8483-4772-8E79-3D69D8477DE4", "Linux" },
    { 0x8E, "E6D6D379-F507-44C2-A23C-238F2A3DF928", "Linux LVM" },
    { 0xEF, "C12A7328-F81F-11D2-BA4B-00A0C93EC93B", "EFI system" },
    { 0xFD, "A19D880F-05FC-4D3B-A006-743F0F84911E", "Linux RAID" },
    { 0x05, NULL, "extended" },
    { 0x0F, NULL, "extended" },
    { 0x85, NULL, "extended" },
};

static bool isExtended(unsigned char type)
{
    return type == 0x05 || type == 0x0F || type == 0x85;
}

static const PartType *gptTypeFor(const QByteArray &guid)
{
    for (const PartType &t : GPT_TYPES)
    {
        if (guidBytes(t.gpt) == guid)
        {
            return &t;
        }
    }
    return NULL;
}

static const PartType *mbrTypeFor(unsigned char type)
{
    for (const PartType &t : MBR_TYPES)
    {
        if (t.mbr == type)
        {
            return &t;
        }
    }
    return NULL;
}

// ------------------------------------------------------------- filesystems ---

// The filesystem an image with no partition table starts with, for the type
// its one partition is given: FAT, NTFS or exFAT as Microsoft basic data,
// anything else -- ext*, btrfs, XFS, squashfs, or not recognized -- as Linux.
static void guessFilesystem(const QByteArray &head, unsigned long long sectorsize,
                            CombinePartition *p)
{
    const unsigned char *b = (const unsigned char *)head.constData();
    const int n = head.size();
    auto at = [&](int off, const char *sig) {
        const int len = (int)strlen(sig);
        return off + len <= n && memcmp(b + off, sig, (size_t)len) == 0;
    };
    const char *label = NULL;
    unsigned char mbr = 0x83;
    if (at(3, "NTFS    "))           { label = "NTFS";  mbr = 0x07; }
    else if (at(3, "EXFAT   "))      { label = "exFAT"; mbr = 0x07; }
    else if (at(82, "FAT32   "))     { label = "FAT32"; mbr = 0x0C; }
    else if (at(54, "FAT16   "))     { label = "FAT16"; mbr = 0x0E; }
    else if (at(54, "FAT12   "))     { label = "FAT12"; mbr = 0x01; }
    else if (n >= 1024 + 58 && rd16(b, 1024 + 56) == 0xEF53) { label = "ext2/3/4"; }
    else if (at(65536 + 64, "_BHRfS_M")) { label = "btrfs"; }
    else if (at(0, "XFSB"))          { label = "XFS"; }
    else if (at(0, "hsqs"))          { label = "squashfs"; }
    (void)sectorsize;
    const PartType *t = mbrTypeFor(mbr);
    p->mbrType = mbr;
    p->gptType = guidBytes(t->gpt);
    p->typeLabel = label ? QObject::tr("%1, no partition table").arg(QLatin1String(label))
                         : QObject::tr("unrecognized filesystem, no partition table");
}

// A boot sector that is a filesystem's own, not an MBR: a "superfloppy"
// image, formatted with no partition table. It carries the 0x55AA signature
// an MBR has.
static bool isVolumeBootRecord(const unsigned char *b, int n)
{
    auto at = [&](int off, const char *sig) {
        const int len = (int)strlen(sig);
        return off + len <= n && memcmp(b + off, sig, (size_t)len) == 0;
    };
    return at(3, "NTFS    ") || at(3, "EXFAT   ") || at(82, "FAT32   ")
        || at(54, "FAT16   ") || at(54, "FAT12   ");
}

// ------------------------------------------------------------------ parse ---

static bool parseGpt(const QByteArray &head, unsigned long long ss, unsigned long long imagesectors,
                     ImageLayout *out, unsigned long long *need, QString *detail)
{
    const unsigned char *hdr = (const unsigned char *)head.constData() + ss;
    const unsigned long long headersize = rd32(hdr, H_HEADERSIZE);
    if (headersize < H_SIZE || headersize > ss)
    {
        *detail = QObject::tr("the GPT header size is out of range");
        return false;
    }
    {
        QByteArray probe((const char *)hdr, (int)headersize);
        wr32((unsigned char *)probe.data(), H_HEADERCRC, 0);
        if (gptCrc32((const unsigned char *)probe.constData(), (size_t)headersize)
            != (DWORD)rd32(hdr, H_HEADERCRC))
        {
            *detail = QObject::tr("the GPT header checksum is invalid");
            return false;
        }
    }
    const unsigned long long entrylba = rd64(hdr, H_ENTRYLBA);
    const unsigned long long numentries = rd32(hdr, H_NUMENTRIES);
    const unsigned long long entrysize = rd32(hdr, H_ENTRYSIZE);
    const unsigned long long firstusable = rd64(hdr, H_FIRSTUSABLE);
    if (numentries == 0 || numentries > 65536 || entrysize < E_SIZE || entrysize > 4096
        || (entrysize % 8) != 0 || entrylba < 2)
    {
        *detail = QObject::tr("the GPT entry array geometry is not usable");
        return false;
    }
    const unsigned long long entrysectors = (numentries * entrysize + ss - 1) / ss;
    const unsigned long long tableend = entrylba + entrysectors;
    // The same bound planGptShrink() holds the table to.
    if (tableend * ss > 64ull * 1024ull * 1024ull)
    {
        *detail = QObject::tr("the GPT partition entry array is not where the header says");
        return false;
    }
    if ((unsigned long long)head.size() < tableend * ss)
    {
        *need = tableend;
        return false;
    }
    const unsigned char *entries = (const unsigned char *)head.constData() + entrylba * ss;
    if (gptCrc32(entries, (size_t)(numentries * entrysize)) != (DWORD)rd32(hdr, H_ENTRIESCRC))
    {
        *detail = QObject::tr("the GPT partition entry array checksum is invalid");
        return false;
    }
    if (firstusable < tableend)
    {
        *detail = QObject::tr("FirstUsableLBA lies inside the partition table");
        return false;
    }

    QList<CombinePartition> parts;
    for (unsigned long long i = 0; i < numentries; ++i)
    {
        const unsigned char *e = entries + i * entrysize;
        bool used = false;
        for (int b = 0; b < 16 && !used; ++b) used = (e[E_TYPE + b] != 0);
        if (!used)
        {
            continue;
        }
        const unsigned long long first = rd64(e, E_FIRST);
        const unsigned long long last = rd64(e, E_LAST);
        if (last < first || first < firstusable
            || (imagesectors && last >= imagesectors))
        {
            *detail = (imagesectors && last >= imagesectors && last >= first)
                ? QObject::tr("partition %1 runs past the end of the image").arg(i + 1)
                : QObject::tr("partition %1 describes an impossible range").arg(i + 1);
            return false;
        }
        CombinePartition p;
        p.slot = (int)i;
        p.first = first;
        p.sectors = last - first + 1;
        QString name = QString::fromUtf16(reinterpret_cast<const char16_t *>(e + E_NAME), 36);
        const int nul = name.indexOf(QChar(0));
        if (nul >= 0) name.truncate(nul);
        p.name = name;
        p.entry = QByteArray((const char *)e, E_SIZE);
        p.gptType = p.entry.mid(E_TYPE, 16);
        const PartType *t = gptTypeFor(p.gptType);
        p.typeLabel = t ? QObject::tr(t->label) : gptGuidText(p.gptType);
        p.mbrType = t ? t->mbr : 0;
        parts.append(p);
    }
    if (parts.isEmpty())
    {
        *detail = QObject::tr("the GPT holds no partitions");
        return false;
    }
    std::sort(parts.begin(), parts.end(), [](const CombinePartition &a, const CombinePartition &b)
    {
        return a.first < b.first;
    });
    for (int i = 1; i < parts.size(); ++i)
    {
        if (parts[i].first < parts[i - 1].first + parts[i - 1].sectors)
        {
            *detail = QObject::tr("two partitions overlap");
            return false;
        }
    }

    out->table = COMBINE_TABLE_GPT;
    out->partitions = parts;
    out->tableEnd = tableend;
    out->firstUsable = firstusable;
    out->leadEnd = parts.first().first;
    out->entryLba = entrylba;
    out->numEntries = numentries;
    out->entrySize = entrysize;
    out->diskGuid = QByteArray((const char *)hdr + H_DISKGUID, 16);
    out->tableRegion = head.left((int)(tableend * ss));
    return true;
}

static bool parseMbr(const QByteArray &head, unsigned long long ss, unsigned long long imagesectors,
                     ImageLayout *out, QString *detail)
{
    const unsigned char *m = (const unsigned char *)head.constData();
    QList<CombinePartition> parts;
    int extended = 0;
    for (int i = 0; i < 4; ++i)
    {
        const unsigned char *e = m + M_TABLE + i * M_SIZE;
        const unsigned char type = e[M_TYPE];
        const unsigned long long start = rd32(e, M_START);
        const unsigned long long count = rd32(e, M_COUNT);
        if (type == 0 || count == 0)
        {
            continue;
        }
        if (start < 1 || (imagesectors && start + count > imagesectors))
        {
            *detail = (start >= 1)
                ? QObject::tr("partition %1 runs past the end of the image").arg(i + 1)
                : QObject::tr("partition %1 describes an impossible range").arg(i + 1);
            return false;
        }
        CombinePartition p;
        p.slot = i;
        p.first = start;
        p.sectors = count;
        p.entry = QByteArray((const char *)e, M_SIZE);
        p.mbrType = type;
        const PartType *t = mbrTypeFor(type);
        p.gptType = (t && t->gpt) ? guidBytes(t->gpt) : QByteArray();
        if (isExtended(type))
        {
            ++extended;
            p.typeLabel = QObject::tr("extended, with its logical partitions (0x%1)")
                              .arg(type, 2, 16, QChar('0')).toUpper();
        }
        else
        {
            p.typeLabel = t ? QString("%1 (0x%2)").arg(QObject::tr(t->label))
                                  .arg(QString::number(type, 16).rightJustified(2, '0').toUpper())
                            : QObject::tr("type 0x%1")
                                  .arg(QString::number(type, 16).rightJustified(2, '0').toUpper());
        }
        parts.append(p);
    }
    if (parts.isEmpty())
    {
        *detail = QObject::tr("the MBR holds no partitions");
        return false;
    }
    if (extended > 1)
    {
        *detail = QObject::tr("the MBR has more than one extended partition");
        return false;
    }
    std::sort(parts.begin(), parts.end(), [](const CombinePartition &a, const CombinePartition &b)
    {
        return a.first < b.first;
    });
    for (int i = 1; i < parts.size(); ++i)
    {
        if (parts[i].first < parts[i - 1].first + parts[i - 1].sectors)
        {
            *detail = QObject::tr("two partitions overlap");
            return false;
        }
    }
    out->table = COMBINE_TABLE_MBR;
    out->partitions = parts;
    out->tableEnd = 1;
    out->firstUsable = 1;
    out->leadEnd = parts.first().first;
    out->entryLba = out->numEntries = out->entrySize = 0;
    out->diskGuid.clear();
    out->tableRegion = head.left((int)ss);
    return true;
}

bool parseImageLayout(const QByteArray &head, unsigned long long sectorsize,
                      unsigned long long imagesectors, ImageLayout *layout,
                      unsigned long long *needsectors, QString *detail)
{
    QString why;
    unsigned long long need = 0;
    if (needsectors) *needsectors = 0;
    if (sectorsize < 512 || (unsigned long long)head.size() < sectorsize)
    {
        if (detail) *detail = QObject::tr("the image is smaller than one sector");
        return false;
    }
    ImageLayout out;
    out.table = COMBINE_TABLE_NONE;
    out.tableEnd = out.firstUsable = out.leadEnd = 0;
    out.entryLba = out.numEntries = out.entrySize = 0;
    out.wholeImage = false;
    const unsigned char *b = (const unsigned char *)head.constData();

    // GPT first: a GPT's sector 0 is an MBR too, protective or hybrid.
    const bool gptsig = (unsigned long long)head.size() >= 2 * sectorsize
                        && memcmp(b + sectorsize, "EFI PART", 8) == 0;
    bool protective = false;
    for (int i = 0; i < 4; ++i) protective = protective || (b[M_TABLE + i * M_SIZE + M_TYPE] == 0xEE);
    const bool bootsig = (b[510] == 0x55 && b[511] == 0xAA);
    if (gptsig)
    {
        if (!parseGpt(head, sectorsize, imagesectors, &out, &need, &why))
        {
            if (need && needsectors) *needsectors = need;
            if (detail) *detail = why;
            return false;
        }
    }
    else if (bootsig && protective)
    {
        if (detail) *detail = QObject::tr("the image has a protective MBR but no GPT header");
        return false;
    }
    else if (bootsig && !isVolumeBootRecord(b, head.size()))
    {
        // A real MBR's entries start with a status byte of 0x00 or 0x80. Any
        // other value there means this sector is something else -- boot
        // code with a stray signature -- and is taken for no table.
        // All four slots empty is no table either: boot code or a raw
        // bootloader that happens to end in the signature.
        bool plausible = true, anyentry = false;
        for (int i = 0; i < 4 && plausible; ++i)
        {
            const unsigned char *e = b + M_TABLE + i * M_SIZE;
            plausible = (e[M_STATUS] == 0x00 || e[M_STATUS] == 0x80);
            anyentry = anyentry || (e[M_TYPE] != 0 && rd32(e, M_COUNT) != 0);
        }
        if (plausible && anyentry)
        {
            if (!parseMbr(head, sectorsize, imagesectors, &out, &why))
            {
                if (detail) *detail = why;
                return false;
            }
        }
    }

    if (out.table == COMBINE_TABLE_NONE)
    {
        // No partition table: the image is one partition, a filesystem image.
        CombinePartition p;
        p.slot = -1;
        p.first = 0;
        p.sectors = imagesectors;
        guessFilesystem(head, sectorsize, &p);
        out.partitions.append(p);
        out.wholeImage = true;
    }
    *layout = out;
    return true;
}

// ------------------------------------------------------------------- plan ---

// The type a chosen partition has on a table of kind `table`, or false, with
// *detail, when that table cannot express it.
static bool typeOnTable(const ImageLayout &img, const CombinePartition &p, CombineTable table,
                        unsigned char *mbrtype, QByteArray *gpttype, QString *detail)
{
    if (table == COMBINE_TABLE_GPT)
    {
        if (img.table == COMBINE_TABLE_GPT)
        {
            *gpttype = p.entry.mid(E_TYPE, 16);
            return true;
        }
        if (img.table == COMBINE_TABLE_MBR && isExtended(p.mbrType))
        {
            *detail = QObject::tr("an extended MBR partition cannot go on a GPT: choose the "
                                  "logical partitions' image as the lead-in, or leave it out");
            return false;
        }
        if (p.gptType.size() != 16)
        {
            *detail = QObject::tr("MBR partition type 0x%1 has no GPT equivalent this program knows")
                          .arg(QString::number(p.mbrType, 16).rightJustified(2, '0').toUpper());
            return false;
        }
        *gpttype = p.gptType;
        return true;
    }
    // MBR
    if (img.table == COMBINE_TABLE_GPT)
    {
        if (p.mbrType == 0)
        {
            *detail = QObject::tr("GPT partition type %1 has no MBR equivalent")
                          .arg(p.typeLabel);
            return false;
        }
    }
    *mbrtype = p.mbrType;
    return true;
}

bool planCombine(const QList<ImageLayout> &images, const QList<CombineChoice> &choices,
                 int leadimage, unsigned long long ss,
                 unsigned long long devicesectors, unsigned long long alignsectors,
                 bool newguids, CombinePlan *plan, QString *detail)
{
    auto fail = [&](const QString &why) { if (detail) *detail = why; return false; };
    if (ss < 512 || alignsectors == 0)
    {
        return fail(QObject::tr("the device geometry is not usable"));
    }
    if (choices.isEmpty())
    {
        return fail(QObject::tr("no partitions are chosen"));
    }
    QSet<QPair<int, int>> seen;
    for (const CombineChoice &c : choices)
    {
        if (c.image < 0 || c.image >= images.size() || c.partition < 0
            || c.partition >= images[c.image].partitions.size())
        {
            return fail(QObject::tr("a chosen partition does not exist"));
        }
        if (seen.contains(qMakePair(c.image, c.partition)))
        {
            return fail(QObject::tr("a partition is chosen twice"));
        }
        seen.insert(qMakePair(c.image, c.partition));
        if (images[c.image].partitions[c.partition].sectors == 0)
        {
            return fail(QObject::tr("the size of an image with no partition table is not "
                                    "known: scan it first"));
        }
    }
    const ImageLayout *lead = NULL;
    if (leadimage >= 0)
    {
        if (leadimage >= images.size() || images[leadimage].table == COMBINE_TABLE_NONE)
        {
            return fail(QObject::tr("the lead-in image has no partition table"));
        }
        lead = &images[leadimage];
    }

    // The kind of table.
    CombineTable table;
    if (lead)
    {
        table = lead->table;
    }
    else
    {
        bool allmbr = choices.size() <= 4;
        for (const CombineChoice &c : choices)
        {
            allmbr = allmbr && images[c.image].table != COMBINE_TABLE_GPT;
        }
        table = allmbr ? COMBINE_TABLE_MBR : COMBINE_TABLE_GPT;
    }
    if (table == COMBINE_TABLE_MBR && choices.size() > 4)
    {
        return fail(QObject::tr("an MBR holds at most four partitions, and %1 are chosen")
                        .arg(choices.size()));
    }

    // Each choice's type on that table, before anything is laid out.
    QList<unsigned char> mbrtypes;
    QList<QByteArray> gpttypes;
    int extendedcount = 0;
    for (const CombineChoice &c : choices)
    {
        const ImageLayout &img = images[c.image];
        const CombinePartition &p = img.partitions[c.partition];
        unsigned char mt = 0;
        QByteArray gt;
        QString why;
        if (!typeOnTable(img, p, table, &mt, &gt, &why))
        {
            return fail(why);
        }
        if (table == COMBINE_TABLE_MBR && isExtended(mt) && ++extendedcount > 1)
        {
            return fail(QObject::tr("an MBR can hold only one extended partition"));
        }
        mbrtypes.append(mt);
        gpttypes.append(gt);
    }

    // The table's geometry: the lead-in image's, so its lead-in stays where
    // it was; otherwise the usual 128 entries at LBA 2.
    unsigned long long entrylba = 0, numentries = 0, entrysize = 0, entrysectors = 0;
    unsigned long long tableend, firstusable;
    if (table == COMBINE_TABLE_GPT)
    {
        entrylba = lead ? lead->entryLba : 2;
        numentries = lead ? lead->numEntries : 128;
        entrysize = lead ? lead->entrySize : E_SIZE;
        entrysectors = (numentries * entrysize + ss - 1) / ss;
        tableend = entrylba + entrysectors;
        firstusable = lead ? lead->firstUsable : tableend;
        if ((unsigned long long)choices.size() > numentries)
        {
            return fail(QObject::tr("the GPT has room for %1 partitions, and %2 are chosen")
                            .arg(numentries).arg(choices.size()));
        }
    }
    else
    {
        tableend = 1;
        firstusable = 1;
    }

    // The lead-in, and where packing starts.
    QList<CombineRange> ranges;
    unsigned long long cursor;
    if (lead)
    {
        const unsigned long long keepend = leadingKeepEnd(lead->tableEnd, lead->leadEnd,
                                                          lead->firstUsable, ss, alignsectors);
        if (keepend > lead->tableEnd)
        {
            ranges.append(CombineRange{leadimage, lead->tableEnd, lead->tableEnd,
                                       keepend - lead->tableEnd});
        }
        cursor = keepend;
    }
    else
    {
        cursor = alignUp(qMax(tableend, firstusable), alignsectors);
    }

    // The partitions, in the order chosen.
    QList<CombinePlaced> placed;
    for (int i = 0; i < choices.size(); ++i)
    {
        const CombineChoice &c = choices[i];
        const CombinePartition &p = images[c.image].partitions[c.partition];
        // With a lead-in the first goes right where it ends, aligned or not,
        // as its own image had it; the rest on alignsectors boundaries.
        const unsigned long long first = (i == 0 && lead) ? cursor : alignUp(cursor, alignsectors);
        if (table == COMBINE_TABLE_MBR && (first > 0xFFFFFFFFull || p.sectors > 0xFFFFFFFFull))
        {
            return fail(QObject::tr("the layout no longer fits a 32-bit MBR entry"));
        }
        ranges.append(CombineRange{c.image, p.first, first, p.sectors});
        placed.append(CombinePlaced{c.image, c.partition, i, first, p.sectors});
        cursor = first + p.sectors;
    }

    // Does it fit? A GPT keeps its backup entry array and header at the end.
    // For an image file there is no device: it is made exactly big enough.
    const unsigned long long reserved = (table == COMBINE_TABLE_GPT) ? entrysectors + 1 : 0;
    if (devicesectors == 0)
    {
        devicesectors = cursor + reserved;
    }
    if (devicesectors <= reserved || cursor > devicesectors - reserved)
    {
        return fail(QObject::tr("the partitions need %1 MB and the device has %2 MB")
                        .arg((cursor + reserved) * ss / 1000000ull)
                        .arg(devicesectors * ss / 1000000ull));
    }

    // Duplicate unique GUIDs among the chosen GPT partitions.
    QStringList duplicates;
    QList<QByteArray> uniques;
    {
        QSet<QByteArray> have, reported;
        for (const CombineChoice &c : choices)
        {
            const ImageLayout &img = images[c.image];
            QByteArray u;
            if (img.table == COMBINE_TABLE_GPT)
            {
                u = img.partitions[c.partition].entry.mid(E_UNIQUE, 16);
                if (have.contains(u))
                {
                    if (!reported.contains(u))
                    {
                        duplicates.append(gptGuidText(u));
                        reported.insert(u);
                    }
                    if (newguids)
                    {
                        u = newGuid();
                    }
                }
                have.insert(u);
            }
            uniques.append(u);
        }
    }

    CombinePlan out;
    out.table = table;
    out.headersectors = tableend;
    out.usedsectors = cursor;
    out.totalsectors = devicesectors;
    out.placed = placed;
    out.duplicateGuids = newguids ? QStringList() : duplicates;
    out.ranges = ranges;
    out.backupfirst = 0;

    if (table == COMBINE_TABLE_MBR)
    {
        QByteArray region(lead ? lead->tableRegion.left((int)ss) : QByteArray((int)ss, 0));
        region.resize((int)ss);
        unsigned char *m = (unsigned char *)region.data();
        if (!lead)
        {
            // A disk signature of its own; Linux names MBR partitions by it
            // (PARTUUID=<signature>-<number>).
            const QByteArray g = newGuid();
            memcpy(m + 440, g.constData(), 4);
        }
        memset(m + M_TABLE, 0, 4 * M_SIZE);
        for (int i = 0; i < choices.size(); ++i)
        {
            const ImageLayout &img = images[choices[i].image];
            const CombinePartition &p = img.partitions[choices[i].partition];
            unsigned char *e = m + M_TABLE + i * M_SIZE;
            bool boot = false;
            if (img.table == COMBINE_TABLE_MBR)
            {
                boot = ((unsigned char)p.entry[M_STATUS] == 0x80);
            }
            else if (img.table == COMBINE_TABLE_GPT)
            {
                boot = (rd64((const unsigned char *)p.entry.constData(), E_ATTRS) & GPT_ATTR_LEGACY_BOOT) != 0;
            }
            e[M_STATUS] = boot ? 0x80 : 0x00;
            // CHS "beyond the geometry" markers: only the LBA fields count.
            e[1] = 0xFE; e[2] = 0xFF; e[3] = 0xFF;
            e[M_TYPE] = mbrtypes[i];
            e[5] = 0xFE; e[6] = 0xFF; e[7] = 0xFF;
            wr32(e, M_START, placed[i].first);
            wr32(e, M_COUNT, placed[i].sectors);
        }
        m[510] = 0x55;
        m[511] = 0xAA;
        out.headerregion = region;
        out.backupregion.clear();
    }
    else
    {
        const unsigned long long lastlba = devicesectors - 1;
        const unsigned long long backupentries = lastlba - entrysectors;
        const unsigned long long lastusable = backupentries - 1;

        QByteArray region = lead ? lead->tableRegion : QByteArray();
        region.resize((int)(tableend * ss));
        if (!lead)
        {
            region.fill(0);
        }
        unsigned char *r = (unsigned char *)region.data();

        // Protective MBR: the lead-in's boot code and disk signature, one 0xEE
        // entry over the whole device. Any hybrid entries the lead-in's MBR
        // had described its own layout and are not kept.
        memset(r + M_TABLE, 0, 4 * M_SIZE);
        unsigned char *pe = r + M_TABLE;
        pe[1] = 0x00; pe[2] = 0x02; pe[3] = 0x00;
        pe[M_TYPE] = 0xEE;
        pe[5] = 0xFF; pe[6] = 0xFF; pe[7] = 0xFF;
        wr32(pe, M_START, 1);
        wr32(pe, M_COUNT, (lastlba > 0xFFFFFFFFull) ? 0xFFFFFFFFull : lastlba);
        r[510] = 0x55;
        r[511] = 0xAA;

        // Entries, in device order.
        QByteArray entries((int)(entrysectors * ss), 0);
        for (int i = 0; i < choices.size(); ++i)
        {
            const ImageLayout &img = images[choices[i].image];
            const CombinePartition &p = img.partitions[choices[i].partition];
            unsigned char *e = (unsigned char *)entries.data() + i * entrysize;
            if (img.table == COMBINE_TABLE_GPT)
            {
                memcpy(e, p.entry.constData(), E_SIZE);
                memcpy(e + E_UNIQUE, uniques[i].constData(), 16);
            }
            else
            {
                memcpy(e + E_TYPE, gpttypes[i].constData(), 16);
                memcpy(e + E_UNIQUE, newGuid().constData(), 16);
                const bool boot = img.table == COMBINE_TABLE_MBR
                                  && (unsigned char)p.entry[M_STATUS] == 0x80;
                wr64(e, E_ATTRS, boot ? GPT_ATTR_LEGACY_BOOT : 0);
            }
            wr64(e, E_FIRST, placed[i].first);
            wr64(e, E_LAST, placed[i].first + placed[i].sectors - 1);
        }
        const DWORD entriescrc = gptCrc32((const unsigned char *)entries.constData(),
                                          (size_t)(numentries * entrysize));

        // Primary header, from scratch: nothing of an image's header but its
        // disk GUID and entry geometry applies to a new device.
        QByteArray header((int)ss, 0);
        unsigned char *h = (unsigned char *)header.data();
        memcpy(h, "EFI PART", 8);
        wr32(h, 8, 0x00010000);
        wr32(h, H_HEADERSIZE, H_SIZE);
        wr64(h, H_MYLBA, 1);
        wr64(h, H_ALTLBA, lastlba);
        wr64(h, H_FIRSTUSABLE, firstusable);
        wr64(h, H_LASTUSABLE, lastusable);
        const QByteArray diskguid = (lead && lead->diskGuid.size() == 16) ? lead->diskGuid : newGuid();
        memcpy(h + H_DISKGUID, diskguid.constData(), 16);
        wr64(h, H_ENTRYLBA, entrylba);
        wr32(h, H_NUMENTRIES, numentries);
        wr32(h, H_ENTRYSIZE, entrysize);
        wr32(h, H_ENTRIESCRC, entriescrc);
        wr32(h, H_HEADERCRC, gptCrc32(h, H_SIZE));

        memcpy(r + ss, h, (size_t)ss);
        memcpy(r + entrylba * ss, entries.constData(), (size_t)entries.size());

        // Backup: the same entries, then the header with MyLBA and
        // AlternateLBA swapped, at the end of the device.
        QByteArray backup = header;
        unsigned char *bh = (unsigned char *)backup.data();
        wr64(bh, H_MYLBA, lastlba);
        wr64(bh, H_ALTLBA, 1);
        wr64(bh, H_ENTRYLBA, backupentries);
        wr32(bh, H_HEADERCRC, 0);
        wr32(bh, H_HEADERCRC, gptCrc32(bh, H_SIZE));

        out.headerregion = region;
        out.backupregion = entries + backup;
        out.backupfirst = backupentries;
    }
    *plan = out;
    return true;
}
