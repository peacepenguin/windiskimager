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

#include "bootcheck.h"

#include <algorithm>
#include <QCoreApplication>
#include <QObject>
#include <QSet>

// The largest file read: a BCD is tens of KB, rarely a MB.
static const unsigned long long MAX_FILE_BYTES = 128ull * 1024ull * 1024ull;  // a SYSTEM hive can be tens of MB

static unsigned int u16(const QByteArray &b, qint64 o)
{
    if (o < 0 || o + 2 > b.size()) return 0;
    const unsigned char *p = (const unsigned char *)b.constData() + o;
    return p[0] | (p[1] << 8);
}

static unsigned int u32(const QByteArray &b, qint64 o)
{
    if (o < 0 || o + 4 > b.size()) return 0;
    const unsigned char *p = (const unsigned char *)b.constData() + o;
    return (unsigned int)p[0] | ((unsigned int)p[1] << 8) | ((unsigned int)p[2] << 16)
           | ((unsigned int)p[3] << 24);
}

static unsigned long long u64(const QByteArray &b, qint64 o)
{
    return (unsigned long long)u32(b, o) | ((unsigned long long)u32(b, o + 4) << 32);
}

// The context the report's text is translated in.
class BootCheck
{
    Q_DECLARE_TR_FUNCTIONS(BootCheck)
};

// Bytes of one partition, by offset from its start, through the disk's
// sector reader.
class PartitionBytes
{
public:
    PartitionBytes(const BootSectorReader &read, unsigned long long sectorsize,
                   unsigned long long first, unsigned long long sectors)
        : myRead(read), mySectorSize(sectorsize), myFirst(first), mySectors(sectors) {}

    bool read(unsigned long long offset, unsigned long long length, QByteArray *out) const
    {
        const unsigned long long size = mySectors * mySectorSize;
        if (length == 0 || offset >= size || length > size - offset || length > MAX_FILE_BYTES * 4)
        {
            return false;
        }
        const unsigned long long s0 = offset / mySectorSize;
        const unsigned long long s1 = (offset + length + mySectorSize - 1) / mySectorSize;
        QByteArray buf((int)((s1 - s0) * mySectorSize), 0);
        if (!myRead(myFirst + s0, s1 - s0, buf.data()))
        {
            return false;
        }
        *out = buf.mid((int)(offset - s0 * mySectorSize), (int)length);
        return true;
    }

private:
    BootSectorReader myRead;
    unsigned long long mySectorSize, myFirst, mySectors;
};

// ---------------------------------------------------------------------- FAT ---

bool fatReadFile(const BootSectorReader &read, unsigned long long sectorsize,
                 unsigned long long first, unsigned long long sectors,
                 const QStringList &path, QByteArray *data, QString *why)
{
    const PartitionBytes part(read, sectorsize, first, sectors);
    QByteArray bs;
    if (!part.read(0, 512, &bs))
    {
        *why = BootCheck::tr("its first sector could not be read");
        return false;
    }
    const unsigned int bps = u16(bs, 11);
    const unsigned int spc = (unsigned char)bs[13];
    const unsigned int reserved = u16(bs, 14);
    const unsigned int fats = (unsigned char)bs[16];
    const unsigned int rootentries = u16(bs, 17);
    const unsigned long long total = u16(bs, 19) ? u16(bs, 19) : u32(bs, 32);
    const unsigned long long fatsize = u16(bs, 22) ? u16(bs, 22) : u32(bs, 36);
    if (u16(bs, 510) != 0xAA55 || (bps != 512 && bps != 1024 && bps != 2048 && bps != 4096)
        || spc == 0 || (spc & (spc - 1)) != 0 || reserved == 0 || fats == 0 || fatsize == 0)
    {
        *why = BootCheck::tr("it holds no FAT filesystem");
        return false;
    }
    const unsigned long long rootsectors = (rootentries * 32ull + bps - 1) / bps;
    const unsigned long long firstdata = reserved + fats * fatsize + rootsectors;
    if (total <= firstdata)
    {
        *why = BootCheck::tr("it holds no FAT filesystem");
        return false;
    }
    const unsigned long long clusters = (total - firstdata) / spc;
    const int kind = (clusters < 4085) ? 12 : (clusters < 65525) ? 16 : 32;
    const unsigned long long clusterbytes = (unsigned long long)spc * bps;

    QByteArray fat;
    if (fatsize * bps > 64ull * 1024 * 1024 || !part.read((unsigned long long)reserved * bps, fatsize * bps, &fat))
    {
        *why = BootCheck::tr("its allocation table could not be read");
        return false;
    }
    auto next = [&](unsigned long long c) -> unsigned long long {
        if (kind == 12)
        {
            const unsigned int v = u16(fat, (qint64)(c + c / 2));
            const unsigned int n = (c & 1) ? (v >> 4) : (v & 0xFFF);
            return (n >= 0xFF8) ? ~0ull : n;
        }
        if (kind == 16)
        {
            const unsigned int n = u16(fat, (qint64)(c * 2));
            return (n >= 0xFFF8) ? ~0ull : n;
        }
        const unsigned int n = u32(fat, (qint64)(c * 4)) & 0x0FFFFFFF;
        return (n >= 0x0FFFFFF8) ? ~0ull : n;
    };
    // A cluster chain's bytes, up to `limit`.
    auto chain = [&](unsigned long long c, unsigned long long limit, QByteArray *out) -> bool {
        out->clear();
        unsigned long long steps = 0;
        while (c != ~0ull && (unsigned long long)out->size() < limit)
        {
            if (c < 2 || c >= clusters + 2 || ++steps > clusters)
            {
                return false;
            }
            QByteArray piece;
            if (!part.read((firstdata + (c - 2) * spc) * bps, clusterbytes, &piece))
            {
                return false;
            }
            out->append(piece);
            c = next(c);
        }
        if ((unsigned long long)out->size() > limit)
        {
            out->truncate((int)limit);
        }
        return true;
    };

    // The root directory: a cluster chain on FAT32, a fixed area before that.
    QByteArray dir;
    if (kind == 32)
    {
        if (!chain(u32(bs, 44), MAX_FILE_BYTES, &dir))
        {
            *why = BootCheck::tr("its root directory could not be read");
            return false;
        }
    }
    else if (!part.read((reserved + fats * fatsize) * bps, rootentries * 32ull, &dir))
    {
        *why = BootCheck::tr("its root directory could not be read");
        return false;
    }

    for (int level = 0; level < path.size(); ++level)
    {
        const QString &want = path[level];
        bool found = false, isdir = false;
        unsigned long long cluster = 0, size = 0;
        QMap<int, QString> lfn;
        for (int o = 0; o + 32 <= dir.size() && !found; o += 32)
        {
            const unsigned char *e = (const unsigned char *)dir.constData() + o;
            if (e[0] == 0x00)
            {
                break;
            }
            if (e[0] == 0xE5)
            {
                lfn.clear();
                continue;
            }
            const unsigned char attr = e[11];
            if (attr == 0x0F)
            {
                // A long name's piece: 13 UTF-16 characters, numbered.
                static const int at[13] = { 1, 3, 5, 7, 9, 14, 16, 18, 20, 22, 24, 28, 30 };
                QString piece;
                for (int i = 0; i < 13; ++i)
                {
                    const unsigned int ch = e[at[i]] | (e[at[i] + 1] << 8);
                    if (ch == 0x0000 || ch == 0xFFFF) break;
                    piece.append(QChar((ushort)ch));
                }
                lfn.insert(e[0] & 0x3F, piece);
                continue;
            }
            if (attr & 0x08)
            {
                lfn.clear();
                continue;
            }
            QString base = QString::fromLatin1((const char *)e, 8).trimmed();
            if (!base.isEmpty() && base[0] == QChar(0x05)) base[0] = QChar(0xE5);
            const QString ext = QString::fromLatin1((const char *)e + 8, 3).trimmed();
            const QString shortname = ext.isEmpty() ? base : base + "." + ext;
            QString longname;
            for (auto it = lfn.begin(); it != lfn.end(); ++it)
            {
                longname += it.value();
            }
            lfn.clear();
            if (want.compare(shortname, Qt::CaseInsensitive) == 0
                || (!longname.isEmpty() && want.compare(longname, Qt::CaseInsensitive) == 0))
            {
                found = true;
                isdir = (attr & 0x10) != 0;
                cluster = ((kind == 32) ? ((unsigned long long)(e[20] | (e[21] << 8)) << 16) : 0)
                          | (unsigned long long)(e[26] | (e[27] << 8));
                size = (unsigned long long)e[28] | ((unsigned long long)e[29] << 8)
                       | ((unsigned long long)e[30] << 16) | ((unsigned long long)e[31] << 24);
            }
        }
        if (!found)
        {
            *why = BootCheck::tr("it has no \\%1").arg(path.mid(0, level + 1).join("\\"));
            return false;
        }
        const bool last = (level == path.size() - 1);
        if (last == isdir)
        {
            *why = BootCheck::tr("\\%1 is not a file").arg(path.join("\\"));
            return false;
        }
        if (last)
        {
            if (size > MAX_FILE_BYTES || (size && !chain(cluster, size, data)))
            {
                *why = BootCheck::tr("\\%1 could not be read").arg(path.join("\\"));
                return false;
            }
            if (size == 0) data->clear();
            return true;
        }
        if (!chain(cluster, MAX_FILE_BYTES, &dir))
        {
            *why = BootCheck::tr("\\%1 could not be read").arg(path.mid(0, level + 1).join("\\"));
            return false;
        }
    }
    *why = BootCheck::tr("no file was named");
    return false;
}

// --------------------------------------------------------------------- NTFS ---

namespace {

struct Run
{
    unsigned long long vcn, length;
    long long lcn;                  // -1: sparse
};

class Ntfs
{
public:
    Ntfs(const PartitionBytes &part) : myPart(part) {}

    bool open(QString *why)
    {
        QByteArray bs;
        if (!myPart.read(0, 512, &bs) || bs.mid(3, 8) != "NTFS    ")
        {
            *why = BootCheck::tr("it holds no NTFS filesystem");
            return false;
        }
        const unsigned int bps = u16(bs, 0x0B);
        const unsigned int spcraw = (unsigned char)bs[0x0D];
        const unsigned long long spc = (spcraw <= 0x80) ? spcraw : (1ull << (256 - spcraw));
        myCluster = bps * spc;
        auto size = [&](int raw) -> unsigned long long {
            const signed char v = (signed char)raw;
            return (v > 0) ? (unsigned long long)v * myCluster : (1ull << (-v));
        };
        myRecord = size((unsigned char)bs[0x40]);
        myIndex = size((unsigned char)bs[0x44]);
        if (bps < 512 || bps > 4096 || myCluster == 0 || myRecord < 512 || myRecord > 65536
            || myIndex < 512 || myIndex > 65536)
        {
            *why = BootCheck::tr("it holds no NTFS filesystem");
            return false;
        }
        // $MFT's own record holds where the MFT lies.
        QByteArray mft;
        if (!myPart.read(u64(bs, 0x30) * myCluster, myRecord, &mft) || !fixup(&mft, "FILE"))
        {
            *why = BootCheck::tr("its file table could not be read");
            return false;
        }
        const qint64 data = attribute(mft, 0x80, QString());
        if (data < 0 || !mft[(int)data + 8] || !runs(mft, data, &myMftRuns))
        {
            *why = BootCheck::tr("its file table could not be read");
            return false;
        }
        return true;
    }

    // File record n, fixed up; false if it is not a record in use.
    bool record(unsigned long long n, QByteArray *rec)
    {
        return stream(myMftRuns, n * myRecord, myRecord, rec) && fixup(rec, "FILE")
               && (u16(*rec, 0x16) & 1);
    }

    // The record of `name` in directory record `dir`, through its index.
    bool lookup(const QByteArray &dir, const QString &name, unsigned long long *found)
    {
        const qint64 root = attribute(dir, 0x90, "$I30");
        if (root < 0 || dir[(int)root + 8])
        {
            return false;
        }
        const qint64 value = root + u16(dir, root + 0x14);
        QList<unsigned long long> pending;
        if (scan(dir, value + 0x10, name, found, &pending))
        {
            return true;
        }
        const qint64 alloc = attribute(dir, 0xA0, "$I30");
        QList<Run> allocruns;
        if (pending.isEmpty() || alloc < 0 || !dir[(int)alloc + 8] || !runs(dir, alloc, &allocruns))
        {
            return false;
        }
        // Each node reached from the root, once; a block not in the tree may
        // hold stale entries, and is never looked at.
        const unsigned long long unit = (myIndex >= myCluster) ? myCluster : 512;
        QSet<unsigned long long> seen;
        while (!pending.isEmpty() && seen.size() < 10000)
        {
            const unsigned long long vcn = pending.takeFirst();
            if (seen.contains(vcn)) continue;
            seen.insert(vcn);
            QByteArray block;
            if (!stream(allocruns, vcn * unit, myIndex, &block) || !fixup(&block, "INDX"))
            {
                return false;
            }
            if (scan(block, 0x18, name, found, &pending))
            {
                return true;
            }
        }
        return false;
    }

    // The unnamed $DATA of a file record.
    bool content(const QByteArray &rec, QByteArray *out, QString *why)
    {
        const qint64 a = attribute(rec, 0x80, QString());
        if (a < 0)
        {
            *why = (attribute(rec, 0x20, QString()) >= 0)
                ? BootCheck::tr("it is stored in more pieces than this reads")
                : BootCheck::tr("it has no data");
            return false;
        }
        if (u16(rec, a + 0x0C) & 0x40FF)
        {
            *why = BootCheck::tr("it is compressed or encrypted");
            return false;
        }
        if (!rec[(int)a + 8])
        {
            *out = rec.mid((int)(a + u16(rec, a + 0x14)), (int)u32(rec, a + 0x10));
            return true;
        }
        const unsigned long long size = u64(rec, a + 0x30);
        QList<Run> r;
        if (size > MAX_FILE_BYTES || !runs(rec, a, &r) || (size && !stream(r, 0, size, out)))
        {
            *why = BootCheck::tr("it could not be read");
            return false;
        }
        if (size == 0) out->clear();
        return true;
    }

private:
    // Undoes the update sequence: the last two bytes of each 512 hold the
    // sequence number, the real bytes being kept in the array.
    static bool fixup(QByteArray *b, const char *sig)
    {
        if (b->left(4) != sig) return false;
        const unsigned int off = u16(*b, 4), count = u16(*b, 6);
        if (count == 0 || off + 2 * count > (unsigned int)b->size()) return false;
        const unsigned int usn = u16(*b, off);
        for (unsigned int i = 1; i < count; ++i)
        {
            const qint64 pos = (qint64)i * 512 - 2;
            if (pos + 2 > b->size()) break;
            if (u16(*b, pos) != usn) return false;
            (*b)[(int)pos] = (*b)[(int)(off + 2 * i)];
            (*b)[(int)pos + 1] = (*b)[(int)(off + 2 * i + 1)];
        }
        return true;
    }

    // The offset of the attribute of `type` named `name` in a record, or -1.
    static qint64 attribute(const QByteArray &rec, unsigned int type, const QString &name)
    {
        qint64 o = u16(rec, 0x14);
        while (o + 16 <= rec.size())
        {
            const unsigned int t = u32(rec, o), len = u32(rec, o + 4);
            if (t == 0xFFFFFFFF || len < 16 || o + len > (unsigned long long)rec.size()) break;
            if (t == type)
            {
                const int nlen = (unsigned char)rec[(int)o + 9];
                const QString n = QString((const QChar *)(rec.constData() + o + u16(rec, o + 0x0A)), nlen);
                if (n == name) return o;
            }
            o += len;
        }
        return -1;
    }

    // The run list of a non-resident attribute.
    static bool runs(const QByteArray &rec, qint64 a, QList<Run> *out)
    {
        out->clear();
        const qint64 end = a + u32(rec, a + 4);
        qint64 p = a + u16(rec, a + 0x20);
        unsigned long long vcn = u64(rec, a + 0x10);
        long long lcn = 0;
        while (p < end && rec[(int)p] != 0)
        {
            const unsigned char h = (unsigned char)rec[(int)p];
            const int ln = h & 0x0F, lo = h >> 4;
            if (ln == 0 || ln > 8 || lo > 8 || p + 1 + ln + lo > end) return false;
            unsigned long long length = 0;
            for (int i = 0; i < ln; ++i) length |= (unsigned long long)(unsigned char)rec[(int)(p + 1 + i)] << (8 * i);
            long long delta = 0;
            for (int i = 0; i < lo; ++i) delta |= (long long)(unsigned char)rec[(int)(p + 1 + ln + i)] << (8 * i);
            if (lo && lo < 8 && (rec[(int)(p + ln + lo)] & 0x80)) delta -= (1ll << (8 * lo));
            if (lo) lcn += delta;
            out->append(Run{vcn, length, lo ? lcn : -1});
            vcn += length;
            p += 1 + ln + lo;
        }
        return true;
    }

    // Bytes [offset, offset + length) of a non-resident stream.
    bool stream(const QList<Run> &r, unsigned long long offset, unsigned long long length,
                QByteArray *out)
    {
        out->clear();
        if (length > MAX_FILE_BYTES * 4) return false;
        while (length)
        {
            const unsigned long long vcn = offset / myCluster;
            const Run *run = NULL;
            for (const Run &x : r)
            {
                if (vcn >= x.vcn && vcn < x.vcn + x.length) run = &x;
            }
            if (!run) return false;
            const unsigned long long within = offset - run->vcn * myCluster;
            const unsigned long long take = qMin(length, run->length * myCluster - within);
            QByteArray piece;
            if (run->lcn < 0)
            {
                piece = QByteArray((int)take, 0);
            }
            else if (!myPart.read((unsigned long long)run->lcn * myCluster + within, take, &piece))
            {
                return false;
            }
            out->append(piece);
            offset += take;
            length -= take;
        }
        return true;
    }

    // The entries of the index node whose header is at `node`: true when one
    // is `name`; the nodes below it are added to *pending.
    static bool scan(const QByteArray &b, qint64 node, const QString &name,
                     unsigned long long *found, QList<unsigned long long> *pending)
    {
        qint64 p = node + u32(b, node);
        const qint64 end = node + u32(b, node + 4);
        for (int guard = 0; p + 16 <= end && p + 16 <= b.size() && guard < 4096; ++guard)
        {
            const unsigned int len = u16(b, p + 8), keylen = u16(b, p + 10), flags = u16(b, p + 12);
            if (len < 16) return false;
            if ((flags & 1) && len >= 24)
            {
                pending->append(u64(b, p + len - 8));
            }
            if (flags & 2) break;
            if (keylen >= 0x42)
            {
                const qint64 key = p + 16;
                const int nlen = (unsigned char)b[(int)(key + 0x40)];
                const int space = (unsigned char)b[(int)(key + 0x41)];
                if (key + 0x42 + 2 * nlen <= b.size() && space != 2)
                {
                    const QString n = QString((const QChar *)(b.constData() + key + 0x42), nlen);
                    if (n.compare(name, Qt::CaseInsensitive) == 0)
                    {
                        *found = u64(b, p) & 0xFFFFFFFFFFFFull;
                        return true;
                    }
                }
            }
            p += len;
        }
        return false;
    }

    const PartitionBytes &myPart;
    unsigned long long myCluster = 0, myRecord = 0, myIndex = 0;
    QList<Run> myMftRuns;
};

} // namespace

bool ntfsReadFile(const BootSectorReader &read, unsigned long long sectorsize,
                  unsigned long long first, unsigned long long sectors,
                  const QStringList &path, QByteArray *data, QString *why)
{
    const PartitionBytes part(read, sectorsize, first, sectors);
    Ntfs fs(part);
    if (!fs.open(why))
    {
        return false;
    }
    QByteArray rec;
    if (!fs.record(5, &rec))        // the root directory
    {
        *why = BootCheck::tr("its root directory could not be read");
        return false;
    }
    for (int level = 0; level < path.size(); ++level)
    {
        unsigned long long n = 0;
        if (!fs.lookup(rec, path[level], &n) || !fs.record(n, &rec))
        {
            *why = BootCheck::tr("it has no \\%1").arg(path.mid(0, level + 1).join("\\"));
            return false;
        }
    }
    QString detail;
    if (!fs.content(rec, data, &detail))
    {
        *why = BootCheck::tr("\\%1: %2").arg(path.join("\\"), detail);
        return false;
    }
    return true;
}

// ---------------------------------------------------------------- the hive ---

namespace {

class Hive
{
public:
    explicit Hive(const QByteArray &d) : myData(d) {}

    // A cell's data, after its size; empty if it is out of range.
    QByteArray cell(unsigned int offset) const
    {
        const qint64 at = 0x1000 + (qint64)offset;
        if (offset == 0xFFFFFFFF || at + 4 > myData.size()) return QByteArray();
        const int size = (int)u32(myData, at);
        const qint64 len = (size < 0) ? -(qint64)size : size;
        if (len < 8 || at + len > myData.size()) return QByteArray();
        return myData.mid((int)(at + 4), (int)(len - 4));
    }

    QString keyName(const QByteArray &nk) const
    {
        const int len = (int)u16(nk, 0x48);
        if (u16(nk, 2) & 0x20)
        {
            return QString::fromLatin1(nk.constData() + 0x4C, qMin(len, nk.size() - 0x4C));
        }
        return QString((const QChar *)(nk.constData() + 0x4C), qMin(len, nk.size() - 0x4C) / 2);
    }

    // Subkeys of key `nk`, by name.
    QMap<QString, QByteArray> subkeys(const QByteArray &nk) const
    {
        QMap<QString, QByteArray> out;
        list(u32(nk, 0x1C), &out, 0);
        return out;
    }

    // A value of key `nk`, by name: its type and data.
    bool value(const QByteArray &nk, const QString &name, unsigned int *type, QByteArray *data) const
    {
        const unsigned int count = u32(nk, 0x24);
        const QByteArray offsets = cell(u32(nk, 0x28));
        for (unsigned int i = 0; i < count && (qint64)(i * 4 + 4) <= offsets.size(); ++i)
        {
            const QByteArray vk = cell(u32(offsets, i * 4));
            if (vk.left(2) != "vk") continue;
            const int nlen = (int)u16(vk, 2);
            const QString n = (u16(vk, 0x10) & 1)
                ? QString::fromLatin1(vk.constData() + 0x14, qMin(nlen, vk.size() - 0x14))
                : QString((const QChar *)(vk.constData() + 0x14), qMin(nlen, vk.size() - 0x14) / 2);
            if (n.compare(name, Qt::CaseInsensitive) != 0) continue;
            *type = u32(vk, 0x0C);
            const unsigned int size = u32(vk, 4);
            if (size & 0x80000000u)
            {
                *data = vk.mid(8, (int)qMin(size & 0x7FFFFFFFu, 4u));
                return true;
            }
            const QByteArray d = cell(u32(vk, 8));
            if (size > 16344 && d.left(2) == "db")
            {
                // Big data, in segments.
                const unsigned int segs = u16(d, 2);
                const QByteArray list = cell(u32(d, 4));
                data->clear();
                for (unsigned int s = 0; s < segs && (unsigned int)data->size() < size; ++s)
                {
                    data->append(cell(u32(list, s * 4)).left(16344));
                }
                data->truncate((int)size);
                return true;
            }
            *data = d.left((int)size);
            return true;
        }
        return false;
    }

    QByteArray root() const { return cell(u32(myData, 0x24)); }

private:
    void list(unsigned int offset, QMap<QString, QByteArray> *out, int depth) const
    {
        const QByteArray l = cell(offset);
        const QByteArray sig = l.left(2);
        const unsigned int count = u16(l, 2);
        if (depth > 4) return;
        for (unsigned int i = 0; i < count; ++i)
        {
            if (sig == "lf" || sig == "lh")
            {
                const QByteArray nk = cell(u32(l, 4 + i * 8));
                if (nk.left(2) == "nk") out->insert(keyName(nk).toLower(), nk);
            }
            else if (sig == "li")
            {
                const QByteArray nk = cell(u32(l, 4 + i * 4));
                if (nk.left(2) == "nk") out->insert(keyName(nk).toLower(), nk);
            }
            else if (sig == "ri")
            {
                list(u32(l, 4 + i * 4), out, depth + 1);
            }
        }
    }

    const QByteArray &myData;
};

QString guidText(const QByteArray &g)
{
    return "{" + gptGuidText(g).toLower() + "}";
}

// Partition descriptors in a device element: the element's own, at 0x10,
// after the GUID of its options object, and any inside it, as a ramdisk
// holds the one its file is on. A descriptor: device type 6 (a partition),
// flags, size, reserved; the partition (GPT: its unique GUID; MBR: its offset
// in bytes); the disk's kind (0, local), the partition style (0 GPT, 1
// MBR), and the disk ID (GPT: its GUID; MBR: its signature).
void deviceRefs(const QByteArray &blob, BcdDeviceRef base, QList<BcdDeviceRef> *out)
{
    for (qint64 o = 0x10; o + 0x2C <= blob.size(); o += 4)
    {
        if (u32(blob, o) != 6 || u32(blob, o + 0x20) != 0 || u32(blob, o + 0x24) > 1)
        {
            continue;
        }
        BcdDeviceRef r = base;
        r.gpt = (u32(blob, o + 0x24) == 0);
        if (r.gpt)
        {
            if (o + 0x38 > blob.size()) continue;
            r.partition = blob.mid((int)(o + 0x10), 16);
            r.disk = blob.mid((int)(o + 0x28), 16);
        }
        else
        {
            if (u64(blob, o + 0x18) != 0) continue;
            r.partition = blob.mid((int)(o + 0x10), 8);
            r.disk = blob.mid((int)(o + 0x28), 4);
        }
        r.nested = (o != 0x10);
        if (r.nested)
        {
            // A ramdisk: the file it loads, the longest path in the element,
            // and its options object, the GUID it starts with.
            QString best;
            for (qint64 s = 0; s + 4 <= blob.size(); ++s)
            {
                if (u16(blob, s) != '\\') continue;
                QString text;
                for (qint64 c = s; c + 2 <= blob.size(); c += 2)
                {
                    const unsigned int ch = u16(blob, c);
                    if (ch == 0) break;
                    if (ch < 0x20 || ch > 0xFFFD) { text.clear(); break; }
                    text.append(QChar((ushort)ch));
                }
                if (text.size() > best.size()) best = text;
            }
            r.path = best;
            if (blob.left(16) != QByteArray(16, 0)) r.options = guidText(blob.left(16));
        }
        out->append(r);
        o += 0x24;
    }
}

} // namespace

bool parseBcdHive(const QByteArray &hive, QList<BcdDeviceRef> *refs, int *objects, QString *why)
{
    refs->clear();
    *objects = 0;
    if (hive.size() < 0x1000 + 32 || hive.left(4) != "regf")
    {
        *why = BootCheck::tr("it is not a registry hive");
        return false;
    }
    const Hive h(hive);
    const QByteArray root = h.root();
    if (root.left(2) != "nk")
    {
        *why = BootCheck::tr("its root key could not be read");
        return false;
    }
    const QMap<QString, QByteArray> top = h.subkeys(root);
    if (!top.contains("objects"))
    {
        *why = BootCheck::tr("it holds no boot objects");
        return false;
    }
    const QMap<QString, QByteArray> objs = h.subkeys(top["objects"]);
    *objects = objs.size();
    for (auto it = objs.begin(); it != objs.end(); ++it)
    {
        const QMap<QString, QByteArray> parts = h.subkeys(it.value());
        BcdDeviceRef base;
        base.object = h.keyName(it.value()).toLower();
        base.objectType = 0;
        base.element = 0;
        base.gpt = false;
        base.nested = false;
        unsigned int type = 0;
        QByteArray data;
        if (parts.contains("description") && h.value(parts["description"], "Type", &type, &data))
        {
            base.objectType = u32(data, 0);
        }
        if (!parts.contains("elements")) continue;
        const QMap<QString, QByteArray> elements = h.subkeys(parts["elements"]);
        if (elements.contains("12000004") && h.value(elements["12000004"], "Element", &type, &data))
        {
            base.description = QString((const QChar *)data.constData(), data.size() / 2);
            base.description.truncate(base.description.indexOf(QChar(0)) < 0
                                          ? base.description.size()
                                          : base.description.indexOf(QChar(0)));
        }
        for (auto e = elements.begin(); e != elements.end(); ++e)
        {
            bool ok = false;
            const unsigned int id = e.key().toUInt(&ok, 16);
            // Only device-format elements name partitions.
            if (!ok || (id & 0x0F000000) != 0x01000000) continue;
            if (!h.value(e.value(), "Element", &type, &data)) continue;
            BcdDeviceRef r = base;
            r.element = id;
            deviceRefs(data, r, refs);
        }
    }
    return true;
}

// ------------------------------------------------------------------- check ---

namespace {

const char *ESP_GUID = "C12A7328-F81F-11D2-BA4B-00A0C93EC93B";

QString sizeText(unsigned long long bytes)
{
    if (bytes >= 1000000000ull) return BootCheck::tr("%1 GB").arg(bytes / 1e9, 0, 'f', 1);
    return BootCheck::tr("%1 MB").arg(bytes / 1e6, 0, 'f', 0);
}

// The bcdedit name of a device element, for the object kind it is in.
QString elementName(unsigned int objecttype, unsigned int element)
{
    switch (element)
    {
    case 0x11000001: return "device";
    case 0x21000001: return (objecttype == 0x10200004) ? "filedevice" : "osdevice";
    case 0x31000003: return "ramdisksdidevice";
    }
    return QString();
}

QString objectText(const BcdDeviceRef &r)
{
    static const QMap<QString, QString> known = {
        { "{9dea862c-5cdd-4e70-acc1-f32b344d4795}", "Windows Boot Manager" },
        { "{a5a30fa2-3d06-4e9f-b5f4-a01df9d1fcba}", "Firmware Boot Manager" },
        { "{b2721d73-1db4-4c62-bf78-c548a880142d}", "Windows Memory Diagnostic" },
        { "{7619dcc8-fafe-11d9-b411-000476eba25f}", "Ramdisk Options" },
    };
    QString name = r.description;
    if (name.isEmpty()) name = known.value(r.object);
    return name.isEmpty() ? r.object : QString("%1 %2").arg(name, r.object);
}

QString fsName(const BootSectorReader &read, unsigned long long ss, const CombinePartition &p)
{
    QByteArray b((int)ss, 0);
    if (!read(p.first, 1, b.data())) return QString();
    if (b.mid(3, 8) == "NTFS    ") return "NTFS";
    if (b.mid(82, 5) == "FAT32") return "FAT32";
    if (b.mid(54, 5) == "FAT16") return "FAT16";
    if (b.mid(54, 5) == "FAT12") return "FAT12";
    return QString();
}

} // namespace

BootCheckResult checkBoot(const ImageLayout &layout, const BootSectorReader &read,
                          const BootCheckDisk &disk)
{
    BootCheckResult out;
    const unsigned long long ss = disk.sectorsize;
    QString &rep = out.report;
    const bool gpt = (layout.table == COMBINE_TABLE_GPT);
    if (layout.table == COMBINE_TABLE_NONE)
    {
        rep = BootCheck::tr("The disk has no partition table, so nothing on it can be booted the way "
                 "Windows boots.");
        return out;
    }
    const QByteArray diskid = gpt ? layout.diskGuid : layout.tableRegion.mid(440, 4);
    auto idText = [&](const QByteArray &id) {
        return (id.size() == 16) ? guidText(id)
                                 : QString("%1").arg(u32(id, 0), 8, 16, QChar('0')).toUpper();
    };
    // diskpart's number for each partition; the table's order without it.
    auto number = [&](int i) {
        return disk.partitionNumbers.value(layout.partitions[i].first, i + 1);
    };
    auto partText = [&](int i) {
        const CombinePartition &p = layout.partitions[i];
        const QString fs = fsName(read, ss, p);
        return fs.isEmpty() ? BootCheck::tr("partition %1 (%2)").arg(number(i)).arg(sizeText(p.sectors * ss))
                            : BootCheck::tr("partition %1 (%2, %3)").arg(number(i)).arg(fs, sizeText(p.sectors * ss));
    };

    rep += (gpt ? BootCheck::tr("Disk %1: GPT, disk GUID %2.") : BootCheck::tr("Disk %1: MBR, disk signature %2."))
               .arg(disk.number).arg(idText(diskid)) + "\n";

    // Where the BCD is: the EFI system partition's, or a BIOS system
    // partition's -- the active one first.
    QList<int> order;
    for (int i = 0; i < layout.partitions.size(); ++i)
    {
        const CombinePartition &p = layout.partitions[i];
        const bool esp = gpt ? (p.entry.size() >= 16 && gptGuidText(p.entry.left(16)) == ESP_GUID)
                             : (p.mbrType == 0xEF);
        const bool active = !gpt && p.entry.size() >= 1 && (unsigned char)p.entry[0] == 0x80;
        if (esp || active) order.prepend(i); else order.append(i);
    }
    int bcdpart = -1;
    bool efi = false;
    QByteArray hive;
    QStringList tried;
    for (int i : order)
    {
        const CombinePartition &p = layout.partitions[i];
        const QString fs = fsName(read, ss, p);
        if (fs.isEmpty()) continue;
        for (int kind = 0; kind < 2 && bcdpart < 0; ++kind)
        {
            const QStringList path = (kind == 0) ? QStringList{ "EFI", "Microsoft", "Boot", "BCD" }
                                                 : QStringList{ "Boot", "BCD" };
            QString why;
            const bool ok = (fs == "NTFS")
                ? ntfsReadFile(read, ss, p.first, p.sectors, path, &hive, &why)
                : fatReadFile(read, ss, p.first, p.sectors, path, &hive, &why);
            if (ok)
            {
                bcdpart = i;
                efi = (kind == 0);
            }
        }
        if (bcdpart >= 0) break;
        tried.append(partText(i));
    }
    // Windows' own record of drive letters, MountedDevices in its SYSTEM
    // hive, names partitions by partition GUID (GPT) or by signature and
    // offset (MBR). A copy whose partitions got new IDs finds no C: there,
    // its Windows partition gets another letter, and sign-in stops at a
    // black screen.
    auto checkLetters = [&](int windows) {
        bool lettersStale = false;
        const CombinePartition &wp = layout.partitions[windows];
        QByteArray system;
        QString why;
        unsigned int type = 0;
        QByteArray c;
        if (fsName(read, ss, wp) != "NTFS"
            || !ntfsReadFile(read, ss, wp.first, wp.sectors, { "Windows", "System32", "config", "SYSTEM" },
                             &system, &why)
            || system.size() < 0x1000 + 32 || system.left(4) != "regf")
        {
            rep += BootCheck::tr("Windows' drive letters could not be checked: %1 has no "
                      "\\Windows\\System32\\config\\SYSTEM that could be read.").arg(partText(windows)) + "\n";
        }
        else
        {
            const Hive h(system);
            const QByteArray root = h.root();
            const QMap<QString, QByteArray> top = (root.left(2) == "nk") ? h.subkeys(root)
                                                                         : QMap<QString, QByteArray>();
            if (!top.contains("mounteddevices")
                || !h.value(top["mounteddevices"], "\\DosDevices\\C:", &type, &c))
            {
                // Windows does not then take C: for itself: it takes the next
                // free letter, D: say, and sign-in fails as for a wrong one.
                lettersStale = true;
                rep += BootCheck::tr("Windows' drive letters: no C: is recorded, so Windows on %1 starts "
                          "under another letter, D: say, and sign-in stops at a black screen. The "
                          "commands below record C: for it.").arg(partText(windows)) + "\n";
            }
            else
            {
                bool mine = false, known = false;
                if (gpt && c.size() == 24 && c.left(8) == "DMIO:ID:")
                {
                    known = true;
                    mine = (c.mid(8, 16) == wp.entry.mid(16, 16));
                }
                else if (!gpt && c.size() == 12)
                {
                    known = true;
                    mine = (c.left(4) == diskid && u64(c, 4) == wp.first * ss);
                }
                if (!known)
                {
                    rep += BootCheck::tr("Windows' drive letters: C: is recorded in a form not checked here.") + "\n";
                }
                else if (mine)
                {
                    rep += BootCheck::tr("Windows' drive letters: C: is %1, as it should be.").arg(partText(windows))
                           + "\n";
                }
                else
                {
                    lettersStale = true;
                    rep += BootCheck::tr("Windows' drive letters: C: is recorded as %1, which this disk does not "
                              "have, so Windows on %2 starts under another letter and sign-in stops at "
                              "a black screen. The commands below record C: for its partition.")
                               .arg(gpt ? BootCheck::tr("partition %1").arg(guidText(c.mid(8, 16)))
                                        : BootCheck::tr("the partition at byte %1 on disk %2")
                                              .arg(u64(c, 4)).arg(idText(c.left(4))),
                                    partText(windows)) + "\n";
                }
            }
        }
        rep += "\n";
        return lettersStale;
    };

    // The commands that record C: for partition `windows`, at letter W here.
    auto regFix = [&](int windows, QChar W) {
        // C: as Windows records it: "DMIO:ID:" and the partition's GUID
        // (GPT), or the disk signature and the partition's offset (MBR).
        const CombinePartition &wp = layout.partitions[windows];
        QByteArray c;
        if (gpt)
        {
            c = QByteArray("DMIO:ID:") + wp.entry.mid(16, 16);
        }
        else
        {
            c = diskid;
            for (int b = 0; b < 8; ++b) c.append((char)(((wp.first * ss) >> (8 * b)) & 0xFF));
        }
        return QString("reg load HKLM\\CLONE %1:\\Windows\\System32\\config\\SYSTEM\n"
                       "reg delete HKLM\\CLONE\\MountedDevices /va /f\n"
                       "reg add HKLM\\CLONE\\MountedDevices /v \\DosDevices\\C: /t REG_BINARY /d %2 /f\n"
                       "reg unload HKLM\\CLONE\n").arg(W).arg(QString(c.toHex().toUpper()));
    };

    // The EFI system and recovery partitions, by type.
    static const QByteArray recoveryType = QByteArray::fromHex("a4bb94ded106404da16abfd50179d6ac");
    auto findType = [&](bool esp) {
        for (int i = 0; i < layout.partitions.size(); ++i)
        {
            const CombinePartition &p = layout.partitions[i];
            const bool match = esp ? (gpt ? (p.entry.size() >= 16 && gptGuidText(p.entry.left(16)) == ESP_GUID)
                                          : p.mbrType == 0xEF)
                                   : (gpt ? p.entry.left(16) == recoveryType
                                          : p.entry.size() >= 16 && (unsigned char)p.entry[4] == 0x27);
            if (match) return i;
        }
        return -1;
    };

    if (bcdpart < 0)
    {
        rep += BootCheck::tr("No Windows boot configuration was found: no partition holds "
                  "\\EFI\\Microsoft\\Boot\\BCD or \\Boot\\BCD.");
        if (!tried.isEmpty()) rep += "\n" + BootCheck::tr("Looked in: %1.").arg(tried.join(", "));
        rep += "\n";

        // A Windows with nothing to start it -- a VM's disk whose boot
        // partitions were left behind, or a Windows on what was a second,
        // data disk: an NTFS partition with \Windows\System32\winload.efi.
        int windows = -1;
        for (int i = 0; i < layout.partitions.size() && windows < 0; ++i)
        {
            const CombinePartition &p = layout.partitions[i];
            QByteArray loader;
            QString why;
            if (fsName(read, ss, p) == "NTFS"
                && ntfsReadFile(read, ss, p.first, p.sectors, { "Windows", "System32", "winload.efi" }, &loader, &why))
            {
                windows = i;
            }
        }
        if (windows < 0) return out;
        if (!gpt)
        {
            rep += "\n" + BootCheck::tr("Windows is on %1, but the disk is MBR: a UEFI VM (Hyper-V's generation 2) "
                      "starts only a GPT disk. Convert it with \"mbr2gpt /convert /disk:%2 /allowFullOS\" "
                      "once it has a boot configuration, or start it in a generation 1 VM after "
                      "\"bcdboot X:\\Windows /s X: /f BIOS\" and marking its partition active.")
                       .arg(partText(windows)).arg(disk.number) + "\n";
            return out;
        }

        const int esp = findType(true);
        const int recovery = findType(false);
        rep += "\n" + BootCheck::tr("Windows is on %1, but nothing on the disk starts it: %2 The disk "
                  "will not boot until it has one.")
                   .arg(partText(windows),
                        esp < 0 ? BootCheck::tr("there is no EFI system partition for the boot manager and "
                                                "its boot configuration.")
                                : BootCheck::tr("%1, the EFI system partition, holds no boot configuration.")
                                      .arg(partText(esp))) + "\n\n";
        const bool lettersStale = checkLetters(windows);

        // The layout Windows setup makes: an EFI system partition, a
        // Microsoft reserved one after it, and a recovery one. Sizes in MB as
        // diskpart counts them (MiB), as setup leaves them on a VM's disk.
        const unsigned long long ESP_MB = 200, MSR_MB = 16, RECOVERY_MB = 892;
        // The least each may have when it goes in space already free: a
        // gap a little short of the full size -- the backup GPT's sectors,
        // or what the partitions removed left -- is used as it is.
        const unsigned long long ESP_MIN_MB = 100, RECOVERY_MIN_MB = 800;
        static const QByteArray msrType = QByteArray::fromHex("16e3c9e35c0bb84d817df92df00215ae");
        bool hasMsr = false;
        for (const CombinePartition &p : layout.partitions) hasMsr = hasMsr || p.entry.left(16) == msrType;

        // The disk's free space: gaps between the first and last usable
        // sectors, each from a 1 MiB boundary, as diskpart puts partitions.
        struct Gap { unsigned long long start, mb; };
        QList<Gap> gaps;
        const unsigned long long mib = 1048576 / ss;
        unsigned long long end = 0;
        for (const CombinePartition &p : layout.partitions) end = qMax(end, p.first + p.sectors);
        const unsigned long long usableEnd = (disk.sectors ? disk.sectors : end) - 33;
        QList<int> byStart;
        for (int i = 0; i < layout.partitions.size(); ++i) byStart.append(i);
        std::sort(byStart.begin(), byStart.end(),
                  [&](int a, int b) { return layout.partitions[a].first < layout.partitions[b].first; });
        unsigned long long at = layout.firstUsable ? layout.firstUsable : 34;
        auto addGap = [&](unsigned long long from, unsigned long long to) {
            from = (from + mib - 1) / mib * mib;
            if (to > from && (to - from) / mib > 0) gaps.append({ from, (to - from) / mib });
        };
        for (int i : byStart)
        {
            addGap(at, layout.partitions[i].first);
            at = qMax(at, layout.partitions[i].first + layout.partitions[i].sectors);
        }
        addGap(at, usableEnd);

        // Each partition to make: in the smallest gap it fits, else in
        // space shrunk from Windows' partition. offset 0: no gap.
        struct Make { unsigned long long mb, offsetKb; };
        auto place = [&](unsigned long long want, unsigned long long least) {
            int best = -1;
            for (int g = 0; g < gaps.size(); ++g)
            {
                if (gaps[g].mb >= least && (best < 0 || gaps[g].mb < gaps[best].mb)) best = g;
            }
            if (best < 0) return Make{ want, 0 };
            const Make m{ qMin(want, gaps[best].mb), gaps[best].start * ss / 1024 };
            gaps[best].start += m.mb * mib;
            gaps[best].mb -= m.mb;
            return m;
        };
        Make espMake{ 0, 0 }, msrMake{ 0, 0 }, recMake{ 0, 0 };
        if (esp < 0)
        {
            espMake = place(ESP_MB, ESP_MIN_MB);
            // The reserved partition, right after it, if the gap has room.
            if (!hasMsr && espMake.offsetKb)
            {
                for (Gap &g : gaps)
                {
                    if (g.start * ss / 1024 == espMake.offsetKb + espMake.mb * 1024 && g.mb >= MSR_MB)
                    {
                        msrMake = Make{ MSR_MB, g.start * ss / 1024 };
                        g.start += MSR_MB * mib;
                        g.mb -= MSR_MB;
                    }
                }
            }
        }
        if (recovery < 0) recMake = place(RECOVERY_MB, RECOVERY_MIN_MB);
        const unsigned long long shrinkMb = (esp < 0 && !espMake.offsetKb ? espMake.mb : 0)
                                          + (recovery < 0 && !recMake.offsetKb ? recMake.mb : 0);

        // Letters: the ones partitions have here, else W:, S:, R: if free.
        QString freeLetters = disk.freeLetters.isEmpty() ? QString("RSTUVWY") : disk.freeLetters;
        auto pick = [&](QChar want) {
            const QChar got = freeLetters.contains(want) ? want
                            : freeLetters.isEmpty()      ? QChar('?')
                                                         : freeLetters[0];
            freeLetters.remove(got);
            return got;
        };
        const QChar haveW = disk.partitionLetters.value(layout.partitions[windows].first);
        const QChar W = haveW.isNull() ? pick('W') : haveW;
        const QChar haveS = esp < 0 ? QChar() : disk.partitionLetters.value(layout.partitions[esp].first);
        const QChar S = haveS.isNull() ? pick('S') : haveS;
        const QChar R = pick('R');

        rep += BootCheck::tr("To make it bootable, run the commands below in an administrator command "
                  "prompt on this computer, with the disk attached as it is now (Disk %1), and detach "
                  "it before starting the VM.").arg(disk.number) + " ";
        if (esp < 0 || recovery < 0)
        {
            QStringList parts;
            if (esp < 0) parts.append(BootCheck::tr("an EFI system partition of %1 MB").arg(espMake.mb));
            if (msrMake.mb) parts.append(BootCheck::tr("a Microsoft reserved partition of %1 MB").arg(MSR_MB));
            if (recovery < 0)
            {
                parts.append(BootCheck::tr("a recovery partition of %1 MB, for the Windows Recovery "
                                           "Environment").arg(recMake.mb));
            }
            rep += BootCheck::tr("They make the partitions Windows setup would have: %1.")
                       .arg(parts.join(", ")) + " ";
            if (shrinkMb)
            {
                rep += BootCheck::tr("The disk's free space does not hold all of them, so Windows' partition "
                          "is shrunk by %1 MB first; that needs as much free space in it, and fails "
                          "harmlessly if it is not there.").arg(shrinkMb);
            }
            else
            {
                rep += BootCheck::tr("They go in the disk's free space, at the offsets given; nothing is "
                          "shrunk.");
            }
            rep += " ";
        }
        rep += "\n\n";

        auto offset = [](const Make &m) {
            return m.offsetKb ? QString(" offset=%1").arg(m.offsetKb) : QString();
        };
        rep += QString("diskpart\nselect disk %1\nlist partition\n").arg(disk.number < 0 ? 0 : disk.number);
        if (haveW.isNull() || shrinkMb)
        {
            rep += QString("select partition %1\n").arg(number(windows));
            if (haveW.isNull()) rep += QString("assign letter=%1\n").arg(W);
            if (shrinkMb) rep += QString("shrink desired=%1 minimum=%1\n").arg(shrinkMb);
        }
        if (esp < 0)
        {
            rep += QString("create partition efi size=%1%2\nformat quick fs=fat32 label=\"System\"\n"
                           "assign letter=%3\n").arg(espMake.mb).arg(offset(espMake)).arg(S);
        }
        else if (haveS.isNull())
        {
            rep += QString("select partition %1\nassign letter=%2\n").arg(number(esp)).arg(S);
        }
        if (msrMake.mb)
        {
            rep += QString("create partition msr size=%1%2\n").arg(msrMake.mb).arg(offset(msrMake));
        }
        if (recovery < 0)
        {
            rep += QString("create partition primary size=%1%2\nformat quick fs=ntfs label=\"Recovery\"\n"
                           "set id=\"de94bba4-06d1-4d40-a16a-bfd50179d6ac\"\n"
                           "gpt attributes=0x8000000000000001\nassign letter=%3\n")
                       .arg(recMake.mb).arg(offset(recMake)).arg(R);
        }
        rep += "exit\n\n";
        rep += QString("bcdboot %1:\\Windows /s %2: /f UEFI\n").arg(W).arg(S);
        if (recovery < 0)
        {
            rep += QString("robocopy %1:\\Windows\\System32\\Recovery %2:\\Recovery\\WindowsRE Winre.wim /copy:DAT\n")
                       .arg(W).arg(R);
        }
        if (lettersStale) rep += regFix(windows, W);
        rep += "\n";
        rep += BootCheck::tr("bcdboot copies the boot manager to the EFI system partition and writes a boot "
                  "configuration that starts Windows on %1.").arg(partText(windows));
        if (recovery < 0)
        {
            rep += " " + BootCheck::tr("robocopy puts the recovery image, Winre.wim, on the new recovery "
                      "partition. It may well find none: while the recovery environment is enabled, "
                      "Winre.wim lives on the recovery partition, not in \\Windows\\System32\\Recovery, "
                      "so it went with that partition. Then copy Winre.wim to "
                      "\\Recovery\\WindowsRE on the new recovery partition from elsewhere -- the "
                      "recovery partition of another copy of this disk, or \\Windows\\System32\\Recovery "
                      "inside the setup media's install.wim of the same version -- or leave the "
                      "recovery environment disabled and skip the reagentc steps below; Windows "
                      "starts without it.");
        }
        rep += "\n\n";
        rep += BootCheck::tr("Once Windows starts, register the recovery environment in an administrator "
                  "command prompt in the VM -- the first line gives the recovery partition R: there:")
               + "\n\n";
        rep += "powershell -c \"Get-Partition -DiskNumber (Get-Partition -DriveLetter C).DiskNumber | "
               "? GptType -eq '{de94bba4-06d1-4d40-a16a-bfd50179d6ac}' | Add-PartitionAccessPath -AccessPath R:\\\"\n"
               "ren C:\\Windows\\System32\\Recovery\\ReAgent.xml ReAgent.xml.old\n"
               "reagentc /setreimage /path R:\\Recovery\\WindowsRE /target C:\\Windows\n"
               "reagentc /enable\n"
               "reagentc /info\n"
               "powershell -c \"Get-Partition -DiskNumber (Get-Partition -DriveLetter C).DiskNumber | "
               "? GptType -eq '{de94bba4-06d1-4d40-a16a-bfd50179d6ac}' | Remove-PartitionAccessPath -AccessPath R:\\\"\n";
        return out;
    }
    out.bcdFound = true;
    const QString bcdpath = efi ? "\\EFI\\Microsoft\\Boot\\BCD" : "\\Boot\\BCD";
    QList<BcdDeviceRef> refs;
    int objects = 0;
    QString why;
    if (!parseBcdHive(hive, &refs, &objects, &why))
    {
        rep += BootCheck::tr("The boot configuration on %1, %2, could not be read: %3.")
                   .arg(partText(bcdpart), bcdpath, why);
        return out;
    }
    rep += BootCheck::tr("Boot configuration: %1, %2, %3 objects.").arg(partText(bcdpart), bcdpath).arg(objects)
           + "\n\n";

    // Each reference: the partition it names, here, if any.
    QList<int> where;
    QSet<QByteArray> otherids;
    int stale = 0, missing = 0;
    for (const BcdDeviceRef &r : refs)
    {
        int at = -1;
        for (int i = 0; i < layout.partitions.size() && at < 0; ++i)
        {
            const CombinePartition &p = layout.partitions[i];
            if (r.gpt && gpt && p.entry.mid(16, 16) == r.partition) at = i;
            if (!r.gpt && !gpt && u64(r.partition, 0) == p.first * ss) at = i;
        }
        where.append(at);
        if (r.disk != diskid) otherids.insert(r.disk);
        if (at >= 0 && r.disk != diskid) ++stale;
        if (at < 0) ++missing;
    }
    if (refs.isEmpty())
    {
        rep += BootCheck::tr("No entry names a partition.") + "\n";
    }
    else
    {
        rep += BootCheck::tr("Entries that name a partition:") + "\n";
    }
    for (int k = 0; k < refs.size(); ++k)
    {
        const BcdDeviceRef &r = refs[k];
        QString name = elementName(r.objectType, r.element);
        if (name.isEmpty()) name = QString("0x%1").arg(r.element, 8, 16, QChar('0'));
        const QString status = (where[k] < 0) ? BootCheck::tr("MISSING") : (r.disk != diskid) ? BootCheck::tr("WRONG")
                                                                                 : BootCheck::tr("OK");
        QString what = (where[k] >= 0) ? partText(where[k])
                                       : (r.gpt ? BootCheck::tr("partition %1").arg(guidText(r.partition))
                                                : BootCheck::tr("the partition at byte %1").arg(u64(r.partition, 0)));
        if (r.disk != diskid) what += ", " + BootCheck::tr("on disk %1").arg(idText(r.disk));
        if (r.nested) what = BootCheck::tr("a ramdisk on %1").arg(what);
        rep += QString("  %1  %2: %3 = %4\n").arg(status, -7).arg(objectText(r), name, what);
    }
    rep += "\n";

    const bool bcdok = (stale == 0 && missing == 0);
    if (bcdok)
    {
        rep += BootCheck::tr("Every entry names this disk as it is now, and a partition it has: the boot "
                  "configuration matches the disk.") + "\n";
    }
    for (const QByteArray &id : otherids)
    {
        rep += (gpt ? BootCheck::tr("The boot configuration names the disk by GUID %1, but the disk's own is %2.")
                    : BootCheck::tr("The boot configuration names the disk by signature %1, but the disk's own is %2."))
                   .arg(idText(id), idText(diskid)) + " ";
        const QString elsewhere = disk.diskWithId ? disk.diskWithId(id) : QString();
        if (!elsewhere.isEmpty())
        {
            rep += BootCheck::tr("%1 on this computer has that ID: probably the disk this one was copied "
                      "from, which made Windows give this one a new ID when it was attached.")
                       .arg(elsewhere);
        }
        else
        {
            rep += BootCheck::tr("Windows gives a disk a new ID when it is attached where a disk with the "
                      "same one already is, as a copy is where its original is.");
        }
        rep += "\n";
    }
    if (missing)
    {
        rep += BootCheck::tr("%1 entries name a partition this disk does not have; bcdedit cannot point "
                  "those at the right partition, but bcdboot (below) can make new ones.")
                   .arg(missing) + "\n";
    }
    if (!bcdok) rep += BootCheck::tr("The disk will not boot until this is fixed.") + "\n";
    rep += "\n";

    // The Windows partition: the one the loader names, or, when no loader
    // entry names one the disk has, the largest basic data / NTFS partition.
    int windows = -1;
    for (int k = 0; k < refs.size(); ++k)
    {
        if (where[k] >= 0 && refs[k].objectType == 0x10200003 && refs[k].element == 0x21000001
            && !refs[k].nested && windows < 0)
        {
            windows = where[k];
        }
    }
    const bool loaderFound = windows >= 0;
    if (windows < 0)
    {
        static const QByteArray basicData = QByteArray::fromHex("a2a0d0ebe5b9334487c068b6b72699c7");
        unsigned long long best = 0;
        for (int i = 0; i < layout.partitions.size(); ++i)
        {
            const CombinePartition &p = layout.partitions[i];
            if (i == bcdpart) continue;
            const bool data = gpt ? p.entry.left(16) == basicData
                                  : p.entry.size() >= 16 && (unsigned char)p.entry[4] == 0x07;
            if (data && p.sectors > best)
            {
                best = p.sectors;
                windows = i;
            }
        }
    }

    const bool lettersStale = (windows >= 0) && checkLetters(windows);

    out.ok = bcdok && !lettersStale;
    if (out.ok) return out;

    // Letters for the partitions the commands use: the one a partition has
    // here already, or S: for the BCD's, W: for Windows, R: for any other,
    // or the next free one when that is taken.
    QString freeLetters = disk.freeLetters.isEmpty() ? QString("RSTUVWY") : disk.freeLetters;
    QMap<int, QChar> letters;
    QList<int> assign;
    auto letterFor = [&](int i, QChar want) {
        if (letters.contains(i)) return;
        const QChar have = disk.partitionLetters.value(layout.partitions[i].first);
        if (!have.isNull())
        {
            letters.insert(i, have);
            return;
        }
        const QChar got = freeLetters.contains(want) ? want
                        : freeLetters.isEmpty()      ? QChar('?')
                                                     : freeLetters[0];
        freeLetters.remove(got);
        letters.insert(i, got);
        assign.append(i);
    };
    if (!bcdok) letterFor(bcdpart, 'S');
    if (windows >= 0) letterFor(windows, 'W');
    if (!bcdok)
    {
        // Any other partition an entry to fix names, a recovery one say.
        for (int k = 0; k < refs.size(); ++k)
        {
            if (where[k] >= 0 && refs[k].disk != diskid) letterFor(where[k], 'R');
        }
    }
    const QChar S = letters.value(bcdpart, 'S');
    const QChar W = letters.value(windows, 'W');

    const QString store = QString("%1:%2").arg(S).arg(bcdpath);
    QStringList cmds;
    for (int k = 0; k < refs.size() && !bcdok; ++k)
    {
        const BcdDeviceRef &r = refs[k];
        if (where[k] < 0 || r.disk == diskid) continue;
        const QString name = elementName(r.objectType, r.element);
        const QChar letter = letters.value(where[k], '?');
        if (name.isEmpty())
        {
            cmds.append(QString("rem %1: element 0x%2 -- set it by the name \"bcdedit /store %3 "
                                "/enum all\" shows for it, to partition=%4:")
                            .arg(objectText(r)).arg(r.element, 8, 16, QChar('0')).arg(store).arg(letter));
        }
        else if (r.nested)
        {
            if (r.path.isEmpty() || r.options.isEmpty())
            {
                cmds.append(QString("rem %1 %2: a ramdisk on partition %3: -- see \"bcdedit /store %4 "
                                    "/enum all\" for its file and options")
                                .arg(objectText(r), name).arg(letter).arg(store));
            }
            else
            {
                cmds.append(QString("bcdedit /store %1 /set %2 %3 ramdisk=[%4:]%5,%6")
                                .arg(store, r.object, name).arg(letter).arg(r.path, r.options));
            }
        }
        else
        {
            cmds.append(QString("bcdedit /store %1 /set %2 %3 partition=%4:")
                            .arg(store, r.object, name).arg(letter));
        }
    }
    const bool rebuild = !bcdok && (cmds.isEmpty() || !loaderFound);
    const QString bcdboot = QString("bcdboot %1:\\Windows /s %2: /f %3\n").arg(W).arg(S).arg(efi ? "UEFI" : "BIOS");

    rep += BootCheck::tr("To fix it and keep the disk's new IDs, run the commands below in an administrator "
              "command prompt on this computer, with the disk attached as it is now (Disk %1, %2), "
              "and detach it before starting the VM. In a VM started from Windows setup media "
              "(Shift+F10), the same commands work with the VM's number for the disk.")
               .arg(disk.number)
               .arg(sizeText((disk.sectors ? disk.sectors
                              : layout.partitions.isEmpty() ? 0
                              : layout.partitions.last().first + layout.partitions.last().sectors) * ss))
           + "\n\n";
    if (!assign.isEmpty())
    {
        rep += QString("diskpart\nselect disk %1\nlist partition\n").arg(disk.number < 0 ? 0 : disk.number);
        for (int i : assign)
        {
            rep += QString("select partition %1\nassign letter=%2\n").arg(number(i)).arg(letters[i]);
        }
        rep += "exit\n\n";
    }
    if (rebuild)
    {
        // Nothing bcdedit can repoint, or not the loader: set the old store
        // aside and let bcdboot write a new one naming the disk as it is.
        rep += QString("ren %1 BCD.old\n").arg(store) + bcdboot;
    }
    else if (!bcdok)
    {
        for (const QString &cmd : cmds) rep += cmd + "\n";
        rep += QString("bcdedit /store %1 /enum all\n").arg(store);
    }
    if (lettersStale)
    {
        rep += regFix(windows, W);
    }
    rep += "\n";
    QStringList l;
    for (auto it = letters.begin(); it != letters.end(); ++it)
    {
        l.append(QString("%1: = %2").arg(it.value()).arg(partText(it.key())));
    }
    rep += BootCheck::tr("Partitions: %1.").arg(l.join("; ")) + "\n";
    if (windows < 0)
    {
        rep += BootCheck::tr("No Windows partition was found: give the one holding \\Windows a letter and "
                  "use it for W: in the commands.") + "\n";
    }
    if (lettersStale)
    {
        rep += BootCheck::tr("The reg commands clear Windows' old drive letters, which name the old "
                  "partitions, and record C: for %1; Windows gives the other volumes letters "
                  "again as it finds them.").arg(partText(windows)) + "\n";
    }
    if (!bcdok && !rebuild)
    {
        rep += "\n" + BootCheck::tr("Or rebuild the boot files and the boot configuration from that Windows "
                  "instead, which replaces the configuration's entries with new ones:") + "\n" + bcdboot;
    }
    if (!bcdok)
    {
        // bcdboot makes no recovery entry, and reagentc's ReAgent.xml still
        // names the old one, so "reagentc /enable" alone fails. The recovery
        // partition: by its type, GPT's Windows recovery or MBR's 0x27.
        const int recovery = findType(false);
        rep += "\n" + BootCheck::tr("bcdboot does not recreate the Windows Recovery Environment's entry, "
                  "and Windows' own record of it still names the old one, so \"reagentc /enable\" "
                  "alone fails. Once Windows starts, run these in an administrator command prompt "
                  "in the VM. \"select volume C\" picks the disk Windows is on; if R: is taken, use "
                  "another letter.") + "\n\n";
        if (recovery >= 0)
        {
            rep += QString("diskpart\nselect volume C\nselect partition %1\nassign letter=R\nexit\n\n")
                       .arg(number(recovery));
        }
        else
        {
            rep += BootCheck::tr("No recovery partition was found: give R: to the one holding "
                      "\\Recovery\\WindowsRE\\Winre.wim.") + "\n\n";
        }
        rep += "robocopy R:\\Recovery\\WindowsRE C:\\WinREBackup Winre.wim /copy:DAT\n"
               "ren C:\\Windows\\System32\\Recovery\\ReAgent.xml ReAgent.xml.old\n"
               "reagentc /setreimage /path R:\\Recovery\\WindowsRE /target C:\\Windows\n"
               "reagentc /enable\n"
               "reagentc /info\n\n";
        rep += BootCheck::tr("robocopy keeps a copy of Winre.wim in C:\\WinREBackup, in case reagentc moves "
                  "or loses it. \"reagentc /info\" should then say Enabled, with the recovery "
                  "partition as its location. To try it, restart into it from the VM's console -- "
                  "in Hyper-V, a Basic Session, not an Enhanced one or Remote Desktop, where it does "
                  "not work:") + "\n\n";
        rep += "shutdown /r /o /t 0\n\n";
        rep += BootCheck::tr("It should start at \"Choose an option\"; Continue goes back to Windows. "
                  "Then take the letter away again and delete the copy:") + "\n\n";
        if (recovery >= 0)
        {
            rep += QString("diskpart\nselect volume C\nselect partition %1\nremove letter=R\nexit\n")
                       .arg(number(recovery));
        }
        rep += "rmdir /s /q C:\\WinREBackup\n";
    }
    return out;
}
