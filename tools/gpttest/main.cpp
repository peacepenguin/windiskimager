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

// Exercises relocateBackupGPT() against a file standing in for a device, so the
// GPT repair can be changed without an SD card, a VM or a UAC prompt. The real
// disk.cpp is linked in: rawSeekRead/rawSeekWrite go through ReadFile/WriteFile,
// which work on a plain file handle exactly as they do on a raw device.
//
// It builds a GPT whose backup sits mid-"device", the way writing a small image
// to a larger card leaves it, relocates it, and checks what came out. The cases
// that matter most are the ones where nothing should be written: this code
// zeroes sectors, and a guard that stops working would quietly destroy data.
//
//   cmake -S tools/gpttest -B build-gpttest -G Ninja
//   cmake --build build-gpttest && ./build-gpttest/gpttest.exe
//
// Exits non-zero if any check fails. See BUILD.md.

#include <QCoreApplication>
#include <QByteArray>
#include <QString>
#include <windows.h>
#include <cstdio>
#include <cstring>
#include "disk.h"
#include "mainwindow.h"

// disk.cpp parents its message boxes on this. Nothing here creates a window, so
// a null instance is all the definition that is needed to link.
MainWindow *MainWindow::instance = NULL;

static const unsigned long long SEC = 512;
static const unsigned long long ENTRIES = 128;
static const unsigned long long ENTRYSIZE = 128;
static const unsigned long long ENTRYSECTORS = (ENTRIES * ENTRYSIZE) / SEC;   // 32

// GPT field offsets, from the UEFI specification. disk.cpp states them too,
// and that repetition is the point: the harness builds its fixtures from these
// and disk.cpp reads them back through its own. Sharing one set of constants
// would leave a wrong offset agreeing with itself and the tests still passing.
// Header:
enum { H_MYLBA = 24, H_ALTLBA = 32, H_FIRSTUSABLE = 40, H_LASTUSABLE = 48,
       H_ENTRYLBA = 72, H_HEADERCRC = 16 };
// Partition entry:
enum { P_START = 32, P_END = 40 };

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

static unsigned int crc32of(const unsigned char *d, size_t n)
{
    static unsigned int tbl[256];
    static bool init = false;
    if (!init)
    {
        for (unsigned int i = 0; i < 256; ++i)
        {
            unsigned int c = i;
            for (int k = 0; k < 8; ++k) c = (c & 1) ? (0xEDB88320u ^ (c >> 1)) : (c >> 1);
            tbl[i] = c;
        }
        init = true;
    }
    unsigned int c = 0xFFFFFFFFu;
    for (size_t i = 0; i < n; ++i) c = tbl[(c ^ d[i]) & 0xFF] ^ (c >> 8);
    return c ^ 0xFFFFFFFFu;
}

static int failures = 0;
static int checks = 0;
static void check(bool ok, const char *what)
{
    printf("  %s %s\n", ok ? "ok  " : "FAIL", what);
    ++checks;
    if (!ok) ++failures;
}

static void makeHeader(unsigned char *h, unsigned long long hdrlba,
                       unsigned long long altlba, unsigned long long entrylba,
                       unsigned long long firstusable, unsigned long long lastusable,
                       unsigned int entriescrc)
{
    memset(h, 0, SEC);
    memcpy(h, "EFI PART", 8);
    wr32(h, 8, 0x00010000);      // revision 1.0
    wr32(h, 12, 92);             // header size
    wr64(h, H_MYLBA, hdrlba);
    wr64(h, H_ALTLBA, altlba);
    wr64(h, H_FIRSTUSABLE, firstusable);
    wr64(h, H_LASTUSABLE, lastusable);
    wr64(h, H_ENTRYLBA, entrylba);
    wr32(h, 80, (unsigned int)ENTRIES);
    wr32(h, 84, (unsigned int)ENTRYSIZE);
    wr32(h, 88, entriescrc);
    wr32(h, H_HEADERCRC, 0);
    wr32(h, H_HEADERCRC, crc32of(h, 92));
}

static bool headerCrcValid(const unsigned char *h)
{
    QByteArray probe((const char *)h, 92);
    wr32((unsigned char *)probe.data(), H_HEADERCRC, 0);
    return crc32of((const unsigned char *)probe.constData(), 92) == rd32(h, H_HEADERCRC);
}

// One test device: the bytes, the entry array written into it, and where the
// image left its backup GPT.
struct Disk
{
    QByteArray bytes;
    QByteArray entries;
    unsigned long long imglast, imgbackupentries;
};

// A GPT written as if an image of imagesectors had just been written to a card
// of devicesectors, so the backup lands mid-device. partend, when non-zero,
// overrides where the single partition ends -- used to park a partition on top
// of the stale backup.
static Disk buildDisk(unsigned long long firstusable, unsigned long long imagesectors,
                      unsigned long long devicesectors, unsigned long long partend = 0)
{
    Disk dk;
    dk.bytes = QByteArray(devicesectors * SEC, 0);
    unsigned char *d = (unsigned char *)dk.bytes.data();

    dk.imglast = imagesectors - 1;
    dk.imgbackupentries = dk.imglast - ENTRYSECTORS;
    unsigned long long imglastusable = dk.imgbackupentries - 1;

    dk.entries = QByteArray(ENTRIES * ENTRYSIZE, 0);
    unsigned char *e = (unsigned char *)dk.entries.data();
    memset(e, 0xAB, 16);                 // type GUID: any non-zero value
    memset(e + 16, 0xCD, 16);            // unique GUID
    wr64(e, P_START, firstusable);
    wr64(e, P_END, partend ? partend : imglastusable - 1);
    unsigned int ecrc = crc32of((const unsigned char *)dk.entries.constData(),
                                ENTRIES * ENTRYSIZE);

    // Protective MBR spanning the image only, as the image left it.
    d[446 + 4] = 0xEE;
    wr32(d, 446 + 8, 1);
    wr32(d, 446 + 12, (unsigned int)dk.imglast);
    d[510] = 0x55; d[511] = 0xAA;

    makeHeader(d + SEC, 1, dk.imglast, 2, firstusable, imglastusable, ecrc);
    memcpy(d + 2 * SEC, dk.entries.constData(), ENTRIES * ENTRYSIZE);
    makeHeader(d + dk.imglast * SEC, dk.imglast, 1, dk.imgbackupentries,
               firstusable, imglastusable, ecrc);
    memcpy(d + dk.imgbackupentries * SEC, dk.entries.constData(), ENTRIES * ENTRYSIZE);
    return dk;
}

static const char *TESTFILE = "gpttest.img";

// Writes the disk to a file, runs the repair, reads the result back.
static GptFixResult runRepair(const Disk &dk, unsigned long long devicesectors,
                              QByteArray *after, QString *detail)
{
    // Sized before anything can fail: every caller indexes into this, and on
    // the early return below a default-constructed QByteArray would be read
    // past its end.
    *after = QByteArray(devicesectors * SEC, 0);

    DeleteFileA(TESTFILE);
    HANDLE h = CreateFileA(TESTFILE, GENERIC_READ | GENERIC_WRITE, 0, NULL,
                           CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE)
    {
        printf("  FAIL could not create %s\n", TESTFILE);
        ++failures;
        return GPT_FIX_FAILED;
    }
    // Checked, or a short write would leave the test device holding something
    // other than the case meant to set up, and the checks would be measuring
    // the wrong thing while still passing.
    DWORD put = 0;
    if (!WriteFile(h, dk.bytes.constData(), (DWORD)dk.bytes.size(), &put, NULL)
        || put != (DWORD)dk.bytes.size())
    {
        printf("  FAIL could not write the test device\n");
        ++failures;
        CloseHandle(h);
        return GPT_FIX_FAILED;
    }

    GptFixResult r = relocateBackupGPT(h, SEC, devicesectors, detail);

    SetFilePointer(h, 0, NULL, FILE_BEGIN);
    DWORD got = 0;
    if (!ReadFile(h, after->data(), (DWORD)after->size(), &got, NULL)
        || got != (DWORD)after->size())
    {
        printf("  FAIL could not read the test device back\n");
        ++failures;
    }
    CloseHandle(h);
    return r;
}

static bool rangeIsZero(const unsigned char *a, unsigned long long first,
                        unsigned long long last)
{
    for (unsigned long long s = first; s <= last; ++s)
    {
        for (unsigned long long i = 0; i < SEC; ++i)
        {
            if (a[s * SEC + i] != 0) return false;
        }
    }
    return true;
}

// The ordinary path: a backup stranded mid-device is moved to the end and the
// stale copy is cleared.
static void caseRelocate(const char *name, unsigned long long firstusable,
                         unsigned long long imagesectors, unsigned long long devicesectors)
{
    printf("%s (FirstUsableLBA=%llu, image=%llu sectors, device=%llu sectors)\n",
           name, firstusable, imagesectors, devicesectors);

    Disk dk = buildDisk(firstusable, imagesectors, devicesectors);
    QByteArray after;
    QString detail;
    GptFixResult r = runRepair(dk, devicesectors, &after, &detail);
    printf("  -> %s\n", detail.toLocal8Bit().constData());

    const unsigned char *a = (const unsigned char *)after.constData();
    unsigned long long lastlba = devicesectors - 1;
    unsigned long long newentries = lastlba - ENTRYSECTORS;

    check(r == GPT_FIX_OK, "returned GPT_FIX_OK");

    const unsigned char *p = a + SEC;
    check(memcmp(p, "EFI PART", 8) == 0, "primary is still a GPT header");
    check(headerCrcValid(p), "primary header CRC is valid");
    check(rd64(p, H_ALTLBA) == lastlba, "primary AlternateLBA is the last LBA");
    check(rd64(p, H_LASTUSABLE) == newentries - 1, "primary LastUsableLBA extended");
    // The whole point of the repair: these two must survive untouched, or the
    // mismatch that triggers Windows' rewrite is reintroduced.
    check(rd64(p, H_FIRSTUSABLE) == firstusable, "primary FirstUsableLBA untouched");
    check(rd64(p, H_ENTRYLBA) == 2, "primary PartitionEntryLBA untouched");

    const unsigned char *b = a + lastlba * SEC;
    check(memcmp(b, "EFI PART", 8) == 0, "backup header sits at the last LBA");
    check(headerCrcValid(b), "backup header CRC is valid");
    check(rd64(b, H_MYLBA) == lastlba, "backup MyLBA is the last LBA");
    check(rd64(b, H_ALTLBA) == 1, "backup AlternateLBA is 1");
    check(rd64(b, H_ENTRYLBA) == newentries, "backup entry array precedes it");
    check(memcmp(a + newentries * SEC, dk.entries.constData(), ENTRIES * ENTRYSIZE) == 0,
          "backup entry array copied");

    check(rangeIsZero(a, dk.imgbackupentries, dk.imglast),
          "stale backup GPT and its entry array are zeroed");

    // Verify has to forgive exactly those sectors. It works the range out from
    // the image's own header, because once the repair has run the device no
    // longer says where the stale copy was. If the two disagree, a good card
    // fails verification at the first sector the repair cleared.
    unsigned long long vfirst = 0, vlast = 0;
    bool vknown = gptImageBackupRange((const unsigned char *)dk.bytes.constData() + SEC,
                                      SEC, &vfirst, &vlast);
    check(vknown && vfirst == dk.imgbackupentries && vlast == dk.imglast,
          "verify forgives exactly the sectors the repair zeroed");
    check(memcmp(a + dk.imglast * SEC, "EFI PART", 8) != 0,
          "no stray EFI PART signature mid-device");

    check(rd32(a, 446 + 12) == (unsigned int)lastlba, "protective MBR spans the device");
    check(memcmp(a + 2 * SEC, dk.entries.constData(), ENTRIES * ENTRYSIZE) == 0,
          "primary entry array untouched");
    printf("\n");
}

// A partition sitting on top of where the stale backup is. The relocate must
// still happen, but nothing inside the partition may be zeroed.
static void caseStaleUnderPartition()
{
    const unsigned long long firstusable = 34, image = 4096, device = 16384;
    printf("stale copy covered by a partition (nothing may be zeroed)\n");

    Disk dk = buildDisk(firstusable, image, device, /*partend=*/image - 1);
    // Recognisable data across the sectors the cleanup would otherwise clear.
    unsigned char *d = (unsigned char *)dk.bytes.data();
    for (unsigned long long s = dk.imgbackupentries; s <= dk.imglast; ++s)
    {
        if (s == dk.imglast || s == dk.imgbackupentries) continue;   // table sectors
        memset(d + s * SEC, 0x5A, SEC);
    }

    QByteArray after;
    QString detail;
    GptFixResult r = runRepair(dk, device, &after, &detail);
    printf("  -> %s\n", detail.toLocal8Bit().constData());
    const unsigned char *a = (const unsigned char *)after.constData();

    check(r == GPT_FIX_OK, "returned GPT_FIX_OK");
    check(rd64(a + SEC, H_ALTLBA) == device - 1, "backup still relocated to the end");

    // The whole covered range, table sectors included. Checking only the data
    // sectors between them -- which is what this did -- steps over the two
    // sectors the cleanup actually aims at, so a guard that narrowed to just
    // the header, or just the entry array, would have gone unnoticed.
    const unsigned char *b = (const unsigned char *)dk.bytes.constData();
    const size_t covered = (size_t)((dk.imglast - dk.imgbackupentries + 1) * SEC);
    check(memcmp(a + dk.imgbackupentries * SEC,
                 b + dk.imgbackupentries * SEC, covered) == 0,
          "nothing under the partition was touched, stale table sectors included");
    check(memcmp(a + dk.imglast * SEC, "EFI PART", 8) == 0,
          "the stale backup header is still where the partition covers it");
    printf("\n");
}

// No GPT at all, a backup already at the end, and a corrupt header: each must
// leave the device exactly as it was.
// The image very nearly fills the device, so the sectors the stale backup sits
// in and the sectors the new one goes into overlap. The relocate must still
// happen, and the cleanup must not run: zeroing the stale copy would erase the
// table just written over the top of it.
//
// "image nearly fills device" does not reach this: it leaves a 160-sector gap
// between the two, so the guard it is named for never has to do anything.
static void caseStaleOverlapsNewTable()
{
    // 8160 puts the stale header exactly on the first sector of the new entry
    // array, which is the only sector of it that is not zeros. Land it any
    // later and a cleanup that wrongly ran would write zeros over zeros, which
    // no check could detect -- the case would pass whether the guard held or
    // not, and prove nothing.
    const unsigned long long firstusable = 34, image = 8160, device = 8192;
    printf("stale copy overlaps where the new table goes\n");

    Disk dk = buildDisk(firstusable, image, device);
    QByteArray after;
    QString detail;
    GptFixResult r = runRepair(dk, device, &after, &detail);
    printf("  -> %s\n", detail.toLocal8Bit().constData());
    const unsigned char *a = (const unsigned char *)after.constData();

    const unsigned long long lastlba = device - 1;
    const unsigned long long newentries = lastlba - ENTRYSECTORS;
    check(dk.imglast >= newentries, "the two ranges really do overlap");
    check(r == GPT_FIX_OK, "returned GPT_FIX_OK");
    // Checked by checksum, not by memcmp against the expected bytes: entries 1
    // to 127 are zeros, so zeroing a sector in the middle of the array changes
    // nothing a comparison would see. The header's own EntriesCRC covers every
    // byte of it, which is the point -- that is what a GPT reader validates.
    check(crc32of(a + newentries * SEC, ENTRIES * ENTRYSIZE)
              == rd32(a + lastlba * SEC, 88),
          "the backup entry array still matches its header checksum");
    check(memcmp(a + lastlba * SEC, "EFI PART", 8) == 0,
          "backup header sits at the last LBA");
    check(headerCrcValid(a + lastlba * SEC), "backup header CRC is valid");
    printf("\n");
}

static void caseUntouched(const char *name, GptFixResult expect,
                          void (*damage)(unsigned char *, unsigned long long))
{
    const unsigned long long firstusable = 34, image = 4096, device = 16384;
    printf("%s\n", name);

    Disk dk = buildDisk(firstusable, image, device);
    damage((unsigned char *)dk.bytes.data(), device);
    QByteArray before = dk.bytes;

    QByteArray after;
    QString detail;
    GptFixResult r = runRepair(dk, device, &after, &detail);

    check(r == expect, "returned the expected result");
    check(after == before, "the device was left byte-identical");
    printf("\n");
}

static void damageSignature(unsigned char *d, unsigned long long)
{
    memset(d + SEC, 0, 8);                       // wipe "EFI PART"
}
static void damageCrc(unsigned char *d, unsigned long long)
{
    wr32(d + SEC, H_HEADERCRC, 0xDEADBEEF);      // header no longer checksums
}
static void damageEntriesCrc(unsigned char *d, unsigned long long)
{
    // A header whose PartitionEntryArrayCRC32 does not describe the entries it
    // points at. The repair must refuse it rather than compute a fresh
    // checksum over whatever happens to be there.
    unsigned char *h = d + SEC;
    wr32(h, 88, 0xDEADBEEF);
    wr32(h, H_HEADERCRC, 0);
    wr32(h, H_HEADERCRC, crc32of(h, 92));
}
static void damageAlreadyAtEnd(unsigned char *d, unsigned long long device)
{
    // Claim the backup is already at the last LBA: nothing to relocate.
    unsigned char *h = d + SEC;
    wr64(h, H_ALTLBA, device - 1);
    wr32(h, H_HEADERCRC, 0);
    wr32(h, H_HEADERCRC, crc32of(h, 92));
}

// The damage as Windows leaves it, after the fact: PartitionEntryLBA pointing
// where the entries are not, with the header checksum recomputed so the header
// still passes its own CRC. Verify has to be able to see that, and the repair
// has to put it back without touching a data sector.
static void caseAfterTheFact(const char *name, unsigned long long firstusable,
                             bool expectbroken)
{
    const unsigned long long image = 4096, device = 16384;
    printf("%s (FirstUsableLBA=%llu)\n", name, firstusable);

    Disk dk = buildDisk(firstusable, image, device);
    unsigned char *d = (unsigned char *)dk.bytes.data();
    unsigned char *hdr = d + SEC;

    // Exactly what the rescan does. PartitionEntryArrayCRC32 is deliberately
    // left alone: the recorded reproducer shows Windows does not recompute it,
    // which is what makes the real array findable again afterwards.
    wr64(hdr, H_ENTRYLBA, firstusable - ENTRYSECTORS);
    wr32(hdr, H_HEADERCRC, 0);
    wr32(hdr, H_HEADERCRC, crc32of(hdr, 92));
    check(headerCrcValid(hdr), "the mangled header still passes its own CRC");

    DeleteFileA(TESTFILE);
    HANDLE h = CreateFileA(TESTFILE, GENERIC_READ | GENERIC_WRITE, 0, NULL,
                           CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE)
    {
        printf("  FAIL could not create %s\n", TESTFILE);
        ++failures;
        return;
    }
    DWORD put = 0;
    if (!WriteFile(h, dk.bytes.constData(), (DWORD)dk.bytes.size(), &put, NULL)
        || put != (DWORD)dk.bytes.size())
    {
        printf("  FAIL could not write the test device\n");
        ++failures;
        CloseHandle(h);
        return;
    }

    GptPrimaryState st = gptPrimaryState(h, SEC, device);
    check(st == (expectbroken ? GPT_PRIMARY_BROKEN : GPT_PRIMARY_OK),
          expectbroken ? "the broken primary table is detected"
                       : "the harmless rewrite is not reported as damage");

    if (expectbroken)
    {
        QString detail;
        check(repairPrimaryGpt(h, SEC, device, &detail), "the repair reports success");
        printf("  -> %s\n", detail.toLocal8Bit().constData());
        check(gptPrimaryState(h, SEC, device) == GPT_PRIMARY_OK,
              "the primary table is consistent again");

        QByteArray after(device * SEC, 0);
        SetFilePointer(h, 0, NULL, FILE_BEGIN);
        DWORD got = 0;
        ReadFile(h, after.data(), (DWORD)after.size(), &got, NULL);
        const unsigned char *a = (const unsigned char *)after.constData();
        check(rd64(a + SEC, H_ENTRYLBA) == 2, "PartitionEntryLBA points at LBA 2 again");
        check(headerCrcValid(a + SEC), "the repaired header CRC is valid");
        check(rd64(a + SEC, H_FIRSTUSABLE) == firstusable, "FirstUsableLBA untouched");
        check(memcmp(a + 2 * SEC, dk.entries.constData(), ENTRIES * ENTRYSIZE) == 0,
              "the partition entries themselves were not touched");
    }
    CloseHandle(h);
    printf("\n");
}

// ---------------------------------------------------------------------------
// listGptPartitions() / listMbrPartitions() / planGptShrink() / planMbrShrink()
// with excludeSlots -- the "choose partitions to read" machinery. These write
// a plain table straight to the test file (no backup GPT is built; the
// functions under test here never look for one) and drive planGptShrink()/
// planMbrShrink() with alignsectors=1 so the expected sector numbers stay
// small and readable instead of chasing a real 1MiB alignment.
// ---------------------------------------------------------------------------

// One partition to place in a multi-partition GPT test fixture, by table
// slot rather than position -- callers deliberately put slots out of
// position order, since that mismatch is exactly what the ordering bug
// this reproduces (a partition recreated into an earlier freed slot lands
// physically out of order, but still is a real diskpart-visible partition).
struct GptPart
{
    int slot;
    unsigned long long first, last;
    const char *name;   // ASCII; NULL for none
};

static QByteArray buildMultiGptDisk(unsigned long long devicesectors,
                                    unsigned long long firstusable,
                                    const QList<GptPart> &parts)
{
    QByteArray bytes(devicesectors * SEC, 0);
    unsigned char *d = (unsigned char *)bytes.data();

    d[446 + 4] = 0xEE;
    wr32(d, 446 + 8, 1);
    unsigned long long span = devicesectors - 1;
    wr32(d, 446 + 12, (unsigned int)(span > 0xFFFFFFFFull ? 0xFFFFFFFFull : span));
    d[510] = 0x55; d[511] = 0xAA;

    QByteArray entries(ENTRIES * ENTRYSIZE, 0);
    unsigned char *earr = (unsigned char *)entries.data();
    for (const GptPart &p : parts)
    {
        unsigned char *e = earr + p.slot * ENTRYSIZE;
        memset(e, 0xAB, 16);          // type GUID: any non-zero value
        memset(e + 16, 0xCD, 16);     // unique GUID
        wr64(e, P_START, p.first);
        wr64(e, P_END, p.last);
        if (p.name)
        {
            QString name = QString::fromLatin1(p.name);
            memcpy(e + 56, name.utf16(), (size_t)name.size() * 2);
        }
    }
    unsigned int ecrc = crc32of((const unsigned char *)entries.constData(),
                                ENTRIES * ENTRYSIZE);
    makeHeader(d + SEC, 1, devicesectors - 1, 2, firstusable, devicesectors - 2, ecrc);
    memcpy(d + 2 * SEC, entries.constData(), ENTRIES * ENTRYSIZE);
    return bytes;
}

static HANDLE writeTestFile(const QByteArray &bytes)
{
    DeleteFileA(TESTFILE);
    HANDLE h = CreateFileA(TESTFILE, GENERIC_READ | GENERIC_WRITE, 0, NULL,
                           CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE)
    {
        printf("  FAIL could not create %s\n", TESTFILE);
        ++failures;
        return INVALID_HANDLE_VALUE;
    }
    DWORD put = 0;
    if (!WriteFile(h, bytes.constData(), (DWORD)bytes.size(), &put, NULL)
        || put != (DWORD)bytes.size())
    {
        printf("  FAIL could not write the test device\n");
        ++failures;
        CloseHandle(h);
        return INVALID_HANDLE_VALUE;
    }
    return h;
}

// A partition physically out of slot order -- slot 3 sits second on the
// disk -- reproduces the real report this fixed: diskpart (and this
// program) must list it as "Partition 4" in position order on the disk,
// not renumber it to match where it happens to appear in the list.
static void caseListGptOrder()
{
    printf("listGptPartitions() orders by disk position, not table slot\n");
    const unsigned long long device = 2000, firstusable = 34;
    QList<GptPart> parts = {
        {0, 100, 199, "PART1"},
        {1, 400, 499, "PART2"},
        {2, 500, 599, "PART3"},
        {3, 200, 299, "NEWVOL"},
    };
    HANDLE h = writeTestFile(buildMultiGptDisk(device, firstusable, parts));
    if (h == INVALID_HANDLE_VALUE) return;

    QList<PartitionInfo> found;
    QString detail;
    bool ok = listGptPartitions(h, SEC, device, &found, &detail);
    CloseHandle(h);

    check(ok, "reported success");
    check(found.size() == 4, "found all four partitions");
    if (found.size() == 4)
    {
        check(found[0].slot == 0 && found[0].name == "PART1", "1st by position is slot 0 (PART1)");
        check(found[1].slot == 3 && found[1].name == "NEWVOL", "2nd by position is slot 3 (NEWVOL)");
        check(found[2].slot == 1 && found[2].name == "PART2", "3rd by position is slot 1 (PART2)");
        check(found[3].slot == 2 && found[3].name == "PART3", "4th by position is slot 2 (PART3)");
        check(found[0].firstSector == 100 && found[0].sectors == 100,
              "position and size are read correctly, not just the slot");
    }
    printf("\n");
}

// Excluding a partition must repack around the gap it leaves, zero its
// table entry (so the excluded data is unreachable even if something later
// reads past what the ranges cover), and leave every other entry pointing
// at its new, repacked location -- both in the primary table (headerregion)
// and in the fresh backup copy planGptShrink() builds for the shrunk image.
static void caseGptShrinkExclude()
{
    printf("planGptShrink() with an excluded slot\n");
    const unsigned long long device = 2000, firstusable = 34;
    QList<GptPart> parts = {
        {0, 100, 199, "KEEP1"},   // 100 sectors
        {1, 300, 399, "DROP"},    // 100 sectors -- excluded
        {2, 500, 599, "KEEP2"},   // 100 sectors
    };
    HANDLE h = writeTestFile(buildMultiGptDisk(device, firstusable, parts));
    if (h == INVALID_HANDLE_VALUE) return;

    QList<int> excludeSlots = {1};
    PartitionShrinkPlan plan;
    QString detail;
    bool ok = planGptShrink(h, SEC, device, /*alignsectors=*/1ull, &plan, &detail, &excludeSlots);
    CloseHandle(h);
    check(ok, "reported success");
    if (!ok)
    {
        printf("  -> %s\n", detail.toLocal8Bit().constData());
        printf("\n");
        return;
    }

    // Mirrors planGptShrink()'s own packing arithmetic with alignsectors=1,
    // over just the two kept partitions in position order: nothing rounds
    // up past the exact byte, so the expected numbers are exact, not
    // approximate.
    unsigned long long newKeep1First = firstusable;                    // 34
    unsigned long long newKeep2First = newKeep1First + 100;            // 134
    unsigned long long cursor = newKeep2First + 100;                   // 234
    unsigned long long backupentries = cursor;                        // 234
    unsigned long long backuphdr = cursor + ENTRYSECTORS;             // 266

    check(plan.ranges.size() == 2, "only the two kept partitions are copied");
    if (plan.ranges.size() == 2)
    {
        check(plan.ranges[0].srcfirst == 100 && plan.ranges[0].dstfirst == newKeep1First
                  && plan.ranges[0].length == 100,
              "KEEP1 repacked right after FirstUsableLBA");
        check(plan.ranges[1].srcfirst == 500 && plan.ranges[1].dstfirst == newKeep2First
                  && plan.ranges[1].length == 100,
              "KEEP2 repacked right after KEEP1, with no gap for the excluded slot");
    }
    check(plan.totalsectors == backuphdr + 1, "totalsectors covers up to the backup header");
    check(plan.backupsectors == ENTRYSECTORS + 1, "backupsectors is the array plus its header");

    const unsigned char *region = (const unsigned char *)plan.headerregion.constData();
    const unsigned char *entry1 = region + 2 * SEC + 1 * ENTRYSIZE;
    QByteArray zero(ENTRYSIZE, 0);
    check(memcmp(entry1, zero.constData(), ENTRYSIZE) == 0,
          "the excluded slot's entry is zeroed in the primary table");
    const unsigned char *entry0 = region + 2 * SEC + 0 * ENTRYSIZE;
    const unsigned char *entry2 = region + 2 * SEC + 2 * ENTRYSIZE;
    check(rd64(entry0, P_START) == newKeep1First && rd64(entry0, P_END) == newKeep1First + 99,
          "KEEP1's entry points at its repacked location");
    check(rd64(entry2, P_START) == newKeep2First && rd64(entry2, P_END) == newKeep2First + 99,
          "KEEP2's entry points at its repacked location");

    const unsigned char *hdr = region + SEC;
    check(headerCrcValid(hdr), "the primary header checksum was recomputed");
    check(rd32(hdr, 88) == crc32of(region + 2 * SEC, ENTRIES * ENTRYSIZE),
          "the entries checksum matches the patched table, exclusion included");

    // The backup copy planGptShrink() builds is entries (already patched
    // above) followed by the backup header -- the same zeroed/repacked
    // bytes have to show up here too, or a card written from this plan
    // would carry a backup table that still describes the excluded
    // partition.
    const unsigned char *backupentriesbytes = (const unsigned char *)plan.backupregion.constData();
    check(memcmp(backupentriesbytes, region + 2 * SEC, ENTRIES * ENTRYSIZE) == 0,
          "the backup entry array matches the patched primary entries exactly");
    const unsigned char *backuphdrbytes = backupentriesbytes + ENTRYSECTORS * SEC;
    check(headerCrcValid(backuphdrbytes), "the backup header checksum is valid");
    check(rd64(backuphdrbytes, H_MYLBA) == backuphdr, "the backup header's own MyLBA matches its position");
    printf("\n");
}

// One partition per slot to place in a multi-primary-entry MBR test
// fixture, by slot (0-3) rather than position, and an optional partition
// type -- 0xEE marks a protective entry, which walkMbrEntries() must skip
// even though it is a non-zero type byte like any real partition's.
struct MbrPart
{
    int slot;
    unsigned long long first, count;
    unsigned char type;
};

static QByteArray buildMultiMbrDisk(unsigned long long devicesectors,
                                    const QList<MbrPart> &parts)
{
    QByteArray bytes(devicesectors * SEC, 0);
    unsigned char *d = (unsigned char *)bytes.data();
    for (const MbrPart &p : parts)
    {
        unsigned char *e = d + 446 + p.slot * 16;
        e[4] = p.type;
        wr32(e, 8, (unsigned int)p.first);
        wr32(e, 12, (unsigned int)p.count);
    }
    d[510] = 0x55; d[511] = 0xAA;
    return bytes;
}

static void caseListMbrOrder()
{
    printf("listMbrPartitions() orders by disk position, not table slot\n");
    const unsigned long long device = 2000;
    QList<MbrPart> parts = {
        {0, 100, 100, 0x0C},
        {1, 400, 100, 0x0C},
        {2, 500, 100, 0x0C},
        {3, 200, 100, 0x0C},   // physically 2nd, like the GPT case above
    };
    HANDLE h = writeTestFile(buildMultiMbrDisk(device, parts));
    if (h == INVALID_HANDLE_VALUE) return;

    QList<PartitionInfo> found;
    QString detail;
    bool ok = listMbrPartitions(h, SEC, device, &found, &detail);
    CloseHandle(h);

    check(ok, "reported success");
    check(found.size() == 4, "found all four partitions");
    if (found.size() == 4)
    {
        check(found[0].slot == 0, "1st by position is slot 0");
        check(found[1].slot == 3, "2nd by position is slot 3");
        check(found[2].slot == 1, "3rd by position is slot 1");
        check(found[3].slot == 2, "4th by position is slot 2");
    }
    printf("\n");
}

static void caseMbrShrinkExclude()
{
    printf("planMbrShrink() with an excluded slot\n");
    const unsigned long long device = 2000;
    QList<MbrPart> parts = {
        {0, 100, 100, 0x0C},   // KEEP1
        {1, 300, 100, 0x0C},   // DROP -- excluded
        {2, 500, 100, 0x0C},   // KEEP2
    };
    HANDLE h = writeTestFile(buildMultiMbrDisk(device, parts));
    if (h == INVALID_HANDLE_VALUE) return;

    QList<int> excludeSlots = {1};
    PartitionShrinkPlan plan;
    QString detail;
    bool ok = planMbrShrink(h, SEC, device, /*alignsectors=*/1ull, &plan, &detail, &excludeSlots);
    CloseHandle(h);
    check(ok, "reported success");
    if (!ok)
    {
        printf("  -> %s\n", detail.toLocal8Bit().constData());
        printf("\n");
        return;
    }

    unsigned long long newKeep1First = 1ull;      // right after the boot sector
    unsigned long long newKeep2First = newKeep1First + 100;

    check(plan.ranges.size() == 2, "only the two kept partitions are copied");
    if (plan.ranges.size() == 2)
    {
        check(plan.ranges[0].srcfirst == 100 && plan.ranges[0].dstfirst == newKeep1First
                  && plan.ranges[0].length == 100,
              "KEEP1 repacked right after the boot sector");
        check(plan.ranges[1].srcfirst == 500 && plan.ranges[1].dstfirst == newKeep2First
                  && plan.ranges[1].length == 100,
              "KEEP2 repacked right after KEEP1, with no gap for the excluded slot");
    }
    check(plan.backupsectors == 0ull && plan.backupregion.isEmpty(),
          "MBR has no backup table to build");

    const unsigned char *sector0 = (const unsigned char *)plan.headerregion.constData();
    QByteArray zero16(16, 0);
    check(memcmp(sector0 + 446 + 16, zero16.constData(), 16) == 0,
          "the excluded slot's entry is entirely zeroed, not just its type byte");
    check(rd32(sector0 + 446 + 0 * 16, 8) == newKeep1First, "KEEP1's start field was patched");
    check(rd32(sector0 + 446 + 2 * 16, 8) == newKeep2First, "KEEP2's start field was patched");
    printf("\n");
}

// A protective MBR (the one a GPT disk itself carries in sector 0) must
// never be walked as if its 0xEE entry were a real partition to repack or
// exclude -- both listMbrPartitions() and planMbrShrink() are exercised
// here since each has its own call into walkMbrEntries().
static void caseMbrProtectiveEntrySkipped()
{
    printf("walkMbrEntries() skips a 0xEE protective entry\n");
    const unsigned long long device = 2000;
    QList<MbrPart> parts = {
        {0, 1, (unsigned long long)(device - 1), 0xEE},   // protective, spans the device
        {1, 100, 100, 0x0C},                              // the one real partition
    };
    HANDLE h = writeTestFile(buildMultiMbrDisk(device, parts));
    if (h == INVALID_HANDLE_VALUE) { return; }

    QList<PartitionInfo> found;
    QString detail;
    bool listok = listMbrPartitions(h, SEC, device, &found, &detail);
    check(listok, "listMbrPartitions reported success");
    check(found.size() == 1 && found[0].slot == 1,
          "only the real partition is listed, not the protective entry");

    PartitionShrinkPlan plan;
    bool planok = planMbrShrink(h, SEC, device, 1ull, &plan, &detail);
    CloseHandle(h);
    check(planok, "planMbrShrink reported success");
    check(plan.ranges.size() == 1 && plan.ranges[0].srcfirst == 100,
          "only the real partition was packed, the protective entry was not treated as data");
    printf("\n");
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    // crc32of() here is a copy of disk.cpp's gptCrc32(), so every CRC check
    // below would agree with a wrong polynomial or seed just as happily as
    // with a right one. This pins it to the published CRC-32 check value
    // instead, which is what makes the rest of the CRC checks mean anything.
    printf("CRC-32 algorithm\n");
    check(crc32of((const unsigned char *)"123456789", 9) == 0xCBF43926u,
          "matches the standard check value for \"123456789\"");
    printf("\n");

    // Ordinary layout: FirstUsableLBA 34, which the Windows rewrite happens to
    // land on correctly.
    caseRelocate("ordinary layout", 34, 2048, 8192);
    // The layout the repair exists for: space reserved ahead of the first
    // partition, as ARM board images do it.
    caseRelocate("reserved-space layout", 2048, 16384, 65536);
    // Old and new tables nearly touching, to exercise the overlap guards.
    caseRelocate("image nearly fills device", 34, 8000, 8192);

    caseStaleUnderPartition();
    caseStaleOverlapsNewTable();

    caseUntouched("no GPT on the device", GPT_FIX_NO_GPT, damageSignature);
    caseUntouched("backup already at the last LBA", GPT_FIX_NOT_NEEDED, damageAlreadyAtEnd);
    caseUntouched("primary header CRC invalid", GPT_FIX_BAD_GPT, damageCrc);
    caseUntouched("entry array checksum invalid", GPT_FIX_BAD_GPT, damageEntriesCrc);

    // After Windows has already mangled it, with "Fix GPT after write" off.
    caseAfterTheFact("windows rewrote the table (reserved-space layout)", 2048, true);
    caseAfterTheFact("windows rewrote the table (ordinary layout)", 34, false);

    caseListGptOrder();
    caseGptShrinkExclude();
    caseListMbrOrder();
    caseMbrShrinkExclude();
    caseMbrProtectiveEntrySkipped();

    DeleteFileA(TESTFILE);
    printf("%d checks, %d failures\n", checks, failures);
    return failures ? 1 : 0;
}
