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

// Reads images back through the real ImageSource and checks the bytes that come
// out are the bytes that went in.
//
// A decoder mistake corrupts an image on its way to a card without anything
// saying so. The fixtures are built here with zlib and liblzma, so the test
// needs no gzip or xz on the path and no compressed files in the repository.
//
// Known gap: nextMemberFollows() keeps a single leftover byte when the input
// buffer runs out with exactly one byte of the next gzip header in it. Reaching
// that from here would mean sizing a member's *compressed* length to land one
// byte short of the 1 MB input buffer, which these fixtures do not do. Removing
// that line leaves every check below passing.

#include <QCoreApplication>
#include <QByteArray>
#include <QFile>
#include <QString>
#include <windows.h>
#include <cstdio>
#include <cstring>
#include <zlib.h>
#include <lzma.h>
#include "imagesource.h"

static const unsigned long long SS = 512;

static int checks = 0;
static int failures = 0;

static void check(bool ok, const char *what)
{
    printf("  %s %s\n", ok ? "ok  " : "FAIL", what);
    ++checks;
    if (!ok) ++failures;
}

// A pattern that does not compress to nothing and where any displacement shows
// up immediately: each 8 bytes carries its own offset.
static QByteArray pattern(int bytes)
{
    QByteArray b(bytes, 0);
    unsigned char *p = (unsigned char *)b.data();
    for (int i = 0; i + 8 <= bytes; i += 8)
    {
        unsigned int a = (unsigned int)(i / 8);
        unsigned int c = a * 2654435761u;
        memcpy(p + i, &a, 4);
        memcpy(p + i + 4, &c, 4);
    }
    return b;
}

static QByteArray gzipOf(const QByteArray &raw)
{
    z_stream zs;
    memset(&zs, 0, sizeof(zs));
    // 16 + MAX_WBITS asks zlib for a gzip wrapper rather than a zlib one.
    if (deflateInit2(&zs, 6, Z_DEFLATED, 16 + MAX_WBITS, 8, Z_DEFAULT_STRATEGY) != Z_OK)
    {
        return QByteArray();
    }
    QByteArray out(raw.size() + (raw.size() / 8) + 4096, 0);
    zs.next_in = (Bytef *)raw.constData();
    zs.avail_in = (uInt)raw.size();
    zs.next_out = (Bytef *)out.data();
    zs.avail_out = (uInt)out.size();
    const int r = deflate(&zs, Z_FINISH);
    const int used = (int)((char *)zs.next_out - out.constData());
    deflateEnd(&zs);
    if (r != Z_STREAM_END)
    {
        return QByteArray();
    }
    out.truncate(used);
    return out;
}

static QByteArray xzOf(const QByteArray &raw)
{
    size_t cap = (size_t)raw.size() + 65536;
    QByteArray out((int)cap, 0);
    size_t used = 0;
    if (lzma_easy_buffer_encode(1, LZMA_CHECK_CRC64, NULL,
                                (const uint8_t *)raw.constData(), (size_t)raw.size(),
                                (uint8_t *)out.data(), &used, cap) != LZMA_OK)
    {
        return QByteArray();
    }
    out.truncate((int)used);
    return out;
}

static bool writeFile(const QString &name, const QByteArray &bytes)
{
    QFile f(name);
    if (!f.open(QIODevice::WriteOnly))
    {
        return false;
    }
    const bool ok = f.write(bytes) == (qint64)bytes.size();
    f.close();
    return ok;
}

// Read the whole image through ImageSource, a few sectors at a time, the way
// the write path does. Returns false with the reason in *why.
static bool readBack(const QString &name, QByteArray *got, QString *why)
{
    ImageSource src;
    if (!src.open(name, SS))
    {
        *why = src.errorString();
        return false;
    }
    got->clear();
    unsigned long long sector = 0;
    for (;;)
    {
        unsigned long long produced = 0;
        char *data = src.read(sector, 8, &produced);
        if (data == NULL)
        {
            *why = src.errorString();
            src.close();
            return false;
        }
        if (produced == 0)
        {
            delete[] data;
            break;
        }
        got->append(data, (int)(produced * SS));
        delete[] data;
        sector += produced;
    }
    src.close();
    return true;
}

// The image is padded out to a whole sector, so only the original bytes are
// compared; everything past them must be zero rather than stale buffer.
static void caseRoundTrip(const char *name, const QString &file, const QByteArray &raw)
{
    printf("%s\n", name);
    QByteArray got;
    QString why;
    if (!readBack(file, &got, &why))
    {
        printf("  FAIL %s\n", why.toLocal8Bit().constData());
        ++checks; ++failures;
        printf("\n");
        return;
    }
    const unsigned long long wantsize = ((unsigned long long)raw.size() + SS - 1) / SS * SS;
    check((unsigned long long)got.size() == wantsize,
          "read back exactly the image, rounded up to a whole sector");
    check(got.size() >= raw.size()
          && memcmp(got.constData(), raw.constData(), (size_t)raw.size()) == 0,
          "every byte matches the original");
    bool padzero = true;
    for (int i = raw.size(); i < got.size(); ++i)
    {
        if (got.at(i) != 0) { padzero = false; break; }
    }
    check(padzero, "the padding to a whole sector is zeros");
    printf("\n");
}

// A truncated stream must fail as truncated -- not as damaged data, which
// would also fail but means the end-of-input path was never reached -- and
// what came before the cut must still have been decoded correctly.
static void caseRejected(const char *name, const QString &file, const QByteArray &raw)
{
    printf("%s\n", name);
    QByteArray got;
    QString why;
    const bool ok = readBack(file, &got, &why);
    check(!ok, "reported a failure rather than returning short data");
    if (!ok)
    {
        printf("  -> %s\n", why.toLocal8Bit().constData());
        check(why.contains("ends in the middle"), "reported as truncated, not as damaged");
        check(got.size() > 0 && got.size() <= raw.size()
                  && memcmp(got.constData(), raw.constData(), (size_t)got.size()) == 0,
              "everything decoded before the cut matches the original");
    }
    printf("\n");
}

// open() decides by content: a misnamed image must still be read correctly.
static void caseMisnamed(const char *name, const QString &file, const QByteArray &raw,
                         bool expectCompressed)
{
    printf("%s\n", name);
    ImageSource src;
    check(src.open(file, SS) && src.isCompressed() == expectCompressed,
          expectCompressed ? "detected as compressed despite the name"
                           : "detected as raw despite the name");
    src.close();
    printf("\n");
    caseRoundTrip("  ...read back", file, raw);
}

static unsigned long long sectorsOf(const QByteArray &raw)
{
    return ((unsigned long long)raw.size() + SS - 1) / SS;
}

// The write and verify loops stop at sizeInSectors() whenever sizeKnown(), so
// a size claimed exact but wrong truncates the image with nothing reported.
static void caseSize(const char *name, const QString &file, const QByteArray &raw)
{
    printf("%s: reported size\n", name);
    ImageSource src;
    if (!src.open(file, SS))
    {
        check(false, "opened");
        printf("\n");
        return;
    }
    if (src.sizeKnown())
    {
        check(src.sizeInSectors() == sectorsOf(raw), "a size reported exact is the real size");
    }
    else
    {
        check(src.sizeInSectors() <= sectorsOf(raw),
              "size unknown, and the estimate does not overshoot the image");
    }
    src.close();
    printf("\n");
}

// Reads at a later sector first: raw seeks, compressed decompresses and
// discards up to it. Then asks to go back, which only raw can do.
static void caseSeek(const char *name, const QString &file, const QByteArray &raw,
                     bool compressed)
{
    printf("%s: reading out of order\n", name);
    ImageSource src;
    if (!src.open(file, SS))
    {
        check(false, "opened");
        printf("\n");
        return;
    }
    const unsigned long long at = 3000;   // well past the first input buffer
    unsigned long long produced = 0;
    char *data = src.read(at, 4, &produced);
    check(data != NULL && produced == 4
          && memcmp(data, raw.constData() + at * SS, 4 * SS) == 0,
          "a later sector read first holds that sector's data");
    delete[] data;

    data = src.read(10, 1, &produced);
    if (compressed)
    {
        check(data == NULL && !src.errorString().isEmpty(),
              "going backwards is refused, not answered with the wrong sector");
    }
    else
    {
        check(data != NULL && produced == 1
              && memcmp(data, raw.constData() + 10 * SS, SS) == 0,
              "going backwards reads the right sector");
    }
    delete[] data;
    src.close();
    printf("\n");
}

// "Read to .img.gz/.img.xz": everything written through ImageSink must come
// back out of ImageSource unchanged. The chunk sizes are uneven and one is
// larger than the sink's internal buffer.
static void caseSinkRoundTrip(const char *name, const QString &file,
                              ImageSink::Format format, const QByteArray &raw)
{
    printf("%s\n", name);
    ImageSink sink;
    bool ok = sink.open(file, format);
    check(ok, "the sink opened");
    const int sizes[] = { 777, 1536 * 1024, 1, 4096 };
    int pos = 0;
    for (int i = 0; ok && pos < raw.size(); i = (i + 1) % 4)
    {
        const int n = qMin(sizes[i], raw.size() - pos);
        ok = sink.write(raw.constData() + pos, (unsigned long long)n);
        pos += n;
    }
    check(ok, "every write was accepted");
    check(ok && sink.finish(), "finish() flushed and closed the file");

    QFile f(file);
    QByteArray head;
    if (f.open(QIODevice::ReadOnly))
    {
        head = f.read(6);
        f.close();
    }
    check(format == ImageSink::FORMAT_GZIP
              ? head.startsWith("\x1f\x8b")
              : head == QByteArray("\xfd" "7zXZ\x00", 6),
          "the file really is compressed in the chosen format");
    printf("\n");
    caseRoundTrip("  ...read back", file, raw);
}

// abort() leaves a truncated stream, which the reader must reject rather than
// hand back as a complete, shorter image.
static void caseSinkAbort(const char *name, const QString &file,
                          ImageSink::Format format, const QByteArray &raw)
{
    printf("%s\n", name);
    {
        ImageSink sink;
        check(sink.open(file, format) && sink.write(raw.constData(), raw.size() / 2),
              "half the image was written");
        sink.abort();
    }
    QByteArray got;
    QString why;
    check(!readBack(file, &got, &why), "the aborted file is not read as a valid image");
    printf("\n");
}

static void caseNames()
{
    printf("output names for Read\n");
    struct { const char *typed; bool gz, xz; const char *want; } cases[] = {
        { "C:\\a\\myimage",          false, false, "C:\\a\\myimage.img" },
        { "C:\\a\\myimage",          true,  false, "C:\\a\\myimage.img.gz" },
        { "C:\\a\\myimage",          false, true,  "C:\\a\\myimage.img.xz" },
        { "C:\\a\\myimage.img",      false, false, "C:\\a\\myimage.img" },
        { "C:\\a\\myimage.img",      true,  false, "C:\\a\\myimage.img.gz" },
        { "C:\\a\\myimage.IMG",      false, true,  "C:\\a\\myimage.IMG.xz" },
        { "C:\\a\\myimage.img.gz",   true,  false, "C:\\a\\myimage.img.gz" },
        { "C:\\a\\myimage.img.gz",   false, true,  "C:\\a\\myimage.img.gz.img.xz" },
        { "C:\\a\\myimage.img.xz",   false, false, "C:\\a\\myimage.img.xz.img" },
        { "C:\\a\\disk.bin",         false, false, "C:\\a\\disk.bin.img" },
    };
    for (const auto &c : cases)
    {
        const QString got = ImageSink::readTargetName(QString::fromLatin1(c.typed), c.gz, c.xz);
        const QByteArray what = QByteArray(c.typed) + (c.gz ? " +gz" : c.xz ? " +xz" : " raw")
                                + " -> " + c.want;
        check(got == QString::fromLatin1(c.want), what.constData());
    }
    check(ImageSource::nameLooksCompressed("x.img.gz") && ImageSource::nameLooksCompressed("X.IMG.XZ")
              && !ImageSource::nameLooksCompressed("x.img"),
          "nameLooksCompressed goes by .gz/.xz, case-insensitively");
    printf("\n");
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    caseNames();

    // Not a whole number of sectors, so the last-sector padding is exercised.
    const QByteArray raw = pattern(3 * 1024 * 1024) + QByteArray("TAIL");
    const QByteArray gz = gzipOf(raw);
    const QByteArray xz = xzOf(raw);

    printf("fixtures\n");
    check(!gz.isEmpty(), "gzip fixture built");
    check(!xz.isEmpty(), "xz fixture built");
    check((raw.size() % (int)SS) != 0, "the image is not a whole number of sectors");
    printf("\n");
    if (gz.isEmpty() || xz.isEmpty())
    {
        printf("%d checks, %d failures\n", checks, failures);
        return 1;
    }

    // Split at an offset that is deliberately not sector-aligned, so a member
    // boundary falls in the middle of a sector.
    const int cut = 1234567;
    const QByteArray gz1 = gzipOf(raw.left(cut)), gz2 = gzipOf(raw.mid(cut));
    const QByteArray xz1 = xzOf(raw.left(cut)),   xz2 = xzOf(raw.mid(cut));

    // Checked: a failed write could leave a stale file from an earlier run to
    // be tested instead.
    printf("fixture files\n");
    check(writeFile("imgtest.img", raw)
          && writeFile("imgtest.img.gz", gz)
          && writeFile("imgtest.img.xz", xz)
          && writeFile("imgtest-multi.img.gz", gz1 + gz2)
          && writeFile("imgtest-multi.img.xz", xz1 + xz2)
          && writeFile("imgtest-padded.img.xz", xz1 + QByteArray(4, '\0') + xz2)
          && writeFile("imgtest-trunc.img.gz", gz.left(gz.size() / 2))
          && writeFile("imgtest-trunc.img.xz", xz.left(xz.size() / 2))
          && writeFile("imgtest-gz-named.img", gz)
          && writeFile("imgtest-raw-named.img.gz", raw),
          "every fixture file was written");
    printf("\n");

    caseRoundTrip("raw", "imgtest.img", raw);
    caseRoundTrip("gzip", "imgtest.img.gz", raw);
    caseRoundTrip("xz", "imgtest.img.xz", raw);
    caseRoundTrip("gzip, two members", "imgtest-multi.img.gz", raw);
    caseRoundTrip("xz, two streams", "imgtest-multi.img.xz", raw);
    caseRoundTrip("xz, two streams with padding between", "imgtest-padded.img.xz", raw);

    // Truncation must be reported, or half an image is written and called done.
    caseRejected("gzip that stops in the middle", "imgtest-trunc.img.gz", raw);
    caseRejected("xz that stops in the middle", "imgtest-trunc.img.xz", raw);

    caseMisnamed("gzip named .img", "imgtest-gz-named.img", raw, true);
    caseMisnamed("raw named .img.gz", "imgtest-raw-named.img.gz", raw, false);

    caseSize("raw", "imgtest.img", raw);
    caseSize("gzip", "imgtest.img.gz", raw);
    caseSize("xz", "imgtest.img.xz", raw);
    // Its trailer records the last member's size only.
    caseSize("gzip, two members", "imgtest-multi.img.gz", raw);
    caseSize("xz, two streams", "imgtest-multi.img.xz", raw);
    caseSize("xz, two streams with padding between", "imgtest-padded.img.xz", raw);
    // Mostly zeros, like a real disk image: small enough compressed that the
    // last member's size passes for the whole image's.
    {
        QByteArray sparse(3 * 1024 * 1024, 0);
        sparse.replace(0, 4096, pattern(4096));
        sparse.append("TAIL");
        const int scut = 2 * 1024 * 1024 + 100;
        writeFile("imgtest-sparse.img.gz", gzipOf(sparse.left(scut)) + gzipOf(sparse.mid(scut)));
        caseSize("gzip, two members, compressible", "imgtest-sparse.img.gz", sparse);
        caseRoundTrip("gzip, two members, compressible", "imgtest-sparse.img.gz", sparse);
        DeleteFileA("imgtest-sparse.img.gz");
    }

    caseSeek("raw", "imgtest.img", raw, false);
    caseSeek("gzip", "imgtest.img.gz", raw, true);
    caseSeek("xz", "imgtest.img.xz", raw, true);

    caseSinkRoundTrip("ImageSink, gzip", "imgtest-sink.img.gz", ImageSink::FORMAT_GZIP, raw);
    caseSinkRoundTrip("ImageSink, xz", "imgtest-sink.img.xz", ImageSink::FORMAT_XZ, raw);
    caseSinkAbort("ImageSink, gzip aborted", "imgtest-abort.img.gz", ImageSink::FORMAT_GZIP, raw);
    caseSinkAbort("ImageSink, xz aborted", "imgtest-abort.img.xz", ImageSink::FORMAT_XZ, raw);

    const char *leftovers[] = {
        "imgtest.img", "imgtest.img.gz", "imgtest.img.xz",
        "imgtest-multi.img.gz", "imgtest-multi.img.xz", "imgtest-padded.img.xz",
        "imgtest-trunc.img.gz", "imgtest-trunc.img.xz",
        "imgtest-sink.img.gz", "imgtest-sink.img.xz",
        "imgtest-abort.img.gz", "imgtest-abort.img.xz",
        "imgtest-gz-named.img", "imgtest-raw-named.img.gz",
    };
    for (size_t i = 0; i < sizeof(leftovers) / sizeof(leftovers[0]); ++i)
    {
        DeleteFileA(leftovers[i]);
    }

    printf("%d checks, %d failures\n", checks, failures);
    return failures ? 1 : 0;
}
