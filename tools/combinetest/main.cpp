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
#include <QCheckBox>
#include <QComboBox>
#include <QListWidget>
#include <QRadioButton>
#include <QToolTip>
#include <QLineEdit>
#include <QSpinBox>
#include <QMessageBox>
#include <QPushButton>
#include <QTimer>
#include <QTreeWidget>
#include <QHeaderView>
#include <QString>
#include <QUuid>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <windows.h>
#include <shlobj.h>
#include <string>
#include <cstdio>
#include <cstring>
#include <zlib.h>
#include "bootcheck.h"
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
static const char *WINRE_PART = "DE94BBA4-06D1-4D40-A16A-BFD50179D6AC";
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

// As apply(), onto a device that already holds `device`, as a plan that keeps
// the target is written: nothing cleared first.
static QByteArray applyOnto(const CombinePlan &plan, const QList<QByteArray> &imgs,
                            QByteArray device)
{
    QByteArray dev = device;
    dev.resize((int)(plan.totalsectors * SEC));
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

// Stand-ins for the main window's device list and sector-size probe, which
// need real disks and Administrator. Disk 95 has 4096-byte sectors.
static CombineDeviceLister fakeDevices(const QList<CombineTarget> &devices)
{
    return [devices](bool) { return devices; };
}
static unsigned long long fakeSectorSize(int n)
{
    return (n == 95) ? 4096ull : SEC;
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
    CombineDialog dlg(NULL, fakeDevices({ {90, "test device", "test device", device * SEC} }),
                      fakeSectorSize, 90, false, SEC, ".", QStringList("*.*"));
    check(dlg.size() == dlg.minimumSizeHint(), "the window opens at its smallest");
    dlg.addImageFiles({ "combinetest-a.img", "combinetest-b.img.gz", "combinetest-f.img" });
    QTreeWidget *images = NULL;
    for (QTreeWidget *t : dlg.findChildren<QTreeWidget *>())
    {
        if (t->headerItem()->text(0) == "Source / partition") images = t;
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

    // Both tables' headers work alike: every column movable, the first too,
    // and every one but the last resizable by hand, the last filling the rest.
    for (QTreeWidget *t : dlg.findChildren<QTreeWidget *>())
    {
        QHeaderView *h = t->header();
        bool interactive = true;
        for (int c = 0; c < h->count() - 1; ++c)
        {
            interactive = interactive && h->sectionResizeMode(c) == QHeaderView::Interactive;
        }
        const QByteArray what = ("the \"" + t->headerItem()->text(0)
                                 + "\" table's columns can all be moved and resized").toLocal8Bit();
        check(h->sectionsMovable() && h->isFirstSectionMovable() && interactive
              && (h->stretchLastSection()
                  || h->sectionResizeMode(h->count() - 1) == QHeaderView::Stretch),
              what.constData());
    }

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

    // Shrunk as small as it goes: the order list still shows two whole rows,
    // and so does the layout table, which is no longer held at six.
    {
        const QSize before = dlg.size();
        dlg.show();
        dlg.resize(1, 1);
        QCoreApplication::processEvents();
        QTreeWidget *preview = NULL;
        for (QTreeWidget *t : dlg.findChildren<QTreeWidget *>())
        {
            if (t->headerItem()->text(0) == "On the device") preview = t;
        }
        const int orderrow = order->sizeHintForRow(0);
        const int previewrow = preview ? preview->sizeHintForRow(0) : 0;
        printf("    (smallest: order %d px for %d-px rows, table %d px for %d-px rows)\n",
               order->viewport()->height(), orderrow,
               preview ? preview->viewport()->height() : 0, previewrow);
        check(orderrow > 0 && order->viewport()->height() >= 2 * orderrow,
              "at its smallest, the order list shows two partitions");
        check(preview && previewrow > 0 && preview->viewport()->height() >= 2 * previewrow
              && preview->viewport()->height() < 6 * previewrow,
              "  and the layout table two rows, not six");
        if (!qEnvironmentVariableIsEmpty("COMBINETEST_SHOT_MIN"))
        {
            dlg.grab().save(qEnvironmentVariable("COMBINETEST_SHOT_MIN"));
        }
        dlg.resize(before);
        QCoreApplication::processEvents();
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

// The image file name field: the one line edit that is not a spin box's.
static QLineEdit *outFileEdit(QWidget &dlg)
{
    for (QLineEdit *e : dlg.findChildren<QLineEdit *>())
    {
        if (!qobject_cast<QAbstractSpinBox *>(e->parentWidget())) return e;
    }
    return NULL;
}

// Presses Write in the dialog, answering whatever message boxes come up with
// the button named `answer` (or the first), and returns their titles in order.
static QStringList pressWrite(CombineDialog &dlg, const QString &answer)
{
    QStringList seen;
    QTimer timer;
    timer.setInterval(20);
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        QMessageBox *box = qobject_cast<QMessageBox *>(QApplication::activeModalWidget());
        if (!box || box == (QWidget *)&dlg) return;
        seen.append(box->windowTitle());
        for (QAbstractButton *b : box->buttons())
        {
            if (b->text() == answer) { b->click(); return; }
        }
        box->buttons().first()->click();
    });
    timer.start();
    QPushButton *write = NULL;
    for (QPushButton *b : dlg.findChildren<QPushButton *>())
    {
        if (b->text() == "Write...") write = b;
    }
    if (write) write->click();
    timer.stop();
    return seen;
}

// The same partition from two copies of one image, to a device and to an
// image file: both must ask about the shared GUID, and "Generate new GUIDs"
// must leave the accepted plan without it.
static void caseDuplicatePrompt(bool tofile)
{
    printf("duplicate GUIDs, writing to %s\n", tofile ? "an image file" : "a device");
    const QByteArray a = gptImage(9000, 34, {
        {2048, 5000, LINUX, "D1D1D1D1-D1D1-D1D1-D1D1-D1D1D1D1D1D1", "root", 'a', 0} },
        "AAAAAAAA-AAAA-AAAA-AAAA-AAAAAAAAAAAA");
    writeFile("combinetest-dup1.img", a);
    writeFile("combinetest-dup2.img", a);
    DeleteFileA("combinetest-dupout.img");
    // Writing to a file, with no device to offer at all.
    CombineDialog dlg(NULL,
                      fakeDevices(tofile ? QList<CombineTarget>()
                                         : QList<CombineTarget>{ {91, "test device", "test device",
                                                                  100000 * SEC} }),
                      fakeSectorSize, 91, false, SEC, ".", QStringList("*.*"));
    dlg.addImageFiles({ "combinetest-dup1.img", "combinetest-dup2.img" });
    QTreeWidget *images = NULL;
    for (QTreeWidget *t : dlg.findChildren<QTreeWidget *>())
    {
        if (t->headerItem()->text(0) == "Source / partition") images = t;
    }
    images->topLevelItem(0)->child(0)->setCheckState(0, Qt::Checked);
    images->topLevelItem(1)->child(0)->setCheckState(0, Qt::Checked);
    if (tofile)
    {
        outFileEdit(dlg)->setText("combinetest-dupout.img");
    }
    check(dlg.toFile() == tofile && dlg.planIsValid() && dlg.plan().duplicateGuids.size() == 1,
          "the shared GUID is found");
    const QStringList seen = pressWrite(dlg, "Generate new GUIDs");
    printf("    (message boxes: %s)\n", seen.join(", ").toLocal8Bit().constData());
    check(seen.contains("Duplicate partition GUIDs"), "Write asks about it");
    check(dlg.result() == QDialog::Accepted && dlg.plan().duplicateGuids.isEmpty(),
          "  and after new GUIDs, the plan accepted has none");
    DeleteFileA("combinetest-dup1.img");
    DeleteFileA("combinetest-dup2.img");
}

// Disks as sources: the path the dialog gives one, and ImageSource taking such
// a path as a disk, never as a file of that name. Reading a real disk needs
// Administrator and a disk to spare, so only what fails cleanly without one
// is tried here: a disk that is not there must say so, naming it.
static void caseDiskPaths()
{
    printf("disks as sources\n");
    check(ImageSource::devicePath(3) == "\\\\.\\PhysicalDrive3", "disk 3 is \\\\.\\PhysicalDrive3");
    check(ImageSource::deviceNumber(ImageSource::devicePath(12)) == 12
          && ImageSource::deviceNumber("\\\\.\\physicaldrive7") == 7,
          "  and the number comes back from the path, in any case");
    check(ImageSource::deviceNumber("C:\\images\\PhysicalDrive3.img") == -1
          && ImageSource::deviceNumber("\\\\.\\PhysicalDriveX") == -1
          && ImageSource::deviceNumber("combinetest.img") == -1,
          "a file is not a disk, whatever its name");
    ImageSource src;
    check(!src.open(ImageSource::devicePath(999), SEC)
          && src.errorString().contains("Disk 999") && !src.isDevice(),
          "a disk that cannot be opened is reported as that disk");
}

// The device is chosen in the dialog now: the main window's selection only
// starts it off. Switching device replans for the new one's size; a device
// the sources' sectors do not fit is refused; with no device at all, only an
// image file is offered.
static void caseDeviceChoice()
{
    printf("choosing the device in the dialog\n");
    const QByteArray a = gptImage(9000, 34, {
        {2048, 5000, LINUX, "E1E1E1E1-E1E1-E1E1-E1E1-E1E1E1E1E1E1", "root", 'a', 0} },
        "AAAAAAAA-AAAA-AAAA-AAAA-AAAAAAAAAAAA");
    writeFile("combinetest-dev.img", a);
    const QList<CombineTarget> devices = {
        {92, "[Disk 92] small", "small", 20000 * SEC},
        {93, "[Disk 93] big", "big", 400000 * SEC},
        {95, "[Disk 95] 4Kn", "4Kn", 400000 * SEC} };
    CombineDialog dlg(NULL, fakeDevices(devices), fakeSectorSize, 93, false, SEC, ".",
                      QStringList("*.*"));
    dlg.addImageFiles({ "combinetest-dev.img" });
    QTreeWidget *images = NULL;
    for (QTreeWidget *t : dlg.findChildren<QTreeWidget *>())
    {
        if (t->headerItem()->text(0) == "Source / partition") images = t;
    }
    images->topLevelItem(0)->child(0)->setCheckState(0, Qt::Checked);
    check(!dlg.toFile() && dlg.targetDevice() == 93 && dlg.targetSectors() == 400000
          && dlg.planIsValid() && dlg.plan().totalsectors == 400000,
          "it starts on the device the main window had, and plans for its size");
    QComboBox *box = NULL;
    for (QComboBox *c : dlg.findChildren<QComboBox *>())
    {
        if (c->findData(92) >= 0) box = c;
    }
    check(box && box->count() == 3, "every device offered is listed");
    if (!box) return;
    box->setCurrentIndex(box->findData(92));
    check(dlg.targetDevice() == 92 && dlg.plan().totalsectors == 20000
          && dlg.targetText() == "[Disk 92] small", "another is chosen, and planned for");
    box->setCurrentIndex(box->findData(95));
    check(!dlg.planIsValid(), "a device with 4096-byte sectors is refused");
    box->setCurrentIndex(box->findData(93));
    check(dlg.planIsValid(), "  and a usable one after it is not");

    CombineDialog none(NULL, fakeDevices({}), fakeSectorSize, -1, false, SEC, ".",
                       QStringList("*.*"));
    QRadioButton *todevice = none.findChildren<QRadioButton *>().first();
    check(none.toFile() && !todevice->isEnabled(), "with no device at all, only a file is offered");
    DeleteFileA("combinetest-dev.img");
}

static void caseFreeSpace()
{
    printf("free space in the layout\n");
    const QByteArray a = gptImage(9000, 34, {
        {2048, 5000, LINUX, "F1F1F1F1-F1F1-F1F1-F1F1-F1F1F1F1F1F1", "fa", 'a', 0} },
        "F0F0F0F0-F0F0-F0F0-F0F0-F0F0F0F0F0F0");
    const QByteArray b = gptImage(9000, 34, {
        {2048, 6000, LINUX, "F2F2F2F2-F2F2-F2F2-F2F2-F2F2F2F2F2F2", "fb", 'b', 0} },
        "F9F9F9F9-F9F9-F9F9-F9F9-F9F9F9F9F9F9");
    QList<ImageLayout> ls(2);
    layoutOf(a, &ls[0]);
    layoutOf(b, &ls[1]);
    const unsigned long long device = 100000;
    CombinePlan plan;
    QString why;
    bool ok = planCombine(ls, { {0, 0}, {COMBINE_FREE, -1, 4096}, {1, 0} }, -1, SEC, device,
                          ALIGN, false, &plan, &why);
    check(ok && plan.placed.size() == 3 && plan.partitionCount() == 2, "free space between two partitions");
    if (!ok) { printf("    (%s)\n", why.toLocal8Bit().constData()); return; }
    check(plan.placed[1].image == COMBINE_FREE && plan.placed[1].first == 5001
          && plan.placed[1].sectors == 4096 && plan.placed[1].slot == -1,
          "  it starts where the partition before it ends");
    check(plan.placed[2].first == 10240 && plan.placed[2].slot == 1,
          "  and the next partition comes after it, aligned, in the next slot");
    QByteArray dev = apply(plan, {a, b}, device);
    check(rd64(gptEntry(dev, 1), 32) == 10240 && sameSectors(dev, 10240, b, 2048, 3953),
          "  the table and the copy agree");

    ok = planCombine(ls, { {COMBINE_FREE, -1, 6144}, {0, 0} }, -1, SEC, device, ALIGN, false,
                     &plan, &why);
    check(ok && plan.placed[0].first == 2048 && plan.placed[1].first == 8192,
          "free space first: the first partition moves up past it");
    ok = planCombine(ls, { {0, 0}, {COMBINE_FREE, -1, 2048} }, -1, SEC, 0, ALIGN, false,
                     &plan, &why);
    check(ok && plan.usedsectors == 5001 + 2048 && plan.totalsectors == 5001 + 2048 + 33,
          "free space last makes an image file that much bigger");
    check(!planCombine(ls, { {0, 0}, {COMBINE_FREE, -1, 0} }, -1, SEC, device, ALIGN, false,
                       &plan, &why), "free space of no size is refused");
    check(!planCombine(ls, { {COMBINE_FREE, -1, 2048} }, -1, SEC, device, ALIGN, false,
                       &plan, &why), "  and free space alone is no layout");
    // After a lead-in, free space puts the first partition on a boundary,
    // not where the lead-in ends.
    ok = planCombine(ls, { {COMBINE_FREE, -1, 2048}, {0, 0} }, 0, SEC, device, ALIGN, false,
                     &plan, &why);
    check(ok && plan.placed[0].first == 2048 && plan.placed[1].first == 4096,
          "after a lead-in, free space goes where it ends, and the partition after it");
}

// A device holding two partitions with space between and after them.
static QByteArray keptGptDevice(unsigned long long sectors)
{
    return gptImage(sectors, 34, {
        {2048, 6143, BASIC, "C1C1C1C1-C1C1-C1C1-C1C1-C1C1C1C1C1C1", "keep1", 'p', 0},
        {20480, 24575, LINUX, "C2C2C2C2-C2C2-C2C2-C2C2-C2C2C2C2C2C2", "keep2", 'q', 0} },
        "DDDDDDDD-DDDD-DDDD-DDDD-DDDDDDDDDDDD");
}

static void caseKeepGpt()
{
    printf("adding to a GPT device, keeping its partitions\n");
    const unsigned long long device = 40000;
    const QByteArray t = keptGptDevice(device);
    ImageLayout tl;
    layoutOf(t, &tl);
    const QByteArray a = gptImage(9000, 34, {
        {2048, 5000, LINUX, "A7A7A7A7-A7A7-A7A7-A7A7-A7A7A7A7A7A7", "new1", 'a', 0} },
        "A0A0A0A0-A0A0-A0A0-A0A0-A0A0A0A0A0A0");
    const QByteArray b = gptImage(20000, 34, {
        {2048, 14000, LINUX, "B7B7B7B7-B7B7-B7B7-B7B7-B7B7B7B7B7B7", "new2", 'b', 0} },
        "B0B0B0B0-B0B0-B0B0-B0B0-B0B0B0B0B0B0");
    QList<ImageLayout> ls(2);
    layoutOf(a, &ls[0]);
    layoutOf(b, &ls[1]);
    CombinePlan plan;
    QString why;
    bool ok = planCombine(ls, { {COMBINE_KEPT, 0}, {0, 0}, {COMBINE_KEPT, 1}, {1, 0} }, -1, SEC,
                          device, ALIGN, false, &plan, &why, &tl);
    check(ok, "planned");
    if (!ok) { printf("    (%s)\n", why.toLocal8Bit().constData()); return; }
    check(plan.table == COMBINE_TABLE_GPT && plan.keepsTarget && plan.targetregion == tl.tableRegion
          && plan.partitionCount() == 4 && plan.ranges.size() == 2,
          "four partitions, two of them kept and not copied");
    check(plan.placed[0].image == COMBINE_KEPT && plan.placed[0].first == 2048 && plan.placed[0].slot == 0
          && plan.placed[2].image == COMBINE_KEPT && plan.placed[2].first == 20480 && plan.placed[2].slot == 1,
          "  the kept ones where they are, in their own slots");
    check(plan.placed[1].first == 6144 && plan.placed[1].slot == 2
          && plan.placed[3].first == 24576 && plan.placed[3].slot == 3,
          "  the new ones in the free space between and after, in the free slots");
    const QByteArray dev = applyOnto(plan, {a, b}, t);
    check(sameSectors(dev, 2048, t, 2048, 4096) && sameSectors(dev, 20480, t, 20480, 4096),
          "the kept partitions' data is untouched");
    check(sameSectors(dev, 6144, a, 2048, 2953) && sameSectors(dev, 24576, b, 2048, 11953),
          "  the new ones are copied whole");
    check(memcmp(gptEntry(dev, 0), t.constData() + 2 * SEC, 128) == 0
          && memcmp(gptEntry(dev, 1), t.constData() + 2 * SEC + 128, 128) == 0,
          "  the kept entries are as they were, byte for byte");
    check(rd64(gptEntry(dev, 2), 32) == 6144 && rd64(gptEntry(dev, 3), 32) == 24576,
          "  and the new entries say where the new partitions are");
    const unsigned char *h = (const unsigned char *)dev.constData() + SEC;
    check(memcmp(h + 56, guid("DDDDDDDD-DDDD-DDDD-DDDD-DDDDDDDDDDDD").constData(), 16) == 0
          && memcmp(dev.constData(), t.constData(), 440) == 0,
          "the device keeps its disk GUID and boot code");
    HANDLE hd = openAsDevice(dev);
    check(gptPrimaryState(hd, SEC, device) == GPT_PRIMARY_OK, "the table checks out");
    CloseHandle(hd);
    check(backupAtEnd(dev, device), "  its backup at the end of the device");

    // The device is bigger than its table says: the table is fixed up.
    const unsigned long long grown = 60000;
    ok = planCombine(ls, { {COMBINE_KEPT, 0}, {COMBINE_KEPT, 1}, {0, 0} }, -1, SEC, grown, ALIGN,
                     false, &plan, &why, &tl);
    const QByteArray big = applyOnto(plan, {a, b}, t);
    check(ok && backupAtEnd(big, grown), "a device grown past its table gets its backup at its new end");

    ok = planCombine(ls, { {COMBINE_KEPT, 0}, {0, 0}, {1, 0}, {COMBINE_KEPT, 1} }, -1, SEC,
                     device, ALIGN, false, &plan, &why, &tl);
    check(!ok && why.contains("too little free space") && why.contains("partition 2"),
          "new partitions that do not fit before a kept one are refused, naming it");
    ok = planCombine(ls, { {0, 0}, {COMBINE_KEPT, 1} }, -1, SEC, device, ALIGN, false,
                     &plan, &why, &tl);
    check(ok && plan.placed[0].first == 2048 && plan.placed[0].slot == 0 && plan.placed[1].slot == 1,
          "a partition taken out of the table frees its space and its slot");
    check(!planCombine(ls, { {COMBINE_KEPT, 1}, {COMBINE_KEPT, 0} }, -1, SEC, device, ALIGN,
                       false, &plan, &why, &tl), "kept partitions cannot change places");
    check(!planCombine(ls, { {COMBINE_KEPT, 0}, {0, 0} }, 0, SEC, device, ALIGN, false,
                       &plan, &why, &tl), "  nor come with a lead-in");
    check(!planCombine(ls, { {COMBINE_KEPT, 0}, {0, 0} }, -1, SEC, 0, ALIGN, false,
                       &plan, &why, &tl), "  nor go in an image file");
    check(!planCombine(ls, { {COMBINE_KEPT, 0}, {0, 0} }, -1, SEC, device, ALIGN, false,
                       &plan, &why), "  nor be planned without the device's layout");

    // A copy of a kept partition: the copy gets the new GUID.
    const QByteArray d = gptImage(9000, 34, {
        {2048, 5000, LINUX, "C1C1C1C1-C1C1-C1C1-C1C1-C1C1C1C1C1C1", "copy", 'd', 0} },
        "D0D0D0D0-D0D0-D0D0-D0D0-D0D0D0D0D0D0");
    QList<ImageLayout> ld(1);
    layoutOf(d, &ld[0]);
    ok = planCombine(ld, { {COMBINE_KEPT, 0}, {0, 0}, {COMBINE_KEPT, 1} }, -1, SEC, device, ALIGN,
                     false, &plan, &why, &tl);
    check(ok && plan.duplicateGuids.size() == 1, "a new partition sharing a kept one's GUID is found");
    ok = planCombine(ld, { {COMBINE_KEPT, 0}, {0, 0}, {COMBINE_KEPT, 1} }, -1, SEC, device, ALIGN,
                     true, &plan, &why, &tl);
    const QByteArray dd = applyOnto(plan, {d}, t);
    const QByteArray c1 = guid("C1C1C1C1-C1C1-C1C1-C1C1-C1C1C1C1C1C1");
    check(ok && memcmp(gptEntry(dd, 0) + 16, c1.constData(), 16) == 0
          && memcmp(gptEntry(dd, 2) + 16, c1.constData(), 16) != 0,
          "  and given a new one, the kept partition keeping its own");

    // More than 32 MiB before the device's first partition: a bootloader may
    // be there, so 32 MiB of it is kept free.
    const QByteArray distant = gptImage(120000, 34, {
        {81920, 90000, LINUX, "C3C3C3C3-C3C3-C3C3-C3C3-C3C3C3C3C3C3", "far", 'r', 0} },
        "EEEEEEEE-EEEE-EEEE-EEEE-EEEEEEEEEEEE");
    ImageLayout fl;
    layoutOf(distant, &fl);
    ok = planCombine(ls, { {0, 0}, {COMBINE_KEPT, 0} }, -1, SEC, 120000, ALIGN, false,
                     &plan, &why, &fl);
    check(ok && plan.placed[0].first == ((34 + 65536 + ALIGN - 1) / ALIGN) * ALIGN,
          "the first 32 MiB before a kept device's first partition stay free");
}

static void caseKeepMbr()
{
    printf("adding to an MBR device, keeping its partitions\n");
    const unsigned long long device = 30000;
    const QByteArray t = mbrImage(device, {
        {0, 2048, 4096, 0x0C, true, 'p'}, {2, 16384, 4096, 0x83, false, 'q'} }, 0x5555AAAAu);
    ImageLayout tl;
    layoutOf(t, &tl);
    const QByteArray a = mbrImage(8192, { {0, 2048, 3000, 0x83, false, 'a'} }, 1);
    const QByteArray c = mbrImage(20000, {
        {0, 2048, 1000, 0x83, false, 'e'}, {1, 4096, 1000, 0x83, false, 'f'},
        {2, 6144, 1000, 0x83, false, 'g'} }, 2);
    QList<ImageLayout> ls(2);
    layoutOf(a, &ls[0]);
    layoutOf(c, &ls[1]);
    CombinePlan plan;
    QString why;
    const bool ok = planCombine(ls, { {COMBINE_KEPT, 0}, {0, 0}, {COMBINE_KEPT, 1} }, -1, SEC,
                                device, ALIGN, false, &plan, &why, &tl);
    check(ok && plan.table == COMBINE_TABLE_MBR, "planned, on the device's MBR");
    if (!ok) { printf("    (%s)\n", why.toLocal8Bit().constData()); return; }
    const QByteArray dev = applyOnto(plan, {a, c}, t);
    const unsigned char *m = (const unsigned char *)dev.constData();
    check(memcmp(m + 446, t.constData() + 446, 16) == 0 && memcmp(m + 478, t.constData() + 478, 16) == 0,
          "the kept entries stay in their slots, as they were");
    check(m[462 + 4] == 0x83 && rd32(m + 462, 8) == 6144 && rd32(m + 462, 12) == 3000,
          "  the new one takes the free slot, in the space between");
    check(rd32(m, 440) == 0x5555AAAAu && memcmp(m, t.constData(), 440) == 0,
          "  the boot code and disk signature are kept");
    check(sameSectors(dev, 2048, t, 2048, 4096) && sameSectors(dev, 16384, t, 16384, 4096)
          && sameSectors(dev, 6144, a, 2048, 3000), "  the kept data untouched, the new copied");
    check(!planCombine(ls, { {COMBINE_KEPT, 0}, {1, 0}, {1, 1}, {1, 2}, {COMBINE_KEPT, 1} }, -1,
                       SEC, device, ALIGN, false, &plan, &why, &tl),
          "more than four partitions in all are refused");
}

static void caseKeepDialog()
{
    printf("keeping the device's partitions, in the dialog\n");
    const unsigned long long device = 40000;
    writeFile("combinetest-keep.img", keptGptDevice(device));
    writeFile("combinetest-keepa.img", gptImage(9000, 34, {
        {2048, 5000, LINUX, "A8A8A8A8-A8A8-A8A8-A8A8-A8A8A8A8A8A8", "new", 'a', 0} },
        "A9A9A9A9-A9A9-A9A9-A9A9-A9A9A9A9A9A9"));
    writeFile("combinetest-keepbare.img", fat32Image(device, 'z'));
    CombineDialog dlg(NULL, fakeDevices({ {94, "[Disk 94] keep", "keep", device * SEC} }),
                      fakeSectorSize, 94, false, SEC, ".", QStringList("*.*"));
    QString standin = "combinetest-keep.img";
    dlg.setDiskPath([&](int n) { return n == 94 ? standin : QString(); });
    dlg.addImageFiles({ "combinetest-keepa.img" });
    QTreeWidget *images = NULL;
    for (QTreeWidget *t : dlg.findChildren<QTreeWidget *>())
    {
        if (t->headerItem()->text(0) == "Source / partition") images = t;
    }
    images->topLevelItem(0)->child(0)->setCheckState(0, Qt::Checked);
    QCheckBox *keep = NULL;
    for (QCheckBox *c : dlg.findChildren<QCheckBox *>())
    {
        if (c->text() == "Keep the device's partitions") keep = c;
    }
    QListWidget *order = dlg.findChild<QListWidget *>();
    QSpinBox *freesize = dlg.findChild<QSpinBox *>();
    QPushButton *up = NULL, *insert = NULL, *removefree = NULL;
    for (QPushButton *b : dlg.findChildren<QPushButton *>())
    {
        if (b->text() == "Up") up = b;
        if (b->text() == "Insert") insert = b;
        if (b->text() == "Remove free space") removefree = b;
    }
    check(keep && order && freesize && up && insert && removefree, "the controls are there");
    if (!keep || !order || !freesize || !up || !insert || !removefree) return;
    check(!dlg.keepsDevice() && dlg.planIsValid() && !dlg.plan().keepsTarget,
          "it starts replacing the device's table");

    keep->setChecked(true);
    check(dlg.keepsDevice() && dlg.planIsValid() && dlg.plan().keepsTarget
          && dlg.plan().partitionCount() == 3, "ticked, the device's two partitions join the new one");
    check(order->count() == 3 && order->item(0)->text().endsWith("-- kept")
          && order->item(1)->text().endsWith("-- kept")
          && order->item(0)->checkState() == Qt::Checked,
          "  listed first in the order, ticked, as kept");
    check(dlg.plan().placed[2].first == 24576, "  the new one after them");
    QComboBox *lead = NULL;
    for (QComboBox *c : dlg.findChildren<QComboBox *>())
    {
        if (c->findData(-1) >= 0 && c->itemText(0).startsWith("None")) lead = c;
    }
    check(lead && !lead->isEnabled(), "  and no lead-in can be chosen");

    order->setCurrentRow(1);
    check(!up->isEnabled(), "one kept partition cannot move past another");
    order->setCurrentRow(2);
    check(up->isEnabled(), "  a new one can");
    up->click();
    check(dlg.planIsValid() && dlg.plan().placed[1].first == 6144 && dlg.plan().placed[1].image == 0,
          "  and moved up, goes into the space between them");

    order->item(0)->setCheckState(Qt::Unchecked);
    check(order->count() == 3 && order->item(0)->text().endsWith("-- taken out of the table")
          && dlg.plan().partitionCount() == 2, "unticked, a kept partition is taken out of the table");

    order->setCurrentRow(0);
    freesize->setValue(2);
    insert->click();
    check(order->count() == 4 && order->item(1)->text() == "Free space: 2 MiB"
          && order->currentRow() == 1, "free space is inserted after the selected item");
    check(dlg.planIsValid() && dlg.plan().placed[1].first == 6144,
          "  2 MiB of it, then the new partition");
    freesize->setValue(3);
    check(order->item(1)->text() == "Free space: 3 MiB" && dlg.plan().placed[1].first == 8192,
          "  selected, its size is changed in the box");
    removefree->click();
    check(order->count() == 3 && dlg.plan().placed[0].first == 2048,
          "  and removed, the partition moves back");

    keep->setChecked(false);
    check(order->count() == 1 && !dlg.keepsDevice() && !dlg.plan().keepsTarget && lead->isEnabled(),
          "unticked, the device's partitions leave the order");

    standin = "combinetest-keepbare.img";
    keep->setChecked(true);
    check(!dlg.keepsDevice() && !dlg.planIsValid(), "a device with no partition table has none to keep");
    DeleteFileA("combinetest-keep.img");
    DeleteFileA("combinetest-keepa.img");
    DeleteFileA("combinetest-keepbare.img");
}

static void caseNewDiskId()
{
    printf("a new disk GUID or signature\n");
    const QByteArray a = gptImage(9000, 34, {
        {2048, 5000, LINUX, "E7E7E7E7-E7E7-E7E7-E7E7-E7E7E7E7E7E7", "a", 'a', 0} },
        "AAAAAAAA-AAAA-AAAA-AAAA-AAAAAAAAAAAA");
    QList<ImageLayout> ls(1);
    layoutOf(a, &ls[0]);
    const QByteArray aguid = guid("AAAAAAAA-AAAA-AAAA-AAAA-AAAAAAAAAAAA");
    const unsigned long long device = 20000;
    CombinePlan plan;
    QString why;
    bool ok = planCombine(ls, { {0, 0} }, 0, SEC, device, ALIGN, false, &plan, &why, NULL, true);
    QByteArray dev = apply(plan, {a}, device);
    const unsigned char *h = (const unsigned char *)dev.constData() + SEC;
    const unsigned char *bh = (const unsigned char *)dev.constData() + (device - 1) * SEC;
    check(ok && memcmp(h + 56, aguid.constData(), 16) != 0, "with a lead-in, a new disk GUID");
    check(memcmp(bh + 56, h + 56, 16) == 0 && backupAtEnd(dev, device),
          "  the backup header has it too, and checks out");
    check(memcmp(dev.constData(), a.constData(), 440) == 0 && sameSectors(dev, 34, a, 34, 2048 - 34),
          "  the lead-in's boot code and lead-in are still copied");
    ok = planCombine(ls, { {0, 0} }, 0, SEC, device, ALIGN, false, &plan, &why);
    dev = apply(plan, {a}, device);
    check(ok && memcmp(dev.constData() + SEC + 56, aguid.constData(), 16) == 0,
          "  without it, the lead-in's own");

    const QByteArray m = mbrImage(8192, { {0, 2048, 3000, 0x83, false, 'm'} }, 0x11112222u);
    QList<ImageLayout> lm(1);
    layoutOf(m, &lm[0]);
    ok = planCombine(lm, { {0, 0} }, 0, SEC, device, ALIGN, false, &plan, &why, NULL, true);
    dev = apply(plan, {m}, device);
    check(ok && rd32((const unsigned char *)dev.constData(), 440) != 0x11112222u
          && memcmp(dev.constData(), m.constData(), 440) == 0,
          "an MBR lead-in: a new disk signature, its boot code kept");

    const QByteArray t = keptGptDevice(40000);
    ImageLayout tl;
    layoutOf(t, &tl);
    ok = planCombine(ls, { {COMBINE_KEPT, 0}, {0, 0}, {COMBINE_KEPT, 1} }, -1, SEC, 40000, ALIGN,
                     false, &plan, &why, &tl, true);
    dev = applyOnto(plan, {a}, t);
    check(ok && memcmp(dev.constData() + SEC + 56,
                       guid("DDDDDDDD-DDDD-DDDD-DDDD-DDDDDDDDDDDD").constData(), 16) != 0
          && memcmp(gptEntry(dev, 0), t.constData() + 2 * SEC, 128) == 0
          && backupAtEnd(dev, 40000),
          "a kept device: a new disk GUID, its partitions' entries unchanged");

    // The dialog: only with something to take a disk ID from.
    writeFile("combinetest-diskid.img", a);
    CombineDialog dlg(NULL, fakeDevices({ {96, "[Disk 96] id", "id", device * SEC} }),
                      fakeSectorSize, 96, false, SEC, ".", QStringList("*.*"));
    dlg.addImageFiles({ "combinetest-diskid.img" });
    QTreeWidget *images = NULL;
    for (QTreeWidget *tw : dlg.findChildren<QTreeWidget *>())
    {
        if (tw->headerItem()->text(0) == "Source / partition") images = tw;
    }
    images->topLevelItem(0)->child(0)->setCheckState(0, Qt::Checked);
    QCheckBox *newid = NULL;
    for (QCheckBox *c : dlg.findChildren<QCheckBox *>())
    {
        if (c->text() == "New disk GUID") newid = c;
    }
    QComboBox *lead = NULL;
    for (QComboBox *c : dlg.findChildren<QComboBox *>())
    {
        if (c->count() && c->itemText(0).startsWith("None")) lead = c;
    }
    check(newid && lead && !newid->isEnabled(), "no lead-in: the box is off, a new table has its own");
    if (!newid || !lead) return;
    lead->setCurrentIndex(lead->findData(0));
    check(newid->isEnabled() && dlg.planIsValid()
          && memcmp(dlg.plan().headerregion.constData() + SEC + 56, aguid.constData(), 16) == 0,
          "a lead-in: the box can be ticked, and unticked its disk GUID is used");
    newid->setChecked(true);
    check(dlg.planIsValid()
          && memcmp(dlg.plan().headerregion.constData() + SEC + 56, aguid.constData(), 16) != 0,
          "  ticked, the plan has a new one");
    lead->setCurrentIndex(0);
    check(!newid->isEnabled(), "  and with the lead-in gone, the box is off again");
    DeleteFileA("combinetest-diskid.img");
}

// nativeEvent is protected: this makes it callable, to hand the dialog a drop.
struct DropDialog : CombineDialog
{
    using CombineDialog::CombineDialog;
    using CombineDialog::nativeEvent;
};

// An HDROP as Explorer builds one: a DROPFILES header, then each name as a
// wide string, and an empty one to end the list. DragFinish frees it.
static HDROP makeDrop(const QStringList &names)
{
    std::wstring list;
    for (const QString &name : names)
    {
        list += QDir::toNativeSeparators(QFileInfo(name).absoluteFilePath()).toStdWString();
        list += L'\0';
    }
    list += L'\0';
    const SIZE_T bytes = sizeof(DROPFILES) + list.size() * sizeof(wchar_t);
    HGLOBAL mem = GlobalAlloc(GHND, bytes);
    DROPFILES *df = (DROPFILES *)GlobalLock(mem);
    df->pFiles = sizeof(DROPFILES);
    df->fWide = TRUE;
    memcpy((char *)df + sizeof(DROPFILES), list.data(), list.size() * sizeof(wchar_t));
    GlobalUnlock(mem);
    return (HDROP)mem;
}

static bool sendDrop(DropDialog &dlg, const QStringList &names)
{
    MSG msg = {};
    msg.message = WM_DROPFILES;
    msg.wParam = (WPARAM)makeDrop(names);
    qintptr result = -1;
    return dlg.nativeEvent("windows_generic_MSG", &msg, &result) && result == 0;
}

static void caseDrop()
{
    printf("dropping image files on the dialog\n");
    const QByteArray a = gptImage(9000, 34, {
        {2048, 5000, LINUX, "D1D1D1D1-D1D1-D1D1-D1D1-D1D1D1D1D1D1", "one", 'a', 0} },
        "DDDDDDDD-DDDD-DDDD-DDDD-DDDDDDDDDDD1");
    const QByteArray b = gptImage(9000, 34, {
        {2048, 5000, LINUX, "D2D2D2D2-D2D2-D2D2-D2D2-D2D2D2D2D2D2", "two", 'b', 0} },
        "DDDDDDDD-DDDD-DDDD-DDDD-DDDDDDDDDDD2");
    writeFile("combinetest-drop1.img", a);
    writeFile("combinetest-drop2.img.gz", gzipOf(b));
    DropDialog dlg(NULL, fakeDevices({ {90, "test device", "test device", 200000 * SEC} }),
                   fakeSectorSize, 90, false, SEC, ".", QStringList("*.*"));
    QLineEdit *outfile = outFileEdit(dlg);
    check(outfile && outfile->placeholderText().isEmpty() && outfile->text().isEmpty(),
          "the image file name starts blank, with no example name");

    check(sendDrop(dlg, { "combinetest-drop1.img" }) && dlg.imagePaths().isEmpty(),
          "a drop while the dialog is not the modal window on top is ignored");

    dlg.setModal(true);
    dlg.show();
    check(sendDrop(dlg, { "combinetest-drop1.img", ".", "combinetest-drop2.img.gz" }),
          "a drop of several files and a folder is taken");
    const QStringList paths = dlg.imagePaths();
    check(paths.size() == 2
          && QFileInfo(paths.value(0)).fileName() == "combinetest-drop1.img"
          && QFileInfo(paths.value(1)).fileName() == "combinetest-drop2.img.gz",
          "  both image files are added as sources, in order, and the folder is not");
    QTreeWidget *images = NULL;
    for (QTreeWidget *t : dlg.findChildren<QTreeWidget *>())
    {
        if (t->headerItem()->text(0) == "Source / partition") images = t;
    }
    check(images && images->topLevelItemCount() == 2
          && images->topLevelItem(1)->childCount() == 1,
          "  and listed with their partitions");
    dlg.hide();
    DeleteFileA("combinetest-drop1.img");
    DeleteFileA("combinetest-drop2.img.gz");
}

// ---------------------------------------------------------------- Check Boot ---

static void put16(QByteArray &b, int o, unsigned int v)
{
    b[o] = (char)(v & 0xFF);
    b[o + 1] = (char)((v >> 8) & 0xFF);
}
static void put32(QByteArray &b, int o, unsigned int v) { wr32((unsigned char *)b.data(), o, v); }
static void put64(QByteArray &b, int o, unsigned long long v) { wr64((unsigned char *)b.data(), o, v); }
static QByteArray utf16(const QString &s)
{
    return QByteArray((const char *)s.utf16(), s.size() * 2);
}

// A registry hive, built cell by cell: keys after their subkeys.
struct HiveBuilder
{
    QByteArray bins = QByteArray(32, 0);

    HiveBuilder() { memcpy(bins.data(), "hbin", 4); }

    unsigned int cell(const QByteArray &data)
    {
        const int size = (data.size() + 4 + 7) & ~7;
        const unsigned int off = (unsigned int)bins.size();
        QByteArray c(size, 0);
        put32(c, 0, (unsigned int)(-size));
        memcpy(c.data() + 4, data.constData(), (size_t)data.size());
        bins.append(c);
        return off;
    }
    unsigned int value(const char *name, unsigned int type, const QByteArray &data)
    {
        QByteArray vk(0x14 + (int)strlen(name), 0);
        memcpy(vk.data(), "vk", 2);
        put16(vk, 2, (unsigned int)strlen(name));
        if (data.size() <= 4)
        {
            put32(vk, 4, 0x80000000u | (unsigned int)data.size());
            memcpy(vk.data() + 8, data.constData(), (size_t)data.size());
        }
        else
        {
            put32(vk, 4, (unsigned int)data.size());
            put32(vk, 8, cell(data));
        }
        put32(vk, 0x0C, type);
        put16(vk, 0x10, 1);
        memcpy(vk.data() + 0x14, name, strlen(name));
        return cell(vk);
    }
    unsigned int key(const QString &name, const QList<unsigned int> &subkeys,
                     const QList<unsigned int> &values = {})
    {
        unsigned int list = 0xFFFFFFFF, vlist = 0xFFFFFFFF;
        if (!subkeys.isEmpty())
        {
            QByteArray l(4 + 8 * subkeys.size(), 0);
            memcpy(l.data(), "lf", 2);
            put16(l, 2, (unsigned int)subkeys.size());
            for (int i = 0; i < subkeys.size(); ++i) put32(l, 4 + 8 * i, subkeys[i]);
            list = cell(l);
        }
        if (!values.isEmpty())
        {
            QByteArray l(4 * values.size(), 0);
            for (int i = 0; i < values.size(); ++i) put32(l, 4 * i, values[i]);
            vlist = cell(l);
        }
        const QByteArray n = name.toLatin1();
        QByteArray nk(0x4C + n.size(), 0);
        memcpy(nk.data(), "nk", 2);
        put16(nk, 2, 0x20);
        put32(nk, 0x14, (unsigned int)subkeys.size());
        put32(nk, 0x1C, list);
        put32(nk, 0x24, (unsigned int)values.size());
        put32(nk, 0x28, vlist);
        put16(nk, 0x48, (unsigned int)n.size());
        memcpy(nk.data() + 0x4C, n.constData(), (size_t)n.size());
        return cell(nk);
    }
    QByteArray hive(unsigned int root) const
    {
        QByteArray base(4096, 0);
        memcpy(base.data(), "regf", 4);
        put32(base, 0x24, root);
        QByteArray b = bins;
        b.resize((b.size() + 4095) & ~4095);
        put32(base, 0x28, (unsigned int)b.size());
        return base + b;
    }
};

// A partition device as a BCD element holds it: the options object's GUID,
// then the descriptor -- type 6, flags, size, reserved, the partition (GPT
// unique GUID, or MBR offset in bytes), the disk's kind (0), the partition
// style (0 GPT, 1 MBR), and the disk ID.
static QByteArray partDescriptor(const QByteArray &partition, const QByteArray &disk)
{
    QByteArray d(0x38, 0);
    put32(d, 0, 6);
    put32(d, 8, 0x48);
    memcpy(d.data() + 0x10, partition.constData(), (size_t)partition.size());
    put32(d, 0x24, (disk.size() == 16) ? 0 : 1);
    memcpy(d.data() + 0x28, disk.constData(), (size_t)disk.size());
    return d;
}
static QByteArray partElement(const QByteArray &partition, const QByteArray &disk)
{
    return QByteArray(16, 0) + partDescriptor(partition, disk);
}
// A ramdisk: its options object's GUID, a descriptor of another kind, the
// partition its file is on inside it, and the file.
static QByteArray ramdiskElement(const QByteArray &options, const QByteArray &partition,
                                 const QByteArray &disk, const QString &path)
{
    QByteArray outer(0x1C, 0);
    put32(outer, 0, 3);
    return options + outer + partDescriptor(partition, disk) + utf16(path) + QByteArray(2, 0);
}

struct BcdObject
{
    const char *guid;
    unsigned int type;
    QString description;
    QList<QPair<unsigned int, QByteArray>> devices;
};

static QByteArray bcdHive(const QList<BcdObject> &objects)
{
    HiveBuilder h;
    QList<unsigned int> objkeys;
    for (const BcdObject &o : objects)
    {
        QList<unsigned int> elements;
        if (!o.description.isEmpty())
        {
            elements.append(h.key("12000004", {}, { h.value("Element", 1, utf16(o.description) + QByteArray(2, 0)) }));
        }
        for (const auto &d : o.devices)
        {
            elements.append(h.key(QString("%1").arg(d.first, 8, 16, QChar('0')), {},
                                  { h.value("Element", 3, d.second) }));
        }
        QByteArray type(4, 0);
        put32(type, 0, o.type);
        const unsigned int desc = h.key("Description", {}, { h.value("Type", 4, type) });
        objkeys.append(h.key(o.guid, { desc, h.key("Elements", elements) }));
    }
    const unsigned int objs = h.key("Objects", objkeys);
    return h.hive(h.key("NewStoreRoot", { h.key("Description", {}), objs }));
}

// A FAT32 ESP of `sectors` 512-byte sectors, a cluster each, holding
// \EFI\Microsoft\Boot\BCD; "Microsoft" has only a long name.
static QByteArray fat32Esp(unsigned long long sectors, const QByteArray &bcd)
{
    const unsigned int fatsize = (unsigned int)((sectors * 4 + 511) / 512);
    const unsigned int firstdata = 32 + 2 * fatsize;
    QByteArray img((int)(sectors * 512), 0);
    QByteArray bs(512, 0);
    bs[0] = (char)0xEB; bs[1] = 0x58; bs[2] = (char)0x90;
    memcpy(bs.data() + 3, "MSWIN4.1", 8);
    put16(bs, 11, 512);
    bs[13] = 1;
    put16(bs, 14, 32);
    bs[16] = 2;
    bs[21] = (char)0xF8;
    put32(bs, 32, (unsigned int)sectors);
    put32(bs, 36, fatsize);
    put32(bs, 44, 2);
    memcpy(bs.data() + 82, "FAT32   ", 8);
    bs[510] = 0x55; bs[511] = (char)0xAA;
    memcpy(img.data(), bs.constData(), 512);

    QByteArray fat((int)(fatsize * 512), 0);
    put32(fat, 0, 0x0FFFFFF8);
    put32(fat, 4, 0x0FFFFFFF);
    for (unsigned int c = 2; c <= 5; ++c) put32(fat, (int)(c * 4), 0x0FFFFFFF);
    const unsigned int bcdclusters = (unsigned int)((bcd.size() + 511) / 512);
    // The BCD's clusters out of order, as a file written in pieces is.
    QList<unsigned int> chain;
    for (unsigned int i = 0; i < bcdclusters; ++i) chain.append(6 + ((i % 2) ? i + 20 : i));
    for (int i = 0; i < chain.size(); ++i)
    {
        put32(fat, (int)(chain[i] * 4), (i + 1 < chain.size()) ? chain[i + 1] : 0x0FFFFFFF);
    }
    for (int f = 0; f < 2; ++f) memcpy(img.data() + (32 + f * fatsize) * 512, fat.constData(), (size_t)fat.size());
    auto cluster = [&](unsigned int c) { return img.data() + (firstdata + c - 2) * 512; };
    auto entry = [](char *at, const char *name11, unsigned char attr, unsigned int c, unsigned int size) {
        memcpy(at, name11, 11);
        at[11] = (char)attr;
        at[20] = (char)((c >> 16) & 0xFF); at[21] = (char)((c >> 24) & 0xFF);
        at[26] = (char)(c & 0xFF); at[27] = (char)((c >> 8) & 0xFF);
        wr32((unsigned char *)at, 28, size);
    };
    // Root: a volume label, a deleted entry, then EFI.
    entry(cluster(2), "ESP        ", 0x08, 0, 0);
    entry(cluster(2) + 32, "\xE5OLD    TXT", 0x20, 9, 10);
    entry(cluster(2) + 64, "EFI        ", 0x10, 3, 0);
    entry(cluster(3), ".          ", 0x10, 3, 0);
    entry(cluster(3) + 32, "..         ", 0x10, 0, 0);
    // "Microsoft": one long-name piece, then its short name.
    char *lfn = cluster(3) + 64;
    lfn[0] = 0x41;
    lfn[11] = 0x0F;
    const QString longname = "Microsoft";
    static const int at[13] = { 1, 3, 5, 7, 9, 14, 16, 18, 20, 22, 24, 28, 30 };
    for (int i = 0; i < 13; ++i)
    {
        const unsigned int ch = (i < longname.size()) ? longname[i].unicode()
                              : (i == longname.size()) ? 0x0000 : 0xFFFF;
        lfn[at[i]] = (char)(ch & 0xFF);
        lfn[at[i] + 1] = (char)(ch >> 8);
    }
    entry(cluster(3) + 96, "MICROS~1   ", 0x10, 4, 0);
    entry(cluster(4), "BOOT       ", 0x10, 5, 0);
    entry(cluster(5), "BCD        ", 0x20, chain.first(), (unsigned int)bcd.size());
    for (int i = 0; i < chain.size(); ++i)
    {
        memcpy(cluster(chain[i]), bcd.constData() + i * 512, (size_t)qMin(512, bcd.size() - i * 512));
    }
    return img;
}

// A small NTFS volume holding \Boot\BCD: 4 KiB clusters, 1 KiB file records.
// The root's index has its entries in an allocation block, beside a stale
// one no node points at; the BCD's data is in two pieces.
static QByteArray ntfsVolume(unsigned long long sectors, const QByteArray &bcd,
                             const QByteArray &system = QByteArray())
{
    const int CL = 4096, REC = 1024;
    QByteArray img((int)(sectors * 512), 0);
    QByteArray bs(512, 0);
    bs[0] = (char)0xEB; bs[1] = 0x52; bs[2] = (char)0x90;
    memcpy(bs.data() + 3, "NTFS    ", 8);
    put16(bs, 0x0B, 512);
    bs[0x0D] = 8;
    put64(bs, 0x28, sectors - 1);
    put64(bs, 0x30, 4);                 // the MFT, at cluster 4
    bs[0x40] = (char)0xF6;              // 2^10-byte records
    bs[0x44] = 1;                       // a cluster per index block
    bs[510] = 0x55; bs[511] = (char)0xAA;
    memcpy(img.data(), bs.constData(), 512);

    auto fixup = [](QByteArray &b, int usaoff) {
        const int count = b.size() / 512 + 1;
        put16(b, 4, (unsigned int)usaoff);
        put16(b, 6, (unsigned int)count);
        put16(b, usaoff, 7);
        for (int i = 1; i < count; ++i)
        {
            b[usaoff + 2 * i] = b[i * 512 - 2];
            b[usaoff + 2 * i + 1] = b[i * 512 - 1];
            put16(b, i * 512 - 2, 7);
        }
    };
    auto attr = [](unsigned int type, const QString &name, const QByteArray &value) {
        const int nameoff = 0x18, valoff = (nameoff + name.size() * 2 + 7) & ~7;
        QByteArray a((valoff + value.size() + 7) & ~7, 0);
        put32(a, 0, type);
        put32(a, 4, (unsigned int)a.size());
        a[9] = (char)name.size();
        put16(a, 0x0A, nameoff);
        memcpy(a.data() + nameoff, name.utf16(), (size_t)name.size() * 2);
        put32(a, 0x10, (unsigned int)value.size());
        put16(a, 0x14, (unsigned int)valoff);
        memcpy(a.data() + valoff, value.constData(), (size_t)value.size());
        return a;
    };
    auto nonres = [](unsigned int type, const QString &name, const QByteArray &runs,
                     unsigned long long clusters, unsigned long long size) {
        const int nameoff = 0x40, runoff = (nameoff + name.size() * 2 + 7) & ~7;
        QByteArray a((runoff + runs.size() + 1 + 7) & ~7, 0);
        put32(a, 0, type);
        put32(a, 4, (unsigned int)a.size());
        a[8] = 1;
        a[9] = (char)name.size();
        put16(a, 0x0A, nameoff);
        memcpy(a.data() + nameoff, name.utf16(), (size_t)name.size() * 2);
        put64(a, 0x18, clusters - 1);
        put16(a, 0x20, (unsigned int)runoff);
        put64(a, 0x28, clusters * 4096);
        put64(a, 0x30, size);
        put64(a, 0x38, size);
        memcpy(a.data() + runoff, runs.constData(), (size_t)runs.size());
        return a;
    };
    auto record = [&](int n, bool dir, const QList<QByteArray> &attrs) {
        QByteArray r(REC, 0);
        memcpy(r.data(), "FILE", 4);
        put16(r, 0x14, 0x38);
        put16(r, 0x16, dir ? 3 : 1);
        int o = 0x38;
        for (const QByteArray &a : attrs)
        {
            memcpy(r.data() + o, a.constData(), (size_t)a.size());
            o += a.size();
        }
        put32(r, o, 0xFFFFFFFF);
        put32(r, 0x18, (unsigned int)o + 8);
        put32(r, 0x1C, REC);
        fixup(r, 0x30);
        memcpy(img.data() + 4 * CL + n * REC, r.constData(), REC);
    };
    // An index entry naming `name`, record `rec`; or the end, with or
    // without a node below.
    auto ientry = [](const QString &name, unsigned long long rec) {
        QByteArray key(0x42 + name.size() * 2, 0);
        put64(key, 0, 5);
        key[0x40] = (char)name.size();
        key[0x41] = 1;
        memcpy(key.data() + 0x42, name.utf16(), (size_t)name.size() * 2);
        QByteArray e((16 + key.size() + 7) & ~7, 0);
        put64(e, 0, rec | (1ull << 48));
        put16(e, 8, (unsigned int)e.size());
        put16(e, 10, (unsigned int)key.size());
        memcpy(e.data() + 16, key.constData(), (size_t)key.size());
        return e;
    };
    auto iend = [](bool child, unsigned long long vcn) {
        QByteArray e(child ? 24 : 16, 0);
        put16(e, 8, (unsigned int)e.size());
        put16(e, 12, child ? 3 : 2);
        if (child) put64(e, 16, vcn);
        return e;
    };
    auto node = [](const QByteArray &entries, unsigned int flags) {
        QByteArray h(16, 0);
        put32(h, 0, 16);
        put32(h, 4, 16 + (unsigned int)entries.size());
        put32(h, 8, 16 + (unsigned int)entries.size());
        put32(h, 12, flags);
        return h + entries;
    };
    auto indexroot = [&](const QByteArray &entries, unsigned int flags) {
        QByteArray v(16, 0);
        put32(v, 0, 0x30);
        put32(v, 4, 1);
        put32(v, 8, 4096);
        v[12] = 1;
        return attr(0x90, "$I30", v + node(entries, flags));
    };
    auto indx = [&](unsigned long long lcn, unsigned long long vcn, const QByteArray &entries) {
        QByteArray b(CL, 0);
        memcpy(b.data(), "INDX", 4);
        put64(b, 0x10, vcn);
        QByteArray n = node(entries, 0);
        put32(n, 0, 0x28);              // entries 0x28 after the node header
        QByteArray padded = n.left(16) + QByteArray(0x18, 0) + n.mid(16);
        put32(padded, 4, 0x28 + (unsigned int)entries.size());
        put32(padded, 8, 0x28 + (unsigned int)entries.size());
        memcpy(b.data() + 0x18, padded.constData(), (size_t)padded.size());
        fixup(b, 0x28);
        memcpy(img.data() + lcn * CL, b.constData(), CL);
    };

    // $MFT: four clusters at 4. The root: its index's one node at VCN 0
    // (cluster 10); cluster 11 holds a stale block naming a record not used.
    QByteArray mftruns;
    mftruns.append((char)0x11).append((char)4).append((char)4);
    record(0, false, { nonres(0x80, QString(), mftruns, 4, 16 * REC) });
    QByteArray rootruns;
    rootruns.append((char)0x11).append((char)2).append((char)10);
    record(5, true, { indexroot(iend(true, 0), 1), nonres(0xA0, "$I30", rootruns, 2, 2 * CL) });
    // Entries of 96, 128 and 136 bytes from 0x40 put "Boot"'s at 424, its
    // name across bytes 510-511, which the block's fixup replaces.
    indx(10, 0, ientry("$MFT", 0) + ientry(QString(20, 'A'), 1) + ientry(QString(24, 'B'), 2)
                    + ientry(bcd.isEmpty() ? "Boox" : "Boot", 11) + (system.isEmpty() ? QByteArray() : ientry("Windows", 6))
                    + iend(false, 0));
    if (!system.isEmpty())
    {
        // \Windows\System32\config\SYSTEM, records 6 to 9, its data from cluster 60.
        record(6, true, { indexroot(ientry("System32", 7) + iend(false, 0), 0) });
        record(7, true, { indexroot(ientry("config", 8) + ientry("winload.efi", 10) + iend(false, 0), 0) });
        // System32's winload.efi, record 10, a cluster at 70.
        QByteArray winload;
        winload.append((char)0x11).append((char)1).append((char)70);
        record(10, false, { nonres(0x80, QString(), winload, 1, 512) });
        memcpy(img.data() + 70 * CL, "MZ", 2);
        record(8, true, { indexroot(ientry("SYSTEM", 9) + iend(false, 0), 0) });
        const unsigned long long n = (system.size() + CL - 1) / CL;
        QByteArray runs;
        runs.append((char)0x11).append((char)n).append((char)60);
        record(9, false, { nonres(0x80, QString(), runs, n, (unsigned long long)system.size()) });
        memcpy(img.data() + 60 * CL, system.constData(), (size_t)system.size());
    }
    indx(11, 1, ientry("Boot", 14) + iend(false, 0));
    record(11, true, { indexroot(ientry("BCD", 12) + iend(false, 0), 0) });
    // The BCD, unless there is none: its first cluster at 20, the rest from 30.
    if (!bcd.isEmpty())
    {
        const unsigned long long clusters = (bcd.size() + CL - 1) / CL;
        QByteArray bcdruns;
        bcdruns.append((char)0x11).append((char)1).append((char)20);
        bcdruns.append((char)0x11).append((char)(clusters - 1)).append((char)10);
        record(12, false, { nonres(0x80, QString(), bcdruns, clusters, (unsigned long long)bcd.size()) });
        memcpy(img.data() + 20 * CL, bcd.constData(), (size_t)qMin(CL, bcd.size()));
        if (bcd.size() > CL)
        {
            memcpy(img.data() + 30 * CL, bcd.constData() + CL, (size_t)(bcd.size() - CL));
        }
    }
    return img;
}

static BootSectorReader readerOf(const QByteArray &disk)
{
    return [disk](unsigned long long first, unsigned long long count, char *buf) {
        if ((first + count) * SEC > (unsigned long long)disk.size()) return false;
        memcpy(buf, disk.constData() + first * SEC, (size_t)(count * SEC));
        return true;
    };
}

static const char *BOOTMGR = "{9dea862c-5cdd-4e70-acc1-f32b344d4795}";
static const char *LOADER = "{1d1e3a6c-0000-4000-8000-00000000c0de}";
static const char *WINRE = "{1d1e3a6c-0000-4000-8000-00000000beef}";
static const char *RAMOPT = "{7619dcc8-fafe-11d9-b411-000476eba25f}";

static void caseBootCheck()
{
    printf("Check Boot\n");
    // The filesystem readers, on their own.
    QByteArray payload(3000, 0);
    for (int i = 0; i < payload.size(); ++i) payload[i] = (char)(i * 7 + 3);
    const QByteArray esp = fat32Esp(70000, payload);
    const BootSectorReader espread = readerOf(esp);
    QByteArray got;
    QString why;
    check(fatReadFile(espread, SEC, 0, 70000, { "efi", "MICROSOFT", "boot", "bcd" }, &got, &why)
          && got == payload, "FAT32: a file found by long and short names, its clusters in pieces");
    check(!fatReadFile(espread, SEC, 0, 70000, { "EFI", "Boot", "BCD" }, &got, &why)
          && why.contains("EFI\\Boot"), "  a path it does not have is reported");
    QByteArray big(9000, 0);
    for (int i = 0; i < big.size(); ++i) big[i] = (char)(i * 13 + 1);
    const QByteArray ntfs = ntfsVolume(800, big);
    const BootSectorReader ntfsread = readerOf(ntfs);
    check(ntfsReadFile(ntfsread, SEC, 0, 800, { "boot", "BCD" }, &got, &why) && got == big,
          "NTFS: a file found through the index, its data in two runs");
    if (got != big) printf("    (%s)\n", why.toLocal8Bit().constData());
    check(!ntfsReadFile(ntfsread, SEC, 0, 800, { "Windows" }, &got, &why),
          "  a name only a stale index block has is not found");

    // A copy's GPT disk: its BCD names the original's disk GUID.
    const QByteArray oldid = guid("0D0D0D0D-0000-4000-8000-0000000000AA");
    const QByteArray newid = guid("0E0E0E0E-0000-4000-8000-0000000000BB");
    const QByteArray espuid = guid("E5E5E5E5-0000-4000-8000-000000000001");
    const QByteArray winuid = guid("A1A1A1A1-0000-4000-8000-000000000002");
    const QByteArray reuid = guid("ABABABAB-0000-4000-8000-000000000003");
    auto bcdFor = [&](const QByteArray &disk, const QByteArray &winpart) {
        return bcdHive({
            { BOOTMGR, 0x10100002, "Windows Boot Manager", { { 0x11000001, partElement(espuid, disk) } } },
            { LOADER, 0x10200003, "Windows 11", { { 0x11000001, partElement(winpart, disk) },
                                                  { 0x21000001, partElement(winpart, disk) } } },
            { WINRE, 0x10200003, "Windows Recovery Environment",
              { { 0x11000001, ramdiskElement(guid("7619DCC8-FAFE-11D9-B411-000476EBA25F"), reuid, disk,
                                             "\\Recovery\\WindowsRE\\Winre.wim") } } },
            { RAMOPT, 0x30000000, QString(), { { 0x31000003, partElement(reuid, disk) } } },
        });
    };
    const unsigned long long espsectors = 70000, espfirst = 2048;
    const unsigned long long winfirst = 73728, refirst = 77824;
    auto gptDisk = [&](const QByteArray &bcd, const QByteArray &system = QByteArray()) {
        QByteArray dev = gptImage(90000, 34, {
            {espfirst, espfirst + espsectors - 1, ESP, "E5E5E5E5-0000-4000-8000-000000000001", "EFI", 'e', 0},
            {winfirst, winfirst + 4095, BASIC, "A1A1A1A1-0000-4000-8000-000000000002", "Windows", 'w', 0},
            {refirst, refirst + 4095, WINRE_PART, "ABABABAB-0000-4000-8000-000000000003", "Recovery", 'r', 0} },
            "0E0E0E0E-0000-4000-8000-0000000000BB");
        const QByteArray fs = fat32Esp(espsectors, bcd);
        memcpy(dev.data() + espfirst * SEC, fs.constData(), (size_t)fs.size());
        if (!system.isEmpty())
        {
            const QByteArray win = ntfsVolume(4096, QByteArray(5000, 'x'), system);
            memcpy(dev.data() + winfirst * SEC, win.constData(), (size_t)win.size());
        }
        return dev;
    };
    // A SYSTEM hive whose MountedDevices gives `letter` to partition `part`.
    auto systemFor = [](const QByteArray &part, const char *letter = "\\DosDevices\\C:") {
        HiveBuilder h;
        const unsigned int md = h.key("MountedDevices", {},
                                      { h.value(letter, 3, QByteArray("DMIO:ID:") + part) });
        return h.hive(h.key("ROOT", { h.key("Select", {}), md }));
    };
    BootCheckDisk disk;
    disk.number = 3;
    disk.sectorsize = SEC;
    disk.sectors = 90000;
    disk.diskWithId = [&](const QByteArray &id) { return (id == oldid) ? QString("Disk 0") : QString(); };

    QList<BcdDeviceRef> refs;
    int objects = 0;
    check(parseBcdHive(bcdFor(oldid, winuid), &refs, &objects, &why) && objects == 4 && refs.size() == 5,
          "the BCD hive: four objects, five partitions named");
    bool ramdisk = false;
    for (const BcdDeviceRef &r : refs)
    {
        ramdisk = ramdisk || (r.nested && r.path == "\\Recovery\\WindowsRE\\Winre.wim"
                              && r.options == RAMOPT && r.partition == reuid && r.disk == oldid);
    }
    check(ramdisk, "  the recovery ramdisk's partition, file and options found inside it");

    QByteArray dev = gptDisk(bcdFor(oldid, winuid));
    ImageLayout layout;
    layoutOf(dev, &layout);
    BootCheckResult r = checkBoot(layout, readerOf(dev), disk);
    const QString &t = r.report;
    check(r.bcdFound && !r.ok, "a copy whose disk GUID Windows changed: found, and not bootable");
    // COMBINETEST_BOOTREPORT=file.txt saves this report, to read.
    if (!qEnvironmentVariableIsEmpty("COMBINETEST_BOOTREPORT"))
    {
        QFile f(qEnvironmentVariable("COMBINETEST_BOOTREPORT"));
        if (f.open(QIODevice::WriteOnly)) f.write(t.toUtf8());
    }
    check(t.contains("WRONG") && !t.contains("MISSING") && t.contains("Disk 0 on this computer"),
          "  every entry wrong, the original named");
    check(t.contains("select disk 3") &&t.contains("select partition 1\nassign letter=S")
          && t.contains("select partition 2\nassign letter=W")
          && t.contains("select partition 3\nassign letter=R"),
          "  diskpart gives the ESP, Windows and recovery partitions letters");
    const QString store = "bcdedit /store S:\\EFI\\Microsoft\\Boot\\BCD /set ";
    check(t.contains(store + BOOTMGR + " device partition=S:")
          && t.contains(store + LOADER + " device partition=W:")
          && t.contains(store + LOADER + " osdevice partition=W:")
          && t.contains(store + RAMOPT + " ramdisksdidevice partition=R:")
          && t.contains(store + WINRE + " device ramdisk=[R:]\\Recovery\\WindowsRE\\Winre.wim," + RAMOPT),
          "  and bcdedit sets each entry");
    check(t.contains("bcdboot W:\\Windows /s S: /f UEFI"), "  or bcdboot rebuilds it");
    if (!t.contains("WRONG")) printf("%s\n", t.toLocal8Bit().constData());

    dev = gptDisk(bcdFor(newid, winuid));
    r = checkBoot(layout, readerOf(dev), disk);
    check(r.bcdFound && r.ok && !r.report.contains("bcdedit"), "a BCD naming the disk as it is: it matches");
    check(!r.duplicate, "  and no other disk shares its IDs");

    // The same disk beside its original, which has the same IDs.
    BootCheckDisk beside = disk;
    beside.others = { { "Disk 0", newid, { espuid, winuid } }, { "Disk 5", guid("55555555-0000-4000-8000-000000000005"), {} } };
    r = checkBoot(layout, readerOf(dev), beside);
    check(r.duplicate && !r.ok && r.report.contains("Disk 0 on this computer has the same disk GUID, and 2 of the same "
                                                    "partition GUIDs")
          && !r.report.contains("Disk 5") && !r.report.contains("bcdedit"),
          "a copy beside its original: found, the original named, New IDs its fix");
    beside.others = { { "Disk 0", guid("55555555-0000-4000-8000-000000000005"), { winuid } } };
    r = checkBoot(layout, readerOf(dev), beside);
    check(r.duplicate && r.report.contains("Disk 0 on this computer has 1 of the same partition GUIDs"),
          "  a partition GUID alone shared: found too");

    dev = gptDisk(bcdFor(newid, guid("99999999-0000-4000-8000-000000000009")));
    r = checkBoot(layout, readerOf(dev), disk);
    check(!r.ok && r.report.contains("MISSING") && r.report.contains("bcdboot"),
          "a partition the disk does not have: missing, and bcdboot offered");
    check(r.report.contains("select partition 2\nassign letter=W")
          && r.report.contains("ren S:\\EFI\\Microsoft\\Boot\\BCD BCD.old\nbcdboot W:\\Windows /s S: /f UEFI"),
          "  nothing for bcdedit to set: Windows found by type, and bcdboot is the fix");
    check(r.report.contains("select volume C\nselect partition 3\nassign letter=R")
          && r.report.contains("robocopy R:\\Recovery\\WindowsRE C:\\WinREBackup Winre.wim /copy:DAT\n"
                               "ren C:\\Windows\\System32\\Recovery\\ReAgent.xml ReAgent.xml.old\n"
                               "reagentc /setreimage /path R:\\Recovery\\WindowsRE /target C:\\Windows\n"
                               "reagentc /enable")
          && r.report.contains("shutdown /r /o /t 0") && r.report.contains("remove letter=R"),
          "  and reagentc re-registers recovery after, on the recovery partition found by its type");

    dev = gptDisk(bcdFor(oldid, guid("99999999-0000-4000-8000-000000000009")));
    r = checkBoot(layout, readerOf(dev), disk);
    check(!r.ok && r.report.contains("select partition 2\nassign letter=W")
          && r.report.contains("bcdboot W:\\Windows /s S: /f UEFI") && !r.report.contains("/set "),
          "  and the same with the disk GUID changed too");

    // Windows' drive letters, in the copy's SYSTEM hive.
    const QByteArray oldwin = guid("99999999-0000-4000-8000-000000000009");
    dev = gptDisk(bcdFor(newid, winuid), systemFor(oldwin));
    r = checkBoot(layout, readerOf(dev), disk);
    const QString regfix = "reg load HKLM\\CLONE W:\\Windows\\System32\\config\\SYSTEM\n"
                           "reg delete HKLM\\CLONE\\MountedDevices /va /f\n"
                           "reg add HKLM\\CLONE\\MountedDevices /v \\DosDevices\\C: /t REG_BINARY /d 444D494F3A49443A"
                           + QString(winuid.toHex().toUpper()) + " /f\nreg unload HKLM\\CLONE";
    check(!r.ok && r.report.contains("C: is recorded as partition {99999999-") && r.report.contains(regfix)
          && r.report.contains("select partition 2\nassign letter=W") && !r.report.contains("bcdboot")
          && !r.report.contains("letter=S"),
          "a BCD that matches, but C: names a partition the disk does not have: reg records C: for its own");
    if (!r.report.contains(regfix)) printf("%s\n", r.report.toLocal8Bit().constData());
    dev = gptDisk(bcdFor(newid, winuid), systemFor(winuid, "\\DosDevices\\D:"));
    r = checkBoot(layout, readerOf(dev), disk);
    check(!r.ok && r.report.contains("no C: is recorded") && r.report.contains(regfix),
          "  and no C: at all, Windows on D:: the same");
    dev = gptDisk(bcdFor(newid, winuid), systemFor(winuid));
    r = checkBoot(layout, readerOf(dev), disk);
    check(r.ok && r.report.contains("C: is partition 2"), "  and C: naming its own partition: it matches");

    // Letters the partitions have here are used; S: taken, the next free one.
    BootCheckDisk here = disk;
    here.partitionLetters.insert(winfirst, 'E');
    here.freeLetters = "FGHRTW";
    dev = gptDisk(bcdFor(oldid, oldwin), systemFor(oldwin));
    r = checkBoot(layout, readerOf(dev), here);
    check(r.report.contains("select partition 1\nassign letter=F") && !r.report.contains("select partition 2")
          && r.report.contains("ren F:\\EFI\\Microsoft\\Boot\\BCD BCD.old\nbcdboot E:\\Windows /s F: /f UEFI")
          && r.report.contains("reg load HKLM\\CLONE E:\\Windows"),
          "on this computer: a partition's own letter kept, a free one for the rest, both fixes");

    // Windows on a disk with nothing to start it: one NTFS partition, no ESP.
    auto dataDisk = [&](unsigned long long sectors) {
        QByteArray d = gptImage(sectors, 34, {
            {2048, 2048 + 40000 - 1, BASIC, "A1A1A1A1-0000-4000-8000-000000000002", "Data", 'w', 0} },
            "0E0E0E0E-0000-4000-8000-0000000000BB");
        const QByteArray win = ntfsVolume(4096, QByteArray(), systemFor(oldwin));
        memcpy(d.data() + 2048 * SEC, win.constData(), (size_t)win.size());
        return d;
    };
    dev = dataDisk(90000);
    ImageLayout datalayout;
    layoutOf(dev, &datalayout);
    r = checkBoot(datalayout, readerOf(dev), disk);
    const QString &dt = r.report;
    check(!r.ok && !r.bcdFound && dt.contains("Windows is on partition 1")
          && dt.contains("no EFI system partition"),
          "Windows on a disk with no EFI system partition: found, and not bootable");
    if (!dt.contains("Windows is on")) printf("%s\n", dt.toLocal8Bit().constData());
    check(dt.contains("select disk 3\nlist partition\nselect partition 1\nassign letter=W\n"
                      "shrink desired=1092 minimum=1092\n"
                      "create partition efi size=200\nformat quick fs=fat32 label=\"System\"\nassign letter=S\n"
                      "create partition primary size=892\nformat quick fs=ntfs label=\"Recovery\"\n"
                      "set id=\"de94bba4-06d1-4d40-a16a-bfd50179d6ac\"\ngpt attributes=0x8000000000000001\n"
                      "assign letter=R\nexit")
          && dt.contains("bcdboot W:\\Windows /s S: /f UEFI\n"
                         "robocopy W:\\Windows\\System32\\Recovery R:\\Recovery\\WindowsRE Winre.wim /copy:DAT"),
          "  no room: Windows shrunk, an EFI and a recovery partition made, bcdboot, Winre.wim copied");
    check(dt.contains("reg add HKLM\\CLONE\\MountedDevices /v \\DosDevices\\C:") && dt.contains("reagentc /enable"),
          "  and its C: recorded, and the recovery environment registered once it starts");
    dev = dataDisk(4000000);
    layoutOf(dev, &datalayout);
    BootCheckDisk roomy = disk;
    roomy.sectors = 4000000;
    r = checkBoot(datalayout, readerOf(dev), roomy);
    check(!r.report.contains("shrink") && r.report.contains("create partition efi size=200"),
          "  with room on the disk: nothing shrunk");

    // An 80 GiB VM disk whose EFI, reserved and recovery partitions were
    // deleted: Windows from 217 MiB, 888 MiB free after it, 887 from its 1 MiB boundary. Read sparsely.
    const unsigned long long vmsectors = 80ull * 1024 * 2048, winat = 217 * 2048;
    const unsigned long long winend = vmsectors - 33 - 888 * 2048 - 100;
    ImageLayout vm = datalayout;
    vm.partitions[0].first = winat;
    vm.partitions[0].sectors = winend - winat;
    const QByteArray winvol = ntfsVolume(4096, QByteArray(), systemFor(oldwin));
    const BootSectorReader sparse = [&](unsigned long long first, unsigned long long count, char *buf) {
        memset(buf, 0, (size_t)(count * SEC));
        for (unsigned long long s = 0; s < count; ++s)
        {
            const unsigned long long at = first + s;
            if (at >= winat && at < winat + 4096) memcpy(buf + s * SEC, winvol.constData() + (at - winat) * SEC, SEC);
        }
        return first + count <= vmsectors;
    };
    BootCheckDisk vmdisk = disk;
    vmdisk.sectors = vmsectors;
    r = checkBoot(vm, sparse, vmdisk);
    check(!r.report.contains("shrink")
          && r.report.contains("create partition efi size=200 offset=1024\n")
          && r.report.contains("create partition msr size=16 offset=205824\n")
          && r.report.contains("create partition primary size=887 offset="),
          "  its free space used as it is: EFI and reserved in front of Windows, recovery after, nothing shrunk");
    if (!r.report.contains("create partition primary size=887 offset=") || !r.report.contains("msr size=16 offset=205824"))
    {
        printf("%s\n", r.report.toLocal8Bit().constData());
    }

    // A BIOS disk: the BCD on the active NTFS partition, by offset and signature.
    const unsigned int oldsig = 0x11223344u, newsig = 0x55667788u;
    auto sig = [](unsigned int s) { QByteArray b(4, 0); put32(b, 0, s); return b; };
    auto off = [](unsigned long long o) { QByteArray b(8, 0); put64(b, 0, o); return b; };
    auto mbrDisk = [&](unsigned int bcdsig) {
        const QByteArray bcd = bcdHive({
            { BOOTMGR, 0x10100002, "Windows Boot Manager", { { 0x11000001, partElement(off(2048 * SEC), sig(bcdsig)) } } },
            { LOADER, 0x10200003, "Windows 10", { { 0x21000001, partElement(off(4096 * SEC), sig(bcdsig)) } } } });
        QByteArray d = mbrImage(12000, { {0, 2048, 800, 0x07, true, 's'}, {1, 4096, 6000, 0x07, false, 'w'} },
                                newsig);
        const QByteArray fs = ntfsVolume(800, bcd);
        memcpy(d.data() + 2048 * SEC, fs.constData(), (size_t)fs.size());
        return d;
    };
    dev = mbrDisk(oldsig);
    layoutOf(dev, &layout);
    r = checkBoot(layout, readerOf(dev), disk);
    check(r.bcdFound && !r.ok && r.report.contains("11223344") && r.report.contains("55667788")
          && r.report.contains("bcdedit /store S:\\Boot\\BCD /set " + QString(LOADER) + " osdevice partition=W:")
          && r.report.contains("/f BIOS"),
          "an MBR disk: the BCD on its NTFS system partition names the old signature");
    dev = mbrDisk(newsig);
    r = checkBoot(layout, readerOf(dev), disk);
    check(r.bcdFound && r.ok, "  and with its own, matches");
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
    caseDuplicatePrompt(false);
    caseDuplicatePrompt(true);
    caseDiskPaths();
    caseDeviceChoice();
    caseDrop();
    caseFreeSpace();
    caseKeepGpt();
    caseKeepMbr();
    caseKeepDialog();
    caseNewDiskId();
    caseBootCheck();
    // COMBINETEST_MAINSHOT=file.png saves the main window's form, as laid
    // out at its smallest, to look at.
    if (!qEnvironmentVariableIsEmpty("COMBINETEST_MAINSHOT"))
    {
        QMainWindow w;
        Ui::MainWindow form;
        form.setupUi(&w);
        w.resize(w.minimumSizeHint());
        w.show();
        w.grab().save(qEnvironmentVariable("COMBINETEST_MAINSHOT"));
    }
    DeleteFileA(TESTFILE);
    printf("\n%d checks, %d failures\n", checks, failures);
    return failures ? 1 : 0;
}
