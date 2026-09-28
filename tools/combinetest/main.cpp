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

// Exercises combine.cpp: parseImageLayout() on GPT, MBR and table-less
// images built here, and planCombine() on choices from them. Each plan is
// applied to a buffer standing in for the device exactly as the writer
// applies it, and the result is read back through disk.cpp -- the partition
// listing, gptPrimaryState(), relocateBackupGPT() -- and compared sector by
// sector with the images it came from. The table-reading side is disk.cpp's,
// so a mistake in combine.cpp's own GPT code cannot agree with itself and
// pass.
//
// Run with tools/combinetest.sh; exits non-zero if any check fails.

#include <QApplication>
#include <QByteArray>
#include <QComboBox>
#include <QListWidget>
#include <QRadioButton>
#include <QToolTip>
#include <QTreeWidget>
#include <QString>
#include <QUuid>
#include <windows.h>
#include <cstdio>
#include <cstring>
#include <zlib.h>
#include "combine.h"
#include "combinedialog.h"
#include "combinereader.h"
#include "imagesource.h"
#include "disk.h"
#include "mainwindow.h"

// disk.cpp parents its message boxes on this; nothing here opens a window.
MainWindow *MainWindow::instance = NULL;

static const unsigned long long SEC = 512;
static const unsigned long long ALIGN = 2048;      // 1 MiB

static int failures = 0;
static int checks = 0;
static void check(bool ok, const char *what)
{
    printf("  %s %s\n", ok ? "ok  " : "FAIL", what);
    ++checks;
    if (!ok) ++failures;
}

static unsigned long long rd64(const unsigned char *p, int o)
{
    unsigned long long v = 0;
    for (int i = 7; i >= 0; --i) v = (v << 8) | p[o + i];
    return v;
}
static unsigned int rd32(const unsigned char *p, int o)
{
    return (unsigned int)p[o] | ((unsigned int)p[o + 1] << 8)
         | ((unsigned int)p[o + 2] << 16) | ((unsigned int)p[o + 3] << 24);
}
static void wr64(unsigned char *p, int o, unsigned long long v)
{
    for (int i = 0; i < 8; ++i) p[o + i] = (unsigned char)((v >> (8 * i)) & 0xFF);
}
static void wr32(unsigned char *p, int o, unsigned int v)
{
    for (int i = 0; i < 4; ++i) p[o + i] = (unsigned char)((v >> (8 * i)) & 0xFF);
}

// GPT GUIDs, in the byte order a GPT stores them.
static QByteArray guid(const char *text)
{
    QByteArray r = QUuid::fromString(QLatin1String(text)).toRfc4122();
    QByteArray g(16, 0);
    for (int i = 0; i < 4; ++i) g[i] = r[3 - i];
    g[4] = r[5]; g[5] = r[4]; g[6] = r[7]; g[7] = r[6];
    for (int i = 8; i < 16; ++i) g[i] = r[i];
    return g;
}
static const char *ESP   = "C12A7328-F81F-11D2-BA4B-00A0C93EC93B";
static const char *LINUX = "0FC63DAF-8483-4772-8E79-3D69D8477DE4";
static const char *BASIC = "EBD0A0A2-B9E5-4433-87C0-68B6B72699C7";
static const char *BIOSBOOT = "21686148-6449-6E6F-744E-656564454649";

// Every sector of a partition, and of a lead-in, carries its own tag -- a
// byte naming what it belongs to, then its sector number within it -- so a
// sector landing anywhere but where it should is caught, not just a
// partition's first.
static void tagSectors(QByteArray &img, unsigned long long first, unsigned long long count, char tag)
{
    for (unsigned long long s = 0; s < count; ++s)
    {
        unsigned char *p = (unsigned char *)img.data() + (first + s) * SEC;
        memset(p, tag, SEC);
        wr64(p, 8, s);
    }
}

struct GptPart
{
    unsigned long long first, last;
    const char *type;
    const char *unique;
    const char *name;
    char tag;
    unsigned long long attrs;
};

// A GPT image of `sectors` sectors: the table at LBA 2, FirstUsableLBA
// firstusable, the partitions tagged, the lead-in [34, first partition)
// tagged 'L', boot code in sector 0 tagged 'B', and the backup at the end.
static QByteArray gptImage(unsigned long long sectors, unsigned long long firstusable,
                           const QList<GptPart> &parts, const char *diskguid)
{
    QByteArray img((int)(sectors * SEC), 0);
    unsigned char *b = (unsigned char *)img.data();
    memset(b, 'B', 440);
    b[450] = 0xEE;
    wr32(b + 446, 8, 1);
    wr32(b + 446, 12, (unsigned int)(sectors - 1));
    b[510] = 0x55; b[511] = 0xAA;
    unsigned long long firstpart = sectors;
    for (const GptPart &p : parts) firstpart = qMin(firstpart, p.first);
    if (firstpart > 34) tagSectors(img, 34, firstpart - 34, 'L');

    QByteArray entries(128 * 128, 0);
    for (int i = 0; i < parts.size(); ++i)
    {
        unsigned char *e = (unsigned char *)entries.data() + i * 128;
        memcpy(e, guid(parts[i].type).constData(), 16);
        memcpy(e + 16, guid(parts[i].unique).constData(), 16);
        wr64(e, 32, parts[i].first);
        wr64(e, 40, parts[i].last);
        wr64(e, 48, parts[i].attrs);
        const QString n = QString::fromLatin1(parts[i].name);
        for (int c = 0; c < n.size() && c < 36; ++c)
        {
            e[56 + 2 * c] = (unsigned char)n[c].unicode();
        }
        tagSectors(img, parts[i].first, parts[i].last - parts[i].first + 1, parts[i].tag);
    }
    const unsigned long long last = sectors - 1;
    auto header = [&](unsigned long long my, unsigned long long alt, unsigned long long elba) {
        QByteArray h((int)SEC, 0);
        unsigned char *p = (unsigned char *)h.data();
        memcpy(p, "EFI PART", 8);
        wr32(p, 8, 0x00010000);
        wr32(p, 12, 92);
        wr64(p, 24, my);
        wr64(p, 32, alt);
        wr64(p, 40, firstusable);
        wr64(p, 48, last - 33);
        memcpy(p + 56, guid(diskguid).constData(), 16);
        wr64(p, 72, elba);
        wr32(p, 80, 128);
        wr32(p, 84, 128);
        wr32(p, 88, gptCrc32((const unsigned char *)entries.constData(), (size_t)entries.size()));
        wr32(p, 16, gptCrc32(p, 92));
        return h;
    };
    memcpy(b + SEC, header(1, last, 2).constData(), SEC);
    memcpy(b + 2 * SEC, entries.constData(), (size_t)entries.size());
    memcpy(b + (last - 32) * SEC, entries.constData(), (size_t)entries.size());
    memcpy(b + last * SEC, header(last, 1, last - 32).constData(), SEC);
    return img;
}

struct MbrPart
{
    int slot;
    unsigned long long first, count;
    unsigned char type;
    bool boot;
    char tag;
};

static QByteArray mbrImage(unsigned long long sectors, const QList<MbrPart> &parts,
                           unsigned int signature)
{
    QByteArray img((int)(sectors * SEC), 0);
    unsigned char *b = (unsigned char *)img.data();
    memset(b, 'B', 440);
    wr32(b, 440, signature);
    unsigned long long firstpart = sectors;
    for (const MbrPart &p : parts)
    {
        unsigned char *e = b + 446 + p.slot * 16;
        e[0] = p.boot ? 0x80 : 0x00;
        e[4] = p.type;
        wr32(e, 8, (unsigned int)p.first);
        wr32(e, 12, (unsigned int)p.count);
        tagSectors(img, p.first, p.count, p.tag);
        firstpart = qMin(firstpart, p.first);
    }
    if (firstpart > 1) tagSectors(img, 1, firstpart - 1, 'L');
    b[510] = 0x55; b[511] = 0xAA;
    return img;
}

// A FAT32 filesystem image with no partition table: its boot sector ends in
// the same 0x55AA an MBR does.
static QByteArray fat32Image(unsigned long long sectors, char tag)
{
    QByteArray img((int)(sectors * SEC), 0);
    tagSectors(img, 0, sectors, tag);
    unsigned char *b = (unsigned char *)img.data();
    memset(b, 0, SEC);
    b[0] = 0xEB; b[1] = 0x58; b[2] = 0x90;
    memcpy(b + 3, "MSDOS5.0", 8);
    memcpy(b + 82, "FAT32   ", 8);
    b[510] = 0x55; b[511] = 0xAA;
    return img;
}

static QByteArray ext4Image(unsigned long long sectors, char tag)
{
    QByteArray img((int)(sectors * SEC), 0);
    tagSectors(img, 0, sectors, tag);
    unsigned char *b = (unsigned char *)img.data();
    memset(b, 0, 4 * SEC);
    b[1024 + 56] = 0x53;
    b[1024 + 57] = 0xEF;
    return img;
}

// parseImageLayout() on the first MiB, as the dialog reads it.
static bool layoutOf(const QByteArray &img, ImageLayout *l, bool sizeknown = true)
{
    unsigned long long need = 0;
    QString why;
    const bool ok = parseImageLayout(img.left(2048 * SEC), SEC,
                                     sizeknown ? (unsigned long long)img.size() / SEC : 0ull,
                                     l, &need, &why);
    if (!ok) printf("    (parse: %s, need %llu)\n", why.toLocal8Bit().constData(), need);
    return ok;
}

// The device the plan describes, built the way the writer builds it: the
// table, each range from its image, the backup at the end.
static QByteArray apply(const CombinePlan &plan, const QList<QByteArray> &imgs,
                        unsigned long long devicesectors)
{
    QByteArray dev((int)(devicesectors * SEC), 0);
    for (const CombineRange &r : plan.ranges)
    {
        memcpy(dev.data() + r.dstfirst * SEC, imgs[r.image].constData() + r.srcfirst * SEC,
               (size_t)(r.length * SEC));
    }
    if (!plan.backupregion.isEmpty())
    {
        memcpy(dev.data() + plan.backupfirst * SEC, plan.backupregion.constData(),
               (size_t)plan.backupregion.size());
    }
    memcpy(dev.data(), plan.headerregion.constData(), (size_t)plan.headerregion.size());
    return dev;
}

static const char *TESTFILE = "combinetest.img";

static HANDLE openAsDevice(const QByteArray &bytes)
{
    HANDLE h = CreateFileA(TESTFILE, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                           FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return h;
    DWORD put = 0;
    WriteFile(h, bytes.constData(), (DWORD)bytes.size(), &put, NULL);
    return h;
}

// Every sector of dst range [dstfirst, +count) equals the image's
// [srcfirst, +count).
static bool sameSectors(const QByteArray &dev, unsigned long long dstfirst, const QByteArray &img,
                        unsigned long long srcfirst, unsigned long long count)
{
    return memcmp(dev.constData() + dstfirst * SEC, img.constData() + srcfirst * SEC,
                  (size_t)(count * SEC)) == 0;
}

// The device's GPT entry for device slot `slot`.
static const unsigned char *gptEntry(const QByteArray &dev, int slot)
{
    const unsigned char *h = (const unsigned char *)dev.constData() + SEC;
    return (const unsigned char *)dev.constData() + rd64(h, 72) * SEC + slot * rd32(h, 84);
}

// The backup GPT where every tool expects it: its header in the last
// sector, pointing back at LBA 1, its entry array right below it, identical
// to the primary's, and LastUsableLBA just below that.
static bool backupAtEnd(const QByteArray &dev, unsigned long long device)
{
    const unsigned char *p = (const unsigned char *)dev.constData() + SEC;
    const unsigned char *b = (const unsigned char *)dev.constData() + (device - 1) * SEC;
    const unsigned long long esec = (rd32(p, 80) * rd32(p, 84) + SEC - 1) / SEC;
    QByteArray probe((const char *)b, 92);
    wr32((unsigned char *)probe.data(), 16, 0);
    return memcmp(b, "EFI PART", 8) == 0
        && gptCrc32((const unsigned char *)probe.constData(), 92) == rd32(b, 16)
        && rd64(b, 24) == device - 1 && rd64(b, 32) == 1
        && rd64(b, 72) == device - 1 - esec
        && rd64(p, 32) == device - 1 && rd64(p, 48) == device - 2 - esec
        && memcmp(dev.constData() + rd64(p, 72) * SEC,
                  dev.constData() + (device - 1 - esec) * SEC, (size_t)(esec * SEC)) == 0;
}

// ------------------------------------------------------------------ cases ---

static void caseParse()
{
    printf("parsing\n");
    const QByteArray g = gptImage(8192, 34, {
        {2048, 4095, ESP, "11111111-1111-1111-1111-111111111111", "boot", 'a', 0},
        {4096, 6143, LINUX, "22222222-2222-2222-2222-222222222222", "root", 'b', 0} },
        "AAAAAAAA-AAAA-AAAA-AAAA-AAAAAAAAAAAA");
    ImageLayout l;
    check(layoutOf(g, &l), "a GPT image parses");
    check(l.table == COMBINE_TABLE_GPT && l.partitions.size() == 2, "  as a GPT with two partitions");
    check(l.partitions[0].first == 2048 && l.partitions[0].sectors == 2048
          && l.partitions[1].first == 4096, "  at the right places");
    check(l.partitions[0].name == "boot" && l.partitions[0].typeLabel == "EFI system",
          "  with its names and types");
    check(l.tableEnd == 34 && l.leadEnd == 2048, "  table end 34, lead-in to 2048");

    unsigned long long need = 0;
    QString why;
    ImageLayout l2;
    check(!parseImageLayout(g.left(2 * SEC), SEC, 8192, &l2, &need, &why) && need == 34,
          "two sectors are not enough: it asks for 34");

    QByteArray bad = g;
    bad[2 * SEC + 40] = (char)(bad[2 * SEC + 40] ^ 0x01);
    check(!layoutOf(bad, &l2), "a damaged entry array is refused");

    check(!parseImageLayout(g.left(2048 * SEC), SEC, 5000, &l2, &need, &why),
          "a partition past the end of the image is refused");

    const QByteArray m = mbrImage(8192, {
        {0, 2048, 2048, 0x0C, true, 'c'}, {1, 4096, 2048, 0x83, false, 'd'} }, 0x12345678);
    check(layoutOf(m, &l) && l.table == COMBINE_TABLE_MBR && l.partitions.size() == 2,
          "an MBR image parses, with two partitions");
    check(l.partitions[0].typeLabel.startsWith("FAT32"), "  the first named FAT32");

    const QByteArray f = fat32Image(4096, 'f');
    check(layoutOf(f, &l) && l.table == COMBINE_TABLE_NONE && l.wholeImage
          && l.partitions.size() == 1 && l.partitions[0].sectors == 4096
          && l.partitions[0].mbrType == 0x0C,
          "a FAT32 image with no table is one FAT32 partition, not an MBR");
    check(layoutOf(f, &l, false) && l.partitions[0].sectors == 0,
          "  of unknown size when the image does not record it");
    const QByteArray e = ext4Image(4096, 'e');
    check(layoutOf(e, &l) && l.wholeImage && l.partitions[0].mbrType == 0x83
          && l.partitions[0].typeLabel.startsWith("ext"),
          "an ext4 image with no table is one Linux partition");

    QByteArray prot = g;
    memset(prot.data() + SEC, 0, SEC);
    check(!layoutOf(prot, &l2), "a protective MBR without its GPT is refused");
}

static void caseTwoGpt()
{
    printf("two GPT images, partitions reordered, no lead-in\n");
    const QByteArray a = gptImage(10000, 34, {
        {2048, 4095, ESP, "11111111-1111-1111-1111-111111111111", "a-boot", 'a', 0},
        {4096, 7000, LINUX, "22222222-2222-2222-2222-222222222222", "a-root", 'b', 0} },
        "AAAAAAAA-AAAA-AAAA-AAAA-AAAAAAAAAAAA");
    const QByteArray b = gptImage(9000, 34, {
        {2048, 5000, LINUX, "33333333-3333-3333-3333-333333333333", "b-data", 'c', 1ull << 60} },
        "BBBBBBBB-BBBB-BBBB-BBBB-BBBBBBBBBBBB");
    QList<ImageLayout> ls(2);
    layoutOf(a, &ls[0]);
    layoutOf(b, &ls[1]);
    const unsigned long long device = 40000;
    CombinePlan plan;
    QString why;
    // Device order: A's root, B's data, A's boot.
    const bool ok = planCombine(ls, { {0, 1}, {1, 0}, {0, 0} }, -1, SEC, device, ALIGN,
                                false, &plan, &why);
    check(ok && plan.table == COMBINE_TABLE_GPT, "planned, as a GPT");
    if (!ok) { printf("    (%s)\n", why.toLocal8Bit().constData()); return; }
    check(plan.placed.size() == 3 && plan.placed[0].first == 2048
          && plan.placed[1].first % ALIGN == 0 && plan.placed[2].first % ALIGN == 0,
          "each partition on a 1 MiB boundary");
    check(plan.placed[1].first >= plan.placed[0].first + plan.placed[0].sectors
          && plan.placed[2].first >= plan.placed[1].first + plan.placed[1].sectors,
          "in the order chosen, none overlapping");
    const QByteArray dev = apply(plan, {a, b}, device);
    check(sameSectors(dev, plan.placed[0].first, a, 4096, 2905), "A's root copied whole");
    check(sameSectors(dev, plan.placed[1].first, b, 2048, 2953), "B's data copied whole");
    check(sameSectors(dev, plan.placed[2].first, a, 2048, 2048), "A's boot copied whole");

    HANDLE h = openAsDevice(dev);
    QList<PartitionInfo> found;
    QString d;
    check(listGptPartitions(h, SEC, device, &found, &d) && found.size() == 3,
          "disk.cpp reads the new GPT: three partitions");
    check(found.size() == 3 && found[0].name == "a-root" && found[1].name == "b-data"
          && found[2].name == "a-boot", "  named and ordered as chosen");
    check(gptPrimaryState(h, SEC, device) == GPT_PRIMARY_OK, "  the primary table checks out");
    check(relocateBackupGPT(h, SEC, device, &d) == GPT_FIX_NOT_NEEDED,
          "  and the backup is already at the end: nothing for Windows to repair");
    CloseHandle(h);
    check(backupAtEnd(dev, device), "the backup header and entries are the last 33 sectors");

    check(memcmp(gptEntry(dev, 1) + 16, guid("33333333-3333-3333-3333-333333333333").constData(), 16) == 0
          && rd64(gptEntry(dev, 1), 48) == (1ull << 60),
          "a partition keeps its unique GUID and attributes");
    check(memcmp(gptEntry(dev, 1), guid(LINUX).constData(), 16) == 0
          && memcmp(gptEntry(dev, 2), guid(ESP).constData(), 16) == 0, "  and its type");
    check(plan.duplicateGuids.isEmpty(), "no GUID collisions reported");
    const unsigned char *s0 = (const unsigned char *)dev.constData();
    check(s0[450] == 0xEE && rd32(s0 + 446, 8) == 1 && rd32(s0 + 446, 12) == device - 1,
          "the protective MBR spans the device");
}

static void caseLeadIn()
{
    printf("lead-in from a board image\n");
    // A board image: FirstUsableLBA 64, a bootloader from 64 to 32767, its
    // first partition at 32768 (16 MiB).
    const QByteArray a = gptImage(60000, 64, {
        {32768, 40959, BASIC, "44444444-4444-4444-4444-444444444444", "a-boot", 'a', 0},
        {40960, 50000, LINUX, "55555555-5555-5555-5555-555555555555", "a-root", 'b', 0} },
        "AAAAAAAA-AAAA-AAAA-AAAA-AAAAAAAAAAAA");
    const QByteArray b = gptImage(9000, 34, {
        {2048, 5000, LINUX, "66666666-6666-6666-6666-666666666666", "b-root", 'c', 0} },
        "BBBBBBBB-BBBB-BBBB-BBBB-BBBBBBBBBBBB");
    QList<ImageLayout> ls(2);
    layoutOf(a, &ls[0]);
    layoutOf(b, &ls[1]);
    const unsigned long long device = 100000;
    CombinePlan plan;
    QString why;
    const bool ok = planCombine(ls, { {0, 0}, {1, 0} }, 0, SEC, device, ALIGN, false, &plan, &why);
    check(ok, "planned");
    if (!ok) { printf("    (%s)\n", why.toLocal8Bit().constData()); return; }
    check(plan.placed[0].first == 32768, "the first partition stays where the lead-in's image had it");
    const QByteArray dev = apply(plan, {a, b}, device);
    check(sameSectors(dev, 34, a, 34, 32768 - 34), "the lead-in (bootloader) is copied where it was");
    check(sameSectors(dev, 0, a, 0, 1) == false
          && memcmp(dev.constData(), a.constData(), 440) == 0,
          "sector 0 keeps the lead-in's boot code");
    const unsigned char *h = (const unsigned char *)dev.constData() + SEC;
    check(rd64(h, 40) == 64, "FirstUsableLBA is the lead-in's, 64");
    check(memcmp(h + 56, guid("AAAAAAAA-AAAA-AAAA-AAAA-AAAAAAAAAAAA").constData(), 16) == 0,
          "the disk GUID is the lead-in's");
    HANDLE hd = openAsDevice(dev);
    check(gptPrimaryState(hd, SEC, device) == GPT_PRIMARY_OK, "the table checks out");
    check(gptRewriteRisk(hd, SEC) != GPT_RISK_UNKNOWN, "  and gptRewriteRisk() can read it");
    CloseHandle(hd);
    check(backupAtEnd(dev, device), "  its backup at the end of the device");
    check(sameSectors(dev, plan.placed[1].first, b, 2048, 2953), "B's root follows it");

    // Over 32 MiB before the first partition: only 32 MiB of it is kept.
    const QByteArray big = gptImage(150000, 34, {
        {100000, 110000, LINUX, "77777777-7777-7777-7777-777777777777", "big", 'd', 0} },
        "CCCCCCCC-CCCC-CCCC-CCCC-CCCCCCCCCCCC");
    QList<ImageLayout> lb(1);
    layoutOf(big, &lb[0]);
    const bool ok2 = planCombine(lb, { {0, 0} }, 0, SEC, 200000, ALIGN, false, &plan, &why);
    const unsigned long long keep = ((34 + 65536 + ALIGN - 1) / ALIGN) * ALIGN;
    check(ok2 && plan.placed[0].first == keep,
          "more than 32 MiB of lead-in: 32 MiB kept, the partition moved up to it");
}

static void caseMbr()
{
    printf("MBR images\n");
    const QByteArray a = mbrImage(8192, {
        {0, 2048, 2048, 0x0C, true, 'a'}, {1, 4096, 3000, 0x83, false, 'b'} }, 0x11112222);
    const QByteArray b = mbrImage(8192, { {0, 2048, 4000, 0x83, false, 'c'} }, 0x33334444);
    QList<ImageLayout> ls(2);
    layoutOf(a, &ls[0]);
    layoutOf(b, &ls[1]);
    const unsigned long long device = 30000;
    CombinePlan plan;
    QString why;
    bool ok = planCombine(ls, { {1, 0}, {0, 0} }, -1, SEC, device, ALIGN, false, &plan, &why);
    check(ok && plan.table == COMBINE_TABLE_MBR, "MBR images alone make an MBR");
    if (!ok) { printf("    (%s)\n", why.toLocal8Bit().constData()); return; }
    QByteArray dev = apply(plan, {a, b}, device);
    HANDLE h = openAsDevice(dev);
    QList<PartitionInfo> found;
    QString d;
    check(listMbrPartitions(h, SEC, device, &found, &d) && found.size() == 2
          && found[0].firstSector == 2048 && found[1].firstSector % ALIGN == 0,
          "disk.cpp reads it: two partitions, aligned");
    CloseHandle(h);
    const unsigned char *m = (const unsigned char *)dev.constData();
    check(m[446 + 4] == 0x83 && m[446] == 0x00 && m[462 + 4] == 0x0C && m[462] == 0x80,
          "types and the boot flag kept, in the order chosen");
    check(rd32(m, 440) != 0, "a disk signature of its own");
    check(sameSectors(dev, found[0].firstSector, b, 2048, 4000)
          && sameSectors(dev, found[1].firstSector, a, 2048, 2048), "both copied whole");

    // Lead-in from A: its boot code and signature.
    ok = planCombine(ls, { {0, 0}, {1, 0} }, 0, SEC, device, ALIGN, false, &plan, &why);
    dev = apply(plan, {a, b}, device);
    m = (const unsigned char *)dev.constData();
    check(ok && plan.table == COMBINE_TABLE_MBR && rd32(m, 440) == 0x11112222
          && memcmp(m, a.constData(), 440) == 0, "an MBR lead-in keeps its boot code and signature");
    check(sameSectors(dev, 1, a, 1, 2047), "  and its lead-in");

    // Five partitions do not fit an MBR.
    const QByteArray c = mbrImage(20000, {
        {0, 2048, 1000, 0x83, false, 'e'}, {1, 4096, 1000, 0x83, false, 'f'},
        {2, 6144, 1000, 0x83, false, 'g'}, {3, 8192, 1000, 0x83, false, 'h'} }, 1);
    QList<ImageLayout> lc(2);
    layoutOf(c, &lc[0]);
    lc[1] = ls[1];
    check(!planCombine(lc, { {0, 0}, {0, 1}, {0, 2}, {0, 3}, {1, 0} }, 0, SEC, 100000, ALIGN,
                       false, &plan, &why), "five partitions under an MBR lead-in are refused");
    check(planCombine(lc, { {0, 0}, {0, 1}, {0, 2}, {0, 3}, {1, 0} }, -1, SEC, 100000, ALIGN,
                      false, &plan, &why) && plan.table == COMBINE_TABLE_GPT,
          "  and without a lead-in make a GPT instead");
}

static void caseMixed()
{
    printf("MBR, GPT and table-less images together\n");
    const QByteArray m = mbrImage(8192, {
        {0, 2048, 2048, 0x0C, true, 'a'}, {1, 4096, 2000, 0x05, false, 'x'} }, 5);
    const QByteArray g = gptImage(9000, 34, {
        {2048, 4000, LINUX, "88888888-8888-8888-8888-888888888888", "g-root", 'b', 0},
        {4096, 4200, BIOSBOOT, "99999999-9999-9999-9999-999999999999", "bios", 'c', 0} },
        "DDDDDDDD-DDDD-DDDD-DDDD-DDDDDDDDDDDD");
    const QByteArray f = fat32Image(3000, 'f');
    QList<ImageLayout> ls(3);
    layoutOf(m, &ls[0]);
    layoutOf(g, &ls[1]);
    layoutOf(f, &ls[2]);
    CombinePlan plan;
    QString why;
    const unsigned long long device = 40000;
    bool ok = planCombine(ls, { {0, 0}, {1, 0}, {2, 0} }, -1, SEC, device, ALIGN, false, &plan, &why);
    check(ok && plan.table == COMBINE_TABLE_GPT, "with a GPT source, a GPT");
    if (!ok) { printf("    (%s)\n", why.toLocal8Bit().constData()); return; }
    const QByteArray dev = apply(plan, {m, g, f}, device);
    check(memcmp(gptEntry(dev, 0), guid(BASIC).constData(), 16) == 0
          && (rd64(gptEntry(dev, 0), 48) & 4) != 0,
          "MBR FAT32 becomes basic data, its boot flag the legacy-bootable attribute");
    check(memcmp(gptEntry(dev, 2), guid(BASIC).constData(), 16) == 0,
          "the table-less FAT32 image becomes basic data");
    check(sameSectors(dev, plan.placed[2].first, f, 0, 3000), "  and is copied whole, from sector 0");
    HANDLE h = openAsDevice(dev);
    check(gptPrimaryState(h, SEC, device) == GPT_PRIMARY_OK, "the table checks out");
    CloseHandle(h);

    check(!planCombine(ls, { {0, 1}, {1, 0} }, -1, SEC, device, ALIGN, false, &plan, &why),
          "an extended MBR partition cannot go on a GPT");
    check(!planCombine(ls, { {0, 0}, {1, 1} }, 0, SEC, device, ALIGN, false, &plan, &why),
          "BIOS boot, with no MBR type, cannot go on an MBR");
    check(planCombine(ls, { {0, 0}, {1, 0} }, 0, SEC, device, ALIGN, false, &plan, &why)
          && plan.table == COMBINE_TABLE_MBR
          && (unsigned char)plan.headerregion[462 + 4] == 0x83,
          "under an MBR lead-in, a GPT Linux partition becomes 0x83");

    QList<ImageLayout> lu(1);
    layoutOf(f, &lu[0], false);
    check(!planCombine(lu, { {0, 0} }, -1, SEC, device, ALIGN, false, &plan, &why),
          "a table-less image of unknown size must be scanned first");
    lu[0].partitions[0].sectors = 3000;
    check(planCombine(lu, { {0, 0} }, -1, SEC, device, ALIGN, false, &plan, &why)
          && plan.table == COMBINE_TABLE_MBR, "  and once its size is known, is planned");
}

static void caseDuplicates()
{
    printf("the same image twice\n");
    const QByteArray a = gptImage(9000, 34, {
        {2048, 5000, LINUX, "12121212-1212-1212-1212-121212121212", "root", 'a', 0} },
        "AAAAAAAA-AAAA-AAAA-AAAA-AAAAAAAAAAAA");
    QList<ImageLayout> ls(2);
    layoutOf(a, &ls[0]);
    layoutOf(a, &ls[1]);
    CombinePlan plan;
    QString why;
    const unsigned long long device = 20000;
    check(planCombine(ls, { {0, 0}, {1, 0} }, -1, SEC, device, ALIGN, false, &plan, &why)
          && plan.duplicateGuids.size() == 1
          && plan.duplicateGuids[0] == "12121212-1212-1212-1212-121212121212",
          "the shared partition GUID is reported");
    QByteArray dev = apply(plan, {a, a}, device);
    check(memcmp(gptEntry(dev, 0) + 16, gptEntry(dev, 1) + 16, 16) == 0,
          "  and left as it is when not asked to change it");
    check(planCombine(ls, { {0, 0}, {1, 0} }, -1, SEC, device, ALIGN, true, &plan, &why)
          && plan.duplicateGuids.isEmpty(), "asked to, nothing is left to report");
    dev = apply(plan, {a, a}, device);
    check(memcmp(gptEntry(dev, 0) + 16, guid("12121212-1212-1212-1212-121212121212").constData(), 16) == 0
          && memcmp(gptEntry(dev, 0) + 16, gptEntry(dev, 1) + 16, 16) != 0,
          "  the first keeps its GUID, the second gets a new one");
    HANDLE h = openAsDevice(dev);
    check(gptPrimaryState(h, SEC, device) == GPT_PRIMARY_OK, "  and the table still checks out");
    CloseHandle(h);
}

static void caseRefusals()
{
    printf("refusals\n");
    const QByteArray a = gptImage(9000, 34, {
        {2048, 8000, LINUX, "13131313-1313-1313-1313-131313131313", "root", 'a', 0} },
        "AAAAAAAA-AAAA-AAAA-AAAA-AAAAAAAAAAAA");
    QList<ImageLayout> ls(1);
    layoutOf(a, &ls[0]);
    CombinePlan plan;
    QString why;
    check(!planCombine(ls, { {0, 0} }, -1, SEC, 8000, ALIGN, false, &plan, &why),
          "too small a device is refused");
    check(planCombine(ls, { {0, 0} }, -1, SEC, 2048 + 5953 + 33, ALIGN, false, &plan, &why),
          "  and one just big enough is not");
    check(!planCombine(ls, {}, -1, SEC, 100000, ALIGN, false, &plan, &why), "nothing chosen is refused");
    check(!planCombine(ls, { {0, 0}, {0, 0} }, -1, SEC, 100000, ALIGN, false, &plan, &why),
          "the same partition twice is refused");
    check(!planCombine(ls, { {0, 1} }, -1, SEC, 100000, ALIGN, false, &plan, &why),
          "a partition that does not exist is refused");
    const QByteArray f = fat32Image(3000, 'f');
    QList<ImageLayout> lf(1);
    layoutOf(f, &lf[0]);
    check(!planCombine(lf, { {0, 0} }, 0, SEC, 100000, ALIGN, false, &plan, &why),
          "an image with no table cannot be the lead-in");
}

static QByteArray gzipOf(const QByteArray &raw)
{
    z_stream z;
    memset(&z, 0, sizeof(z));
    deflateInit2(&z, 6, Z_DEFLATED, 31, 8, Z_DEFAULT_STRATEGY);
    QByteArray out((int)deflateBound(&z, (uLong)raw.size()) + 64, 0);
    z.next_in = (Bytef *)raw.constData();
    z.avail_in = (uInt)raw.size();
    z.next_out = (Bytef *)out.data();
    z.avail_out = (uInt)out.size();
    deflate(&z, Z_FINISH);
    out.resize((int)z.total_out);
    deflateEnd(&z);
    return out;
}

static bool writeFile(const char *name, const QByteArray &bytes)
{
    FILE *f = fopen(name, "wb");
    if (!f) return false;
    const bool ok = fwrite(bytes.constData(), 1, (size_t)bytes.size(), f) == (size_t)bytes.size();
    fclose(f);
    return ok;
}

// The dialog, off screen, driven as a user would drive it: images added, each
// partition ticked in the order wanted, a lead-in picked. What it plans must
// be what planCombine() plans, and apply to a device that reads back right --
// through ImageSource, so the gzip image's table comes from decompressing it.
static void caseDialog()
{
    printf("the dialog\n");
    const QByteArray a = gptImage(60000, 64, {
        {32768, 40959, BASIC, "A1A1A1A1-A1A1-A1A1-A1A1-A1A1A1A1A1A1", "a-boot", 'a', 0},
        {40960, 50000, LINUX, "A2A2A2A2-A2A2-A2A2-A2A2-A2A2A2A2A2A2", "a-root", 'b', 0} },
        "AAAAAAAA-AAAA-AAAA-AAAA-AAAAAAAAAAAA");
    const QByteArray b = gptImage(12000, 34, {
        {2048, 6000, LINUX, "B1B1B1B1-B1B1-B1B1-B1B1-B1B1B1B1B1B1", "b-home", 'c', 0} },
        "BBBBBBBB-BBBB-BBBB-BBBB-BBBBBBBBBBBB");
    const QByteArray f = fat32Image(3000, 'f');
    check(writeFile("combinetest-a.img", a) && writeFile("combinetest-b.img.gz", gzipOf(b))
          && writeFile("combinetest-f.img", f), "fixture files written");

    const unsigned long long device = 200000;
    CombineDialog dlg(NULL, "test device", device, SEC, ".", QStringList("*.*"));
    dlg.addImageFiles({ "combinetest-a.img", "combinetest-b.img.gz", "combinetest-f.img" });
    QTreeWidget *images = NULL;
    for (QTreeWidget *t : dlg.findChildren<QTreeWidget *>())
    {
        if (t->headerItem()->text(0) == "Image / partition") images = t;
    }
    check(images && images->topLevelItemCount() == 3, "three images listed");
    if (!images || images->topLevelItemCount() != 3) return;
    check(images->topLevelItem(1)->text(1).contains("gzip")
          && images->topLevelItem(1)->childCount() == 1
          && images->topLevelItem(1)->child(0)->text(0).contains("b-home"),
          "the gzip image's partition table was read from inside it");
    check(images->topLevelItem(2)->childCount() == 1
          && images->topLevelItem(2)->child(0)->text(1).startsWith("FAT32"),
          "the FAT32 image is one partition");
    check(!dlg.planIsValid(), "nothing ticked: nothing to write");

    // Its tooltips wrapped as the main window's are: no line of one wider
    // than the budget unless it is a single word, and the long ones broken.
    {
        const QFontMetrics fm(QToolTip::font());
        bool narrow = true, anywrapped = false;
        for (QWidget *w : dlg.findChildren<QWidget *>())
        {
            const QStringList lines = w->toolTip().split(QChar('\n'));
            anywrapped = anywrapped || lines.size() > 1;
            for (const QString &line : lines)
            {
                narrow = narrow && (fm.horizontalAdvance(line) <= 380 || !line.contains(QChar(' ')));
            }
        }
        check(narrow && anywrapped, "its long tooltips are wrapped");
    }

    // Ticked in the order wanted on the device: B's home, A's boot, the FAT
    // image, A's root.
    images->topLevelItem(1)->child(0)->setCheckState(0, Qt::Checked);
    images->topLevelItem(0)->child(0)->setCheckState(0, Qt::Checked);
    images->topLevelItem(2)->child(0)->setCheckState(0, Qt::Checked);
    images->topLevelItem(0)->child(1)->setCheckState(0, Qt::Checked);
    QListWidget *order = dlg.findChild<QListWidget *>();
    check(order && order->count() == 4 && order->item(0)->text().contains("b-home")
          && order->item(3)->text().contains("a-root"), "the order is the order ticked");
    check(dlg.planIsValid() && dlg.plan().table == COMBINE_TABLE_GPT
          && dlg.plan().placed.size() == 4 && dlg.plan().placed[0].image == 1,
          "planned: a GPT, B's home first");

    // A as the lead-in.
    QComboBox *lead = dlg.findChild<QComboBox *>();
    check(lead && lead->findData(0) >= 0, "A is offered as the lead-in");
    lead->setCurrentIndex(lead->findData(0));
    check(dlg.planIsValid() && dlg.plan().placed[0].first == 32768,
          "with A's lead-in the first partition starts where A's did");
    check(lead->findData(2) < 0, "the image with no table is not offered as a lead-in");

    const CombinePlan plan = dlg.plan();
    const QStringList paths = dlg.imagePaths();
    check(paths.size() == 3 && paths[1].endsWith("combinetest-b.img.gz"), "the paths the ranges index");
    const QByteArray dev = apply(plan, { a, b, f }, device);
    check(sameSectors(dev, 64, a, 64, 32768 - 64), "A's bootloader area copied");
    check(sameSectors(dev, plan.placed[0].first, b, 2048, 3953), "B's home copied");
    check(sameSectors(dev, plan.placed[2].first, f, 0, 3000), "the FAT image copied");
    HANDLE h = openAsDevice(dev);
    QList<PartitionInfo> found;
    QString d;
    check(listGptPartitions(h, SEC, device, &found, &d) && found.size() == 4
          && found[0].name == "b-home" && found[1].name == "a-boot" && found[3].name == "a-root",
          "disk.cpp reads the four partitions, in order");
    check(gptPrimaryState(h, SEC, device) == GPT_PRIMARY_OK, "  under a valid table");
    CloseHandle(h);

    // COMBINETEST_SHOT=file.png saves the dialog as drawn, to look at.
    if (!qEnvironmentVariableIsEmpty("COMBINETEST_SHOT"))
    {
        dlg.show();
        QCoreApplication::processEvents();
        dlg.grab().save(qEnvironmentVariable("COMBINETEST_SHOT"));
    }

    // To an image file instead: planned to fit, not to the device.
    QRadioButton *tofile = NULL;
    for (QRadioButton *r : dlg.findChildren<QRadioButton *>())
    {
        if (r->text() == "An image file:") tofile = r;
    }
    check(tofile && !dlg.toFile(), "the device is the default destination");
    if (tofile)
    {
        tofile->setChecked(true);
        const CombinePlan &fp = dlg.plan();
        check(dlg.toFile() && dlg.planIsValid() && fp.totalsectors == fp.usedsectors + 33,
              "an image file is planned just big enough, backup GPT included");
        dlg.findChildren<QRadioButton *>().first()->setChecked(true);
        check(!dlg.toFile() && dlg.plan().totalsectors == device, "  and back to the device, its size");
    }

    // Unticking one takes it out of the order.
    images->topLevelItem(2)->child(0)->setCheckState(0, Qt::Unchecked);
    check(order->count() == 3 && dlg.plan().placed.size() == 3, "unticked, it leaves the layout");

    DeleteFileA("combinetest-a.img");
    DeleteFileA("combinetest-b.img.gz");
    DeleteFileA("combinetest-f.img");
}

// Planned for an image file: no device, so exactly as big as the layout,
// and produced front to back by CombineReader, as a compressed file must be.
// It must be byte for byte what applying the plan to a device gives -- read
// in odd-sized pieces, from a gzip image whose partitions go on in reverse
// order, which it can only do by starting that image again.
static void caseImageFile()
{
    printf("to an image file\n");
    const QByteArray a = gptImage(20000, 34, {
        {2048, 6000, LINUX, "C1C1C1C1-C1C1-C1C1-C1C1-C1C1C1C1C1C1", "a-root", 'a', 0} },
        "AAAAAAAA-AAAA-AAAA-AAAA-AAAAAAAAAAAA");
    const QByteArray b = gptImage(20000, 34, {
        {2048, 4095, ESP, "C2C2C2C2-C2C2-C2C2-C2C2-C2C2C2C2C2C2", "b-boot", 'b', 0},
        {4096, 9000, LINUX, "C3C3C3C3-C3C3-C3C3-C3C3-C3C3C3C3C3C3", "b-root", 'c', 0} },
        "BBBBBBBB-BBBB-BBBB-BBBB-BBBBBBBBBBBB");
    check(writeFile("combinetest-a.img", a) && writeFile("combinetest-b.img.gz", gzipOf(b)),
          "fixture files written");
    QList<ImageLayout> ls(2);
    layoutOf(a, &ls[0]);
    layoutOf(b, &ls[1]);
    CombinePlan plan;
    QString why;
    // B's root before B's boot: backwards through the gzip stream.
    const bool ok = planCombine(ls, { {1, 1}, {0, 0}, {1, 0} }, -1, SEC, 0, ALIGN, false, &plan, &why);
    check(ok, "planned with no device");
    if (!ok) { printf("    (%s)\n", why.toLocal8Bit().constData()); return; }
    const CombinePlaced &lastp = plan.placed.last();
    check(plan.totalsectors == lastp.first + lastp.sectors + 33 && plan.usedsectors == lastp.first + lastp.sectors,
          "the image is just the layout and its backup GPT");
    const QByteArray expect = apply(plan, { a, b }, plan.totalsectors);
    check(backupAtEnd(expect, plan.totalsectors), "  which ends the image");

    QByteArray buf(7 * (int)SEC, 0);
    unsigned long long n = 0;
    {
        // Scoped: it holds the images open, and the one below is rewritten.
        CombineReader reader(plan, { "combinetest-a.img", "combinetest-b.img.gz" }, SEC);
        check(reader.totalSectors() == plan.totalsectors, "the reader's length is the image's");
        QByteArray got;
        bool readok = true;
        do
        {
            readok = reader.read(buf.data(), 7, &n);
            got.append(buf.constData(), (int)(n * SEC));
        } while (readok && n == 7);
        check(readok, "read to the end without error");
        if (!readok) printf("    (%s)\n", reader.errorString().toLocal8Bit().constData());
        check(got == expect, "every byte is what the plan puts there");
    }
    {
        // Into an .img.xz as the file output writes it, and read back as a
        // Write or Verify would read it.
        CombineReader reader(plan, { "combinetest-a.img", "combinetest-b.img.gz" }, SEC);
        ImageSink sink;
        bool ok2 = sink.open("combinetest-out.img.xz", ImageSink::FORMAT_XZ);
        QByteArray chunk(64 * (int)SEC, 0);
        do
        {
            ok2 = ok2 && reader.read(chunk.data(), 64, &n)
                  && sink.write(chunk.constData(), n * SEC);
        } while (ok2 && n == 64);
        ok2 = ok2 && sink.finish();
        check(ok2, "written to an .img.xz");
        ImageSource back;
        QByteArray all((int)(plan.totalsectors * SEC), 0);
        unsigned long long gotback = 0, extra = 0;
        check(back.open("combinetest-out.img.xz", SEC)
              && back.readInto(all.data(), 0, plan.totalsectors, &gotback)
              && gotback == plan.totalsectors && all == expect,
              "  which decompresses to exactly the combined image");
        char *tail = back.read(plan.totalsectors, 1, &extra);
        check(tail != NULL && extra == 0, "  and ends where it does");
        delete[] tail;
    }
    DeleteFileA("combinetest-out.img.xz");

    // A gzip image cut short: the reader must say so, not pad it out.
    const QByteArray gz = gzipOf(b);
    // A fifth of the stream: short of b-root, which ends 45% of the way in.
    check(writeFile("combinetest-b.img.gz", gz.left(gz.size() / 5)), "a truncated copy written");
    CombineReader cut(plan, { "combinetest-a.img", "combinetest-b.img.gz" }, SEC);
    bool cutok = true;
    do
    {
        cutok = cut.read(buf.data(), 7, &n);
    } while (cutok && n == 7);
    check(!cutok && !cut.errorString().isEmpty(), "a truncated image is an error");
    DeleteFileA("combinetest-a.img");
    DeleteFileA("combinetest-b.img.gz");
}

int main(int argc, char **argv)
{
    // The dialog is created off screen: nothing is shown.
    qputenv("QT_QPA_PLATFORM", "offscreen");
    QApplication app(argc, argv);
    caseParse();
    caseTwoGpt();
    caseLeadIn();
    caseMbr();
    caseMixed();
    caseDuplicates();
    caseRefusals();
    caseDialog();
    caseImageFile();
    DeleteFileA(TESTFILE);
    printf("\n%d checks, %d failures\n", checks, failures);
    return failures ? 1 : 0;
}
