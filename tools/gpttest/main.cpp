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

// Exercises disk.cpp's GPT repair (relocateBackupGPT, gptPrimaryState,
// repairPrimaryGpt, gptImageBackupRange) and its partition listing and
// shrink planning against a file standing in for a device. rawSeekRead and
// rawSeekWrite use ReadFile/WriteFile, which work on a plain file exactly as on
// a raw device.
//
// The cases that matter most are the ones where nothing should be written: this
// code zeroes sectors, and a guard that stops working would quietly destroy data.
//
// Run with tools/gpttest.sh; exits non-zero if any check fails.

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

// GPT field offsets, from the UEFI specification. Deliberately not shared with
// disk.cpp: a wrong offset in one shared set would agree with itself and pass.
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

    // Non-zero partition data, so a write or a zeroing that strays into it
    // shows up in a whole-device comparison. Skipped when the partition is
    // parked over the backup table, which would overwrite the table.
    if (!partend)
    {
        // Through imglastusable: the sector right below the stale copy is the
        // first one an off-by-one in its cleanup would hit.
        for (unsigned long long s = firstusable; s <= imglastusable; ++s)
        {
            memset(d + s * SEC, (int)(0x40 + (s % 61)), SEC);
        }
    }
    return dk;
}

// Every sector outside the listed [first, last] ranges must be unchanged.
struct SectorRange { unsigned long long first, last; };
static bool unchangedOutside(const QByteArray &before, const QByteArray &after,
                             const QList<SectorRange> &allowed)
{
    if (before.size() != after.size()) return false;
    const unsigned long long sectors = (unsigned long long)before.size() / SEC;
    for (unsigned long long s = 0; s < sectors; ++s)
    {
        bool isAllowed = false;
        for (const SectorRange &r : allowed)
        {
            if (s >= r.first && s <= r.last) { isAllowed = true; break; }
        }
        if (!isAllowed && memcmp(before.constData() + s * SEC,
                                 after.constData() + s * SEC, SEC) != 0)
        {
            printf("       sector %llu changed\n", s);
            return false;
        }
    }
    return true;
}

static const char *TESTFILE = "gpttest.img";

// Writes the disk to a file, runs the repair, reads the result back.
static GptFixResult runRepair(const Disk &dk, unsigned long long devicesectors,
                              QByteArray *after, QString *detail)
{
    // Sized before anything can fail: every caller indexes into this, even
    // after an early return.
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
    // Checked, or a short write would leave the checks measuring a different
    // case while still passing.
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
    // Changing these two reintroduces the mismatch that triggers Windows' rewrite.
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

    // Verify works the range out from the image's header, since the repaired
    // device no longer says where the stale copy was. If it disagrees with the
    // repair, a good card fails verification.
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
    check(rd32(b, 88) == crc32of(a + newentries * SEC, ENTRIES * ENTRYSIZE),
          "backup header's entries checksum matches the backup array");
    check(rd64(b, H_FIRSTUSABLE) == firstusable && rd64(b, H_LASTUSABLE) == newentries - 1,
          "backup header's usable range matches the primary's");
    check(unchangedOutside(dk.bytes, after,
                           { {0, 1}, {dk.imgbackupentries, dk.imglast}, {newentries, lastlba} }),
          "nothing but the MBR, the primary header, the stale copy and the new "
          "backup was written -- partition data included");
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

    // The whole covered range, including the two table sectors the cleanup
    // actually aims at, so a guard narrowed to just one of them is caught.
    const unsigned char *b = (const unsigned char *)dk.bytes.constData();
    const size_t covered = (size_t)((dk.imglast - dk.imgbackupentries + 1) * SEC);
    check(memcmp(a + dk.imgbackupentries * SEC,
                 b + dk.imgbackupentries * SEC, covered) == 0,
          "nothing under the partition was touched, stale table sectors included");
    check(memcmp(a + dk.imglast * SEC, "EFI PART", 8) == 0,
          "the stale backup header is still where the partition covers it");
    printf("\n");
}

// The stale backup overlaps where the new one goes, so the cleanup must not run:
// zeroing the stale copy would erase the table just written over it. This
// checks the outcome; the guard itself can't be isolated, because the new
// table always overwrites a stale header sitting inside it.
static void caseStaleOverlapsNewTable()
{
    // 8160 puts the stale header exactly on the first sector of the new entry
    // array, its only non-zero sector. Any later and a cleanup that wrongly ran
    // would write zeros over zeros, and the case would prove nothing.
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
    check(dk.imglast >= newentries, "fixture: the two ranges really do overlap");
    check(r == GPT_FIX_OK, "returned GPT_FIX_OK");
    check(!detail.contains("cleared"), "no stale copy is reported cleared");
    // By checksum, the way a GPT reader validates it.
    check(crc32of(a + newentries * SEC, ENTRIES * ENTRYSIZE)
              == rd32(a + lastlba * SEC, 88),
          "the backup entry array still matches its header checksum");
    check(memcmp(a + lastlba * SEC, "EFI PART", 8) == 0,
          "backup header sits at the last LBA");
    check(headerCrcValid(a + lastlba * SEC), "backup header CRC is valid");
    printf("\n");
}

// A device relocateBackupGPT() must refuse or skip: it must stay byte-identical.
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
static void damagePartitionPastEnd(unsigned char *d, unsigned long long device)
{
    // A partition ending where the relocated backup would go. The primary
    // header is re-signed so only the partition check can refuse it.
    unsigned char *h = d + SEC;
    unsigned char *e = d + 2 * SEC;
    wr64(e, P_END, device - 2);
    wr32(h, 88, crc32of(e, ENTRIES * ENTRYSIZE));
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

    // What the rescan does. PartitionEntryArrayCRC32 is left alone because
    // Windows does not recompute it, which is what makes the real array
    // findable again afterwards.
    wr64(hdr, H_ENTRYLBA, firstusable - ENTRYSECTORS);
    wr32(hdr, H_HEADERCRC, 0);
    wr32(hdr, H_HEADERCRC, crc32of(hdr, 92));
    check(headerCrcValid(hdr), "fixture: the mangled header is re-signed, as Windows leaves it");

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
        check(ReadFile(h, after.data(), (DWORD)after.size(), &got, NULL)
                  && got == (DWORD)after.size(),
              "the repaired device was read back");
        const unsigned char *a = (const unsigned char *)after.constData();
        check(unchangedOutside(dk.bytes, after, { {1, 1} }),
              "only the primary header sector was written");
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
// listGptPartitions() / listMbrPartitions(), and planGptShrink() /
// planMbrShrink() with excludeSlots. These fixtures have no backup GPT (none of
// these functions reads one), and alignsectors=1 keeps the expected sector
// numbers exact.
// ---------------------------------------------------------------------------

// Placed by table slot rather than position: a partition recreated into an
// earlier freed slot sits physically out of slot order.
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

// Slot 3 sits second on the disk; diskpart lists partitions in disk order, and
// so must this program.
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

// The excluded slot's entry is zeroed so its data is unreachable even if
// something reads past what the ranges cover; checked in both the primary
// (headerregion) and the backup copy (backupregion).
static void caseGptShrinkExclude()
{
    printf("planGptShrink() with an excluded slot\n");
    const unsigned long long device = 2000, firstusable = 34;
    // DROP is slot 2 but second on disk, so excluding by position instead of
    // by slot would drop KEEP2 and still copy two partitions.
    QList<GptPart> parts = {
        {0, 100, 199, "KEEP1"},   // 100 sectors
        {1, 500, 599, "KEEP2"},   // 100 sectors
        {2, 300, 399, "DROP"},    // 100 sectors -- excluded
    };
    HANDLE h = writeTestFile(buildMultiGptDisk(device, firstusable, parts));
    if (h == INVALID_HANDLE_VALUE) return;

    QList<int> excludeSlots = {2};
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

    // With alignsectors=1: everything before KEEP1 is kept where it is, KEEP1
    // stays put, and KEEP2 closes up behind it over the excluded slot's space.
    const unsigned long long headerend = 2 + ENTRYSECTORS;              // 34
    unsigned long long newKeep1First = 100;                            // unmoved
    unsigned long long newKeep2First = newKeep1First + 100;            // 200
    unsigned long long cursor = newKeep2First + 100;                   // 300
    unsigned long long backupentries = cursor;                        // 300
    unsigned long long backuphdr = cursor + ENTRYSECTORS;             // 332

    check(plan.ranges.size() == 3, "the area before KEEP1, then only the two kept partitions");
    if (plan.ranges.size() == 3)
    {
        check(plan.ranges[0].srcfirst == headerend && plan.ranges[0].dstfirst == headerend
                  && plan.ranges[0].length == 100 - headerend,
              "everything from the table to the first partition is copied where it is");
        check(plan.ranges[1].srcfirst == 100 && plan.ranges[1].dstfirst == newKeep1First
                  && plan.ranges[1].length == 100,
              "KEEP1, the first partition, does not move");
        check(plan.ranges[2].srcfirst == 500 && plan.ranges[2].dstfirst == newKeep2First
                  && plan.ranges[2].length == 100,
              "KEEP2 closes up behind KEEP1, with no gap for the excluded slot");
    }
    check(plan.totalsectors == backuphdr + 1, "totalsectors covers up to the backup header");
    check(plan.backupsectors == ENTRYSECTORS + 1, "backupsectors is the array plus its header");

    check(plan.headersectors == headerend
              && (unsigned long long)plan.headerregion.size() == headerend * SEC,
          "the header region is the MBR, primary header and entry array");

    const unsigned char *region = (const unsigned char *)plan.headerregion.constData();
    const unsigned char *entry2 = region + 2 * SEC + 2 * ENTRYSIZE;
    QByteArray zero(ENTRYSIZE, 0);
    check(memcmp(entry2, zero.constData(), ENTRYSIZE) == 0,
          "the excluded slot's entry is zeroed in the primary table");
    const unsigned char *entry0 = region + 2 * SEC + 0 * ENTRYSIZE;
    const unsigned char *entry1 = region + 2 * SEC + 1 * ENTRYSIZE;
    check(rd64(entry0, P_START) == newKeep1First && rd64(entry0, P_END) == newKeep1First + 99,
          "KEEP1's entry points at its repacked location");
    check(rd64(entry1, P_START) == newKeep2First && rd64(entry1, P_END) == newKeep2First + 99,
          "KEEP2's entry points at its repacked location");

    const unsigned char *hdr = region + SEC;
    check(headerCrcValid(hdr), "the primary header checksum was recomputed");
    check(rd64(hdr, H_ALTLBA) == backuphdr, "primary AlternateLBA points at the new backup");
    check(rd64(hdr, H_LASTUSABLE) == backupentries - 1,
          "primary LastUsableLBA ends before the backup");
    check(rd32(region, 446 + 12) == (unsigned int)backuphdr,
          "protective MBR spans the shrunk image");
    check(rd32(hdr, 88) == crc32of(region + 2 * SEC, ENTRIES * ENTRYSIZE),
          "the entries checksum matches the patched table, exclusion included");

    // backupregion is the entry array followed by the backup header; a stale
    // copy here would still describe the excluded partition.
    const unsigned char *backupentriesbytes = (const unsigned char *)plan.backupregion.constData();
    check(memcmp(backupentriesbytes, region + 2 * SEC, ENTRIES * ENTRYSIZE) == 0,
          "the backup entry array matches the patched primary entries exactly");
    const unsigned char *backuphdrbytes = backupentriesbytes + ENTRYSECTORS * SEC;
    check(headerCrcValid(backuphdrbytes), "the backup header checksum is valid");
    check(rd64(backuphdrbytes, H_MYLBA) == backuphdr, "the backup header's own MyLBA matches its position");
    check(rd64(backuphdrbytes, H_ALTLBA) == 1 && rd64(backuphdrbytes, H_ENTRYLBA) == backupentries,
          "the backup header points back at LBA 1 and at its own entry array");
    printf("\n");
}

// As GptPart, for MBR slots 0-3. type 0xEE marks a protective entry, which
// walkMbrEntries() must skip.
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
    // As the GPT case: the excluded slot is not the one second in slot order.
    QList<MbrPart> parts = {
        {0, 100, 100, 0x0C},   // KEEP1
        {1, 500, 100, 0x0C},   // KEEP2
        {2, 300, 100, 0x0C},   // DROP -- excluded
    };
    HANDLE h = writeTestFile(buildMultiMbrDisk(device, parts));
    if (h == INVALID_HANDLE_VALUE) return;

    QList<int> excludeSlots = {2};
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

    unsigned long long newKeep1First = 100ull;    // the first partition does not move
    unsigned long long newKeep2First = newKeep1First + 100;

    check(plan.ranges.size() == 3, "the area before KEEP1, then only the two kept partitions");
    if (plan.ranges.size() == 3)
    {
        check(plan.ranges[0].srcfirst == 1 && plan.ranges[0].dstfirst == 1
                  && plan.ranges[0].length == 99,
              "everything from the boot sector to the first partition is copied where it is");
        check(plan.ranges[1].srcfirst == 100 && plan.ranges[1].dstfirst == newKeep1First
                  && plan.ranges[1].length == 100,
              "KEEP1, the first partition, does not move");
        check(plan.ranges[2].srcfirst == 500 && plan.ranges[2].dstfirst == newKeep2First
                  && plan.ranges[2].length == 100,
              "KEEP2 closes up behind KEEP1, with no gap for the excluded slot");
    }
    check(plan.backupsectors == 0ull && plan.backupregion.isEmpty(),
          "MBR has no backup table to build");
    check(plan.totalsectors == newKeep2First + 100, "totalsectors ends right after KEEP2");

    const unsigned char *sector0 = (const unsigned char *)plan.headerregion.constData();
    QByteArray zero16(16, 0);
    check(memcmp(sector0 + 446 + 2 * 16, zero16.constData(), 16) == 0,
          "the excluded slot's entry is entirely zeroed, not just its type byte");
    check(rd32(sector0 + 446 + 0 * 16, 8) == newKeep1First, "KEEP1's start field is its unmoved start");
    check(rd32(sector0 + 446 + 1 * 16, 8) == newKeep2First, "KEEP2's start field was patched");
    printf("\n");
}

// A 0xEE entry beside a real one is a hybrid MBR: the disk is a GPT, and the
// real entry only mirrors a GPT partition. Neither caller may treat it as an
// MBR disk, even with no GPT header present to say so; see readValidMbr().
static void caseMbrProtectiveEntryDeclined()
{
    printf("an MBR with a 0xEE entry is not repacked or listed\n");
    const unsigned long long device = 2000;
    QList<MbrPart> parts = {
        {0, 1, (unsigned long long)(device - 1), 0xEE},   // protective, spans the device
        {1, 100, 100, 0x0C},                              // a hybrid mirror of a GPT partition
    };
    HANDLE h = writeTestFile(buildMultiMbrDisk(device, parts));
    if (h == INVALID_HANDLE_VALUE) { return; }

    QList<PartitionInfo> found;
    QString detail;
    check(!listMbrPartitions(h, SEC, device, &found, &detail), "listMbrPartitions declines");
    PartitionShrinkPlan plan;
    const bool planok = planMbrShrink(h, SEC, device, 1ull, &plan, &detail);
    CloseHandle(h);
    check(!planok, "planMbrShrink declines");
    check(detail.contains("GPT"), "and says the device has a GPT");
    printf("\n");
}

// ---------------------------------------------------------------------------
// A shrink plan applied end to end: the image is assembled from the plan the
// way Read writes it (header region, zero gaps, each range copied, backup
// region at the end), then checked with the same readers that would meet it
// on a card.
// ---------------------------------------------------------------------------

static const unsigned long long ALIGN_1MIB = 1048576 / SEC;   // 2048

static void fillSectors(QByteArray &dev, unsigned long long first,
                        unsigned long long count, char value)
{
    memset(dev.data() + first * SEC, value, (size_t)(count * SEC));
}

// Empty, with a failure recorded, if the plan's ranges overlap or run
// backwards -- output this assembly would have to seek back for.
static QByteArray applyPlan(const QByteArray &device, const PartitionShrinkPlan &plan)
{
    QByteArray out = plan.headerregion;
    for (const ShrinkCopyRange &r : plan.ranges)
    {
        if ((unsigned long long)out.size() > r.dstfirst * SEC)
        {
            check(false, "plan ranges are in order and do not overlap");
            return QByteArray();
        }
        out.append(QByteArray((int)(r.dstfirst * SEC - out.size()), 0));
        out.append(device.mid((int)(r.srcfirst * SEC), (int)(r.length * SEC)));
    }
    const unsigned long long backupat = plan.totalsectors - plan.backupsectors;
    if ((unsigned long long)out.size() > backupat * SEC)
    {
        check(false, "the backup region starts after the last range");
        return QByteArray();
    }
    out.append(QByteArray((int)(backupat * SEC - out.size()), 0));
    out.append(plan.backupregion);
    return out;
}

// No whole sector in bytes is filled with value -- the excluded partition's
// data must not reach the image anywhere.
static bool noSectorFilledWith(const QByteArray &bytes, char value)
{
    QByteArray probe(SEC, value);
    for (int off = 0; off + (int)SEC <= bytes.size(); off += (int)SEC)
    {
        if (memcmp(bytes.constData() + off, probe.constData(), SEC) == 0) return false;
    }
    return true;
}

struct KeptPart { unsigned long long sectors; char fill; };

// The partitions listed in the image are the expected ones, in order, each
// starting on a 1MiB boundary and holding its original data.
static void checkImagePartitions(const QByteArray &img, const QList<PartitionInfo> &found,
                                 const QList<KeptPart> &want)
{
    check(found.size() == want.size(), "the image lists exactly the kept partitions");
    if (found.size() != want.size()) return;
    bool aligned = true, sized = true, data = true;
    for (int i = 0; i < found.size(); ++i)
    {
        aligned = aligned && (found[i].firstSector % ALIGN_1MIB) == 0;
        sized = sized && found[i].sectors == want[i].sectors;
        QByteArray expect((int)(want[i].sectors * SEC), want[i].fill);
        data = data && (found[i].firstSector + found[i].sectors) * SEC <= (unsigned long long)img.size()
               && img.mid((int)(found[i].firstSector * SEC), expect.size()) == expect;
    }
    check(aligned, "every partition starts on a 1MiB boundary");
    check(sized, "every partition kept its size");
    check(data, "every partition holds its original data");
}

static void caseGptShrinkEndToEnd(const char *name, const QList<int> &exclude)
{
    printf("%s\n", name);
    const unsigned long long device = 40000, firstusable = 2048;
    // C sits between A and B on disk but is in the last slot.
    QList<GptPart> parts = {
        {0,  4096,  6143, "A"},
        {1, 20000, 20999, "B"},
        {2, 10000, 10499, "C"},
    };
    QByteArray dev = buildMultiGptDisk(device, firstusable, parts);
    // A bootloader below FirstUsableLBA, as genimage lays one out; more of it
    // above FirstUsableLBA but still before the first partition, as Armbian's
    // Rockchip images have it; and data between two partitions.
    fillSectors(dev, 64, 100, (char)0x1D);
    fillSectors(dev, 3000, 500, (char)0x1E);
    fillSectors(dev, 7000, 100, (char)0x1F);
    fillSectors(dev, 4096, 2048, (char)0xA1);
    fillSectors(dev, 20000, 1000, (char)0xB2);
    fillSectors(dev, 10000, 500, (char)0xC3);

    HANDLE h = writeTestFile(dev);
    if (h == INVALID_HANDLE_VALUE) return;
    PartitionShrinkPlan plan;
    QString detail;
    bool ok = planGptShrink(h, SEC, device, ALIGN_1MIB, &plan, &detail,
                            exclude.isEmpty() ? NULL : &exclude);
    CloseHandle(h);
    check(ok, "planned");
    if (!ok) { printf("  -> %s\n\n", detail.toLocal8Bit().constData()); return; }

    QByteArray img = applyPlan(dev, plan);
    if (img.isEmpty()) { printf("\n"); return; }
    check((unsigned long long)img.size() == plan.totalsectors * SEC,
          "the assembled image is totalsectors long");
    check(plan.totalsectors < device, "the image is smaller than the device");

    h = writeTestFile(img);
    if (h == INVALID_HANDLE_VALUE) return;
    check(gptPrimaryState(h, SEC, plan.totalsectors) == GPT_PRIMARY_OK,
          "the image's primary GPT is consistent");
    QString fixdetail;
    check(relocateBackupGPT(h, SEC, plan.totalsectors, &fixdetail) == GPT_FIX_NOT_NEEDED,
          "the backup GPT is already at the image's last LBA");
    const unsigned char *b = (const unsigned char *)img.constData() + (plan.totalsectors - 1) * SEC;
    check(memcmp(b, "EFI PART", 8) == 0 && headerCrcValid(b)
              && rd32(b, 88) == crc32of(b - ENTRYSECTORS * SEC, ENTRIES * ENTRYSIZE),
          "the backup header and its entry array are valid");
    QList<PartitionInfo> found;
    check(listGptPartitions(h, SEC, plan.totalsectors, &found, &detail), "the image's GPT lists");
    CloseHandle(h);

    check(img.mid(64 * SEC, 100 * SEC) == dev.mid(64 * SEC, 100 * SEC),
          "the bootloader below FirstUsableLBA is kept at the same sectors");
    check(img.mid(3000 * SEC, 500 * SEC) == dev.mid(3000 * SEC, 500 * SEC),
          "data above FirstUsableLBA but before the first partition is kept at the same sectors");
    check(noSectorFilledWith(img, (char)0x1F),
          "unpartitioned data between partitions is not kept");
    QList<KeptPart> want;
    if (!exclude.contains(0)) want.append({2048, (char)0xA1});
    if (!exclude.contains(2)) want.append({500, (char)0xC3});
    want.append({1000, (char)0xB2});
    checkImagePartitions(img, found, want);
    // A, at 4096, is the first partition; excluded, C takes its place.
    check(!found.isEmpty() && found[0].firstSector == 4096,
          "the first kept partition starts where the first partition did");
    for (int slot : exclude)
    {
        const char fill = (slot == 0) ? (char)0xA1 : (char)0xC3;
        check(noSectorFilledWith(img, fill), "no sector of an excluded partition was copied");
    }
    printf("\n");
}

// Read tries planGptShrink() and, if that declines, planMbrShrink(). On a GPT
// disk the second must decline too: its MBR is only a protective or hybrid
// copy, and repacking it would move partitions the GPT still describes where
// they were.
// Here the GPT plan declines honestly -- the partition already runs to the end
// of the card, as one grown on first boot does -- and the hybrid MBR entry
// that mirrors it must not be repacked in its place.
static void caseGptFallbackToHybridMbr()
{
    printf("a GPT with a hybrid MBR, when the GPT plan declines\n");
    const unsigned long long device = 40000, firstusable = 16384;
    const unsigned long long partfirst = 16384, partlast = device - 34;
    QList<GptPart> parts = { {0, partfirst, partlast, "rootfs"} };
    QByteArray dev = buildMultiGptDisk(device, firstusable, parts);
    unsigned char *d = (unsigned char *)dev.data();
    // The hybrid entry, beside the protective 0xEE one in slot 0.
    d[446 + 16 + 4] = 0x83;
    wr32(d, 446 + 16 + 8, (unsigned int)partfirst);
    wr32(d, 446 + 16 + 12, (unsigned int)(partlast - partfirst + 1));
    fillSectors(dev, 64, 100, (char)0x1D);     // bootloader, below FirstUsableLBA
    fillSectors(dev, partfirst, 100, (char)0xA1);

    HANDLE h = writeTestFile(dev);
    if (h == INVALID_HANDLE_VALUE) return;
    PartitionShrinkPlan plan;
    QString detail;
    const bool gpt = planGptShrink(h, SEC, device, ALIGN_1MIB, &plan, &detail);
    check(!gpt, "fixture: the GPT plan declines (nothing to gain)");
    const bool mbr = !gpt && planMbrShrink(h, SEC, device, ALIGN_1MIB, &plan, &detail);
    check(!mbr, "the hybrid MBR is not repacked in the GPT's place, so the Read is a full one");
    if (mbr)
    {
        QByteArray img = applyPlan(dev, plan);
        check(img.size() > (int)(165 * SEC) && img.mid(64 * SEC, 100 * SEC) == dev.mid(64 * SEC, 100 * SEC),
              "(had it been) the bootloader below FirstUsableLBA survives");
    }
    QList<PartitionInfo> found;
    check(!listMbrPartitions(h, SEC, device, &found, &detail),
          "Choose partitions does not offer the hybrid MBR's entries either");
    CloseHandle(h);
    printf("\n");
}

// A GPT whose protective MBR has lost its 0xEE entry is still a GPT: the
// header at LBA 1 is what says so.
static void caseMbrPlanOnGptWithoutProtectiveEntry()
{
    printf("an MBR with no 0xEE entry in front of a GPT\n");
    const unsigned long long device = 40000, firstusable = 2048;
    QList<GptPart> parts = { {0, 4096, 6143, "A"} };
    QByteArray dev = buildMultiGptDisk(device, firstusable, parts);
    unsigned char *d = (unsigned char *)dev.data();
    memset(d + 446, 0, 16);
    d[446 + 4] = 0x0C;
    wr32(d, 446 + 8, 4096);
    wr32(d, 446 + 12, 2048);
    HANDLE h = writeTestFile(dev);
    if (h == INVALID_HANDLE_VALUE) return;
    PartitionShrinkPlan plan;
    QString detail;
    check(!planMbrShrink(h, SEC, device, ALIGN_1MIB, &plan, &detail),
          "planMbrShrink declines a disk with a GPT header at LBA 1");
    QList<PartitionInfo> found;
    check(!listMbrPartitions(h, SEC, device, &found, &detail),
          "listMbrPartitions declines it too");
    CloseHandle(h);
    printf("\n");
}

// genimage sets FirstUsableLBA, and so the first partition, to wherever the
// bootloader ends, which is rarely on an alignment boundary. The first
// partition must stay exactly there -- rounding it up would move it, and
// rounding it down would put it on the bootloader -- and only the partitions
// after it are aligned.
static void caseGptShrinkUnalignedFirstUsable()
{
    printf("GPT shrink with the first partition off a 1MiB boundary\n");
    const unsigned long long device = 40000, firstusable = 3000;
    QList<GptPart> parts = { {0, 3000, 5047, "A"}, {1, 9000, 9999, "B"} };
    QByteArray dev = buildMultiGptDisk(device, firstusable, parts);
    fillSectors(dev, 64, firstusable - 64, (char)0x1D);   // the loader, up to FirstUsableLBA
    fillSectors(dev, 3000, 2048, (char)0xA1);
    fillSectors(dev, 9000, 1000, (char)0xB2);

    HANDLE h = writeTestFile(dev);
    if (h == INVALID_HANDLE_VALUE) return;
    PartitionShrinkPlan plan;
    QString detail;
    bool ok = planGptShrink(h, SEC, device, ALIGN_1MIB, &plan, &detail);
    CloseHandle(h);
    check(ok, "planned");
    if (!ok) { printf("  -> %s\n\n", detail.toLocal8Bit().constData()); return; }

    check(plan.ranges.size() == 3 && plan.ranges[1].srcfirst == 3000
              && plan.ranges[1].dstfirst == 3000,
          "the first partition stays at its unaligned start");
    check(plan.ranges.size() == 3 && plan.ranges[2].srcfirst == 9000
              && plan.ranges[2].dstfirst == 3 * ALIGN_1MIB,
          "the next moves to the first 1MiB boundary after it (6144)");
    QByteArray img = applyPlan(dev, plan);
    check(!img.isEmpty() && img.mid(64 * SEC, (int)((firstusable - 64) * SEC))
                                == dev.mid(64 * SEC, (int)((firstusable - 64) * SEC)),
          "the whole reserved area up to FirstUsableLBA is kept intact");
    printf("\n");
}

static void caseMbrShrinkEndToEnd(const char *name, const QList<int> &exclude)
{
    printf("%s\n", name);
    const unsigned long long device = 40000;
    QList<MbrPart> parts = {
        {0,  4096, 2048, 0x83},
        {1, 20000, 1000, 0x83},
        {2, 10000,  500, 0x0C},
    };
    QByteArray dev = buildMultiMbrDisk(device, parts);
    // A loader before the first partition, where Allwinner and Rockchip keep
    // theirs, and data between two partitions.
    fillSectors(dev, 16, 1000, (char)0x1D);
    fillSectors(dev, 7000, 100, (char)0x1F);
    fillSectors(dev, 4096, 2048, (char)0xA1);
    fillSectors(dev, 20000, 1000, (char)0xB2);
    fillSectors(dev, 10000, 500, (char)0xC3);

    HANDLE h = writeTestFile(dev);
    if (h == INVALID_HANDLE_VALUE) return;
    PartitionShrinkPlan plan;
    QString detail;
    bool ok = planMbrShrink(h, SEC, device, ALIGN_1MIB, &plan, &detail,
                            exclude.isEmpty() ? NULL : &exclude);
    CloseHandle(h);
    check(ok, "planned");
    if (!ok) { printf("  -> %s\n\n", detail.toLocal8Bit().constData()); return; }

    QByteArray img = applyPlan(dev, plan);
    if (img.isEmpty()) { printf("\n"); return; }
    check((unsigned long long)img.size() == plan.totalsectors * SEC,
          "the assembled image is totalsectors long");

    h = writeTestFile(img);
    if (h == INVALID_HANDLE_VALUE) return;
    QList<PartitionInfo> found;
    check(listMbrPartitions(h, SEC, plan.totalsectors, &found, &detail),
          "the image's MBR lists, every entry within the image");
    CloseHandle(h);

    check(img.mid(16 * SEC, 1000 * SEC) == dev.mid(16 * SEC, 1000 * SEC),
          "the loader before the first partition is kept at the same sectors");
    check(noSectorFilledWith(img, (char)0x1F),
          "unpartitioned data between partitions is not kept");
    check(!found.isEmpty() && found[0].firstSector == 4096,
          "the first partition does not move");
    QList<KeptPart> want = { {2048, (char)0xA1} };
    if (!exclude.contains(2)) want.append({500, (char)0xC3});
    want.append({1000, (char)0xB2});
    checkImagePartitions(img, found, want);
    const unsigned char *m = (const unsigned char *)img.constData();
    check(m[510] == 0x55 && m[511] == 0xAA, "the boot signature survived");
    if (exclude.contains(2))
    {
        check(noSectorFilledWith(img, (char)0xC3), "no sector of the excluded partition was copied");
    }
    printf("\n");
}

// ---------------------------------------------------------------------------
// The smaller helpers the write and verify paths depend on.
// ---------------------------------------------------------------------------

static void caseRewriteRisk()
{
    printf("gptRewriteRisk() / gptPrimaryState() classify the table\n");
    struct { unsigned long long firstusable; GptRewriteRisk expect; const char *what; } cases[] = {
        {  34, GPT_RISK_SAFE,     "FirstUsableLBA 34: the rewrite lands on the right value" },
        {2048, GPT_RISK_AFFECTED, "space reserved ahead of the first partition: affected" },
    };
    for (const auto &c : cases)
    {
        HANDLE h = writeTestFile(buildDisk(c.firstusable, 4096, 16384).bytes);
        if (h == INVALID_HANDLE_VALUE) return;
        check(gptRewriteRisk(h, SEC) == c.expect, c.what);
        CloseHandle(h);
    }
    Disk dk = buildDisk(34, 4096, 16384);
    damageSignature((unsigned char *)dk.bytes.data(), 16384);
    HANDLE h = writeTestFile(dk.bytes);
    if (h == INVALID_HANDLE_VALUE) return;
    check(gptRewriteRisk(h, SEC) == GPT_RISK_NO_GPT, "no GPT: nothing for Windows to rewrite");
    check(gptPrimaryState(h, SEC, 16384) == GPT_PRIMARY_NO_GPT, "no GPT: reported as such");
    CloseHandle(h);
    printf("\n");
}

static void caseHasMbrTable()
{
    printf("deviceHasMbrTable() tells an MBR image from none\n");
    const unsigned long long device = 2000;
    HANDLE h = writeTestFile(buildMultiMbrDisk(device, { {1, 100, 100, 0x0C} }));
    if (h == INVALID_HANDLE_VALUE) return;
    check(deviceHasMbrTable(h, SEC), "an MBR with a real partition");
    CloseHandle(h);

    h = writeTestFile(buildMultiMbrDisk(device, { {0, 1, device - 1, 0xEE} }));
    if (h == INVALID_HANDLE_VALUE) return;
    check(!deviceHasMbrTable(h, SEC), "a protective MBR alone is not an MBR image");
    CloseHandle(h);

    QByteArray nosig = buildMultiMbrDisk(device, { {1, 100, 100, 0x0C} });
    nosig[510] = 0;
    h = writeTestFile(nosig);
    if (h == INVALID_HANDLE_VALUE) return;
    check(!deviceHasMbrTable(h, SEC), "no boot signature, no MBR");
    CloseHandle(h);
    printf("\n");
}

static void caseWipe()
{
    printf("wipePartitionTables() zeroes both ends and nothing else\n");
    const unsigned long long device = 1000;
    QByteArray before(device * SEC, 0);
    for (unsigned long long s = 0; s < device; ++s) fillSectors(before, s, 1, (char)(1 + s % 250));
    HANDLE h = writeTestFile(before);
    if (h == INVALID_HANDLE_VALUE) return;
    check(wipePartitionTables(h, SEC, device), "reported success");
    QByteArray after(device * SEC, 0);
    SetFilePointer(h, 0, NULL, FILE_BEGIN);
    DWORD got = 0;
    ReadFile(h, after.data(), (DWORD)after.size(), &got, NULL);
    CloseHandle(h);
    const unsigned char *a = (const unsigned char *)after.constData();
    check(got == (DWORD)after.size() && rangeIsZero(a, 0, 33) && rangeIsZero(a, device - 34, device - 1),
          "the first and last 34 sectors are zero");
    check(unchangedOutside(before, after, { {0, 33}, {device - 34, device - 1} }),
          "every sector in between is untouched");
    printf("\n");
}

// Verify forgives exactly what the fix may rewrite: gptOwnedSectors() for the
// tables and gptImageBackupRange() for the stale copy. Anything else the fix
// changed would fail the verify of a good card.
static void caseOwnedSectors()
{
    printf("gptOwnedSectors() covers everything the fix writes\n");
    const unsigned long long device = 16384;
    Disk dk = buildDisk(2048, 4096, device);
    QByteArray after;
    QString detail;
    check(runRepair(dk, device, &after, &detail) == GPT_FIX_OK, "the fix ran");

    HANDLE h = writeTestFile(after);
    if (h == INVALID_HANDLE_VALUE) return;
    unsigned long long frontend = 0, tailstart = 0;
    bool ok = gptOwnedSectors(h, SEC, device, &frontend, &tailstart);
    CloseHandle(h);
    check(ok && frontend == 2 && tailstart == device - 1 - ENTRYSECTORS,
          "front is the MBR and primary header only; tail is the backup array + header");
    unsigned long long sfirst = 0, slast = 0;
    bool stale = gptImageBackupRange((const unsigned char *)dk.bytes.constData() + SEC, SEC,
                                     &sfirst, &slast);
    check(ok && stale && unchangedOutside(dk.bytes, after,
                                          { {0, frontend - 1}, {sfirst, slast}, {tailstart, device - 1} }),
          "every sector the fix changed is one verify forgives");
    printf("\n");
}

static void caseSectorIO()
{
    printf("readSectorDataFromHandle() / writeSectorDataToHandle() / getFileSizeInSectors()\n");
    QByteArray before(20 * SEC, 0);
    for (unsigned long long s = 0; s < 20; ++s) fillSectors(before, s, 1, (char)(0x10 + s));
    before.append("half");   // 20 sectors and 4 bytes
    HANDLE h = writeTestFile(before);
    if (h == INVALID_HANDLE_VALUE) return;

    check(getFileSizeInSectors(h, SEC) == 21, "a partial last sector counts as a sector");

    QByteArray chunk(3 * SEC, (char)0xEE);
    check(writeSectorDataToHandle(h, chunk.data(), 5, 3, SEC), "wrote 3 sectors at sector 5");
    char *back = readSectorDataFromHandle(h, 5, 3, SEC);
    check(back && memcmp(back, chunk.constData(), 3 * SEC) == 0, "read back what was written there");
    delete[] back;

    back = readSectorDataFromHandle(h, 4, 1, SEC);
    check(back && memcmp(back, before.constData() + 4 * SEC, SEC) == 0,
          "the sector before it is unchanged");
    delete[] back;
    back = readSectorDataFromHandle(h, 8, 1, SEC);
    check(back && memcmp(back, before.constData() + 8 * SEC, SEC) == 0,
          "the sector after it is unchanged");
    delete[] back;

    back = readSectorDataFromHandle(h, 20, 2, SEC);
    QByteArray tail(2 * SEC, 0);
    memcpy(tail.data(), "half", 4);
    check(back && memcmp(back, tail.constData(), 2 * SEC) == 0,
          "a read past the end of the file is zero-padded, not stale");
    delete[] back;
    CloseHandle(h);
    printf("\n");
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    // crc32of() mirrors disk.cpp's gptCrc32(), so the CRC checks below would
    // agree with a wrong polynomial or seed. Pin it to the standard check value.
    printf("CRC-32 algorithm\n");
    check(crc32of((const unsigned char *)"123456789", 9) == 0xCBF43926u,
          "matches the standard check value for \"123456789\"");
    printf("\n");

    // FirstUsableLBA 34, which the Windows rewrite happens to land on correctly.
    caseRelocate("ordinary layout", 34, 2048, 8192);
    // Space reserved ahead of the first partition, as ARM board images have it.
    caseRelocate("reserved-space layout", 2048, 16384, 65536);
    // Close but not overlapping (160 sectors apart); caseStaleOverlapsNewTable
    // covers the overlap guard.
    caseRelocate("image nearly fills device", 34, 8000, 8192);

    caseStaleUnderPartition();
    caseStaleOverlapsNewTable();

    caseUntouched("no GPT on the device", GPT_FIX_NO_GPT, damageSignature);
    caseUntouched("backup already at the last LBA", GPT_FIX_NOT_NEEDED, damageAlreadyAtEnd);
    caseUntouched("primary header CRC invalid", GPT_FIX_BAD_GPT, damageCrc);
    caseUntouched("entry array checksum invalid", GPT_FIX_BAD_GPT, damageEntriesCrc);
    caseUntouched("a partition runs past the new last usable LBA", GPT_FIX_FAILED,
                  damagePartitionPastEnd);

    // After Windows has already mangled it, with "Fix GPT after write" off.
    caseAfterTheFact("windows rewrote the table (reserved-space layout)", 2048, true);
    caseAfterTheFact("windows rewrote the table (ordinary layout)", 34, false);

    caseListGptOrder();
    caseGptShrinkExclude();
    caseListMbrOrder();
    caseMbrShrinkExclude();
    caseMbrProtectiveEntryDeclined();

    caseGptShrinkEndToEnd("GPT shrink applied end to end", {});
    caseGptShrinkEndToEnd("GPT shrink applied end to end, one partition excluded", {2});
    caseGptShrinkEndToEnd("GPT shrink applied end to end, first partition excluded", {0});
    caseGptShrinkUnalignedFirstUsable();
    caseGptFallbackToHybridMbr();
    caseMbrPlanOnGptWithoutProtectiveEntry();
    caseMbrShrinkEndToEnd("MBR shrink applied end to end", {});
    caseMbrShrinkEndToEnd("MBR shrink applied end to end, one partition excluded", {2});

    caseRewriteRisk();
    caseHasMbrTable();
    caseWipe();
    caseOwnedSectors();
    caseSectorIO();

    DeleteFileA(TESTFILE);
    printf("%d checks, %d failures\n", checks, failures);
    return failures ? 1 : 0;
}
