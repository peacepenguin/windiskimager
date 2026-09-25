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
// saying so. The fixtures are built here with zlib, liblzma, libbz2 and
// libzstd, so the test needs no compressors on the path and no compressed
// files in the repository.
//
// nextMemberFollows() keeps the leftover bytes when the input buffer runs out
// part way into the next stream's header. The zstd "header split across the
// input buffer" case reaches that; gzip and bzip2 share the code but have no
// fixture of their own for it.

#include <QCoreApplication>
#include <QByteArray>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <windows.h>
#include <cstdio>
#include <cstring>
#include <zlib.h>
#include <lzma.h>
#include <bzlib.h>
#include <zstd.h>
#include "imagesource.h"
#include "transferpipe.h"

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

// Many blocks, as xz -T writes: what the multi-threaded decoder decodes in
// parallel. lzma_easy_buffer_encode above always writes one.
static QByteArray xzBlocksOf(const QByteArray &raw)
{
    lzma_stream ls = LZMA_STREAM_INIT;
    lzma_mt mt;
    memset(&mt, 0, sizeof(mt));
    mt.threads = 4;
    mt.block_size = 256 * 1024;
    mt.preset = 1;
    mt.check = LZMA_CHECK_CRC64;
    if (lzma_stream_encoder_mt(&ls, &mt) != LZMA_OK)
    {
        return QByteArray();
    }
    QByteArray out(raw.size() + 65536, 0);
    ls.next_in = (const uint8_t *)raw.constData();
    ls.avail_in = (size_t)raw.size();
    ls.next_out = (uint8_t *)out.data();
    ls.avail_out = (size_t)out.size();
    lzma_ret r;
    do
    {
        r = lzma_code(&ls, LZMA_FINISH);
    } while (r == LZMA_OK);
    const int used = (int)ls.total_out;
    lzma_end(&ls);
    if (r != LZMA_STREAM_END)
    {
        return QByteArray();
    }
    out.truncate(used);
    return out;
}

static QByteArray bzip2Of(const QByteArray &raw)
{
    unsigned int cap = (unsigned int)raw.size() + (unsigned int)raw.size() / 100 + 1024;
    QByteArray out((int)cap, 0);
    if (BZ2_bzBuffToBuffCompress(out.data(), &cap, (char *)raw.constData(),
                                 (unsigned int)raw.size(), 9, 0, 0) != BZ_OK)
    {
        return QByteArray();
    }
    out.truncate((int)cap);
    return out;
}

// One frame. With sizeFlag false the frame leaves out its content size, as
// zstd does when it compresses from a pipe; with checksum true it carries the
// content checksum the zstd tool adds by default.
//
// windowLog above 27 declares a window past what zstd decodes by default, as
// zstd --long does; left unset, the level's own window is used.
static QByteArray zstdOf(const QByteArray &raw, bool sizeFlag = true, bool checksum = false,
                         int windowLog = 0)
{
    ZSTD_CCtx *cc = ZSTD_createCCtx();
    if (cc == NULL)
    {
        return QByteArray();
    }
    ZSTD_CCtx_setParameter(cc, ZSTD_c_compressionLevel, 3);
    if (windowLog)
    {
        ZSTD_CCtx_setParameter(cc, ZSTD_c_windowLog, windowLog);
    }
    ZSTD_CCtx_setParameter(cc, ZSTD_c_contentSizeFlag, sizeFlag ? 1 : 0);
    ZSTD_CCtx_setParameter(cc, ZSTD_c_checksumFlag, checksum ? 1 : 0);
    QByteArray out((int)ZSTD_compressBound((size_t)raw.size()), 0);
    ZSTD_outBuffer o = { out.data(), (size_t)out.size(), 0 };
    if (sizeFlag)
    {
        ZSTD_CCtx_setPledgedSrcSize(cc, (unsigned long long)raw.size());
    }
    // Fed a piece at a time, as from a pipe: given everything in one call
    // zstd knows the size anyway and shrinks the window to fit it.
    const size_t PIECE = 64 * 1024;
    size_t left = 0;
    for (size_t at = 0; !ZSTD_isError(left); )
    {
        const size_t n = qMin(PIECE, (size_t)raw.size() - at);
        const bool last = (at + n == (size_t)raw.size());
        ZSTD_inBuffer in = { raw.constData() + at, n, 0 };
        do
        {
            left = ZSTD_compressStream2(cc, &o, &in, last ? ZSTD_e_end : ZSTD_e_continue);
        } while (!ZSTD_isError(left) && (last ? left != 0 : in.pos < in.size));
        at += n;
        if (last)
        {
            break;
        }
    }
    ZSTD_freeCCtx(cc);
    if (ZSTD_isError(left))
    {
        return QByteArray();
    }
    out.truncate((int)o.pos);
    return out;
}

// A zstd skippable frame, as pzstd writes ahead of its data frames.
static QByteArray zstdSkippable(int payload)
{
    QByteArray b(8 + payload, '\x5a');
    const unsigned char head[8] = { 0x50, 0x2a, 0x4d, 0x18,
                                    (unsigned char)payload, (unsigned char)(payload >> 8),
                                    (unsigned char)(payload >> 16), (unsigned char)(payload >> 24) };
    memcpy(b.data(), head, 8);
    return b;
}

// Bytes zstd cannot compress, so it stores them and a frame's compressed size
// follows its input size exactly -- which is what lets a fixture put a frame
// boundary at a chosen byte of the file.
static QByteArray noise(int bytes, unsigned int seed)
{
    QByteArray b(bytes, 0);
    unsigned int x = seed | 1u;
    for (int i = 0; i < bytes; ++i)
    {
        x ^= x << 13; x ^= x >> 17; x ^= x << 5;
        b[i] = (char)(x >> 24);
    }
    return b;
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
    bool right = false;
    switch (format)
    {
        case ImageSink::FORMAT_GZIP:  right = head.startsWith("\x1f\x8b"); break;
        case ImageSink::FORMAT_XZ:    right = head == QByteArray("\xfd" "7zXZ\x00", 6); break;
        case ImageSink::FORMAT_BZIP2: right = head.startsWith("BZh9"); break;
        case ImageSink::FORMAT_ZSTD:  right = head.startsWith("\x28\xb5\x2f\xfd"); break;
    }
    check(right, "the file really is compressed in the chosen format");
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
    // A multi-threaded compressor can still hold everything written so far,
    // leaving an empty file -- which Write refuses outright ("contains no
    // data"), so it counts as rejected too.
    const bool empty = QFileInfo(file).size() == 0;
    check(empty || !readBack(file, &got, &why), "the aborted file is not read as a valid image");
    printf("\n");
}

// Write and verify stop at a known size, then read one sector more: that is
// what makes a decoder reach the end of the stream and check it. A clean
// image must answer "nothing left"; a damaged trailer must be an error.
static void caseEndProbe(const char *name, const QString &file, const QByteArray &raw,
                         bool expectError)
{
    printf("%s: reading past the end\n", name);
    ImageSource src;
    if (!src.open(file, SS))
    {
        check(false, "opened");
        printf("\n");
        return;
    }
    const unsigned long long n = sectorsOf(raw);
    bool ok = true;
    for (unsigned long long at = 0; ok && at < n; at += 8)
    {
        unsigned long long produced = 0;
        char *data = src.read(at, qMin(8ull, n - at), &produced);
        ok = (data != NULL);
        delete[] data;
    }
    if (expectError)
    {
        // zlib checks the trailer as soon as it is in the same input buffer
        // as the last data, so the error can come before the extra read.
        bool reported = !ok;
        if (ok)
        {
            unsigned long long leftover = 0;
            char *extra = src.read(n, 1, &leftover);
            reported = (extra == NULL);
            delete[] extra;
        }
        check(reported && !src.errorString().isEmpty(),
              "the damaged trailer is reported by the time the stream has been read to its end");
    }
    else
    {
        check(ok, "the whole image up to its size reads without error");
        unsigned long long leftover = 99;
        char *extra = src.read(n, 1, &leftover);
        check(extra != NULL && leftover == 0, "one sector past the end: no error, nothing left");
        delete[] extra;
    }
    src.close();
    printf("\n");
}

// Write and Verify take the image from ImagePrefetcher, on a worker thread.
// What it hands out must be exactly what reading in order hands out: the
// same bytes, the same short last chunk, the same error at the same place.
static void casePrefetch(const char *name, const QString &file, const QByteArray &raw,
                         unsigned long long chunksectors, bool expectError)
{
    printf("%s: read ahead on a thread, %llu-sector chunks\n", name, chunksectors);
    ImageSource src;
    if (!src.open(file, SS))
    {
        check(false, "opened");
        printf("\n");
        return;
    }
    // Past the image, as Write asks for when the size is only an estimate.
    const unsigned long long total = sectorsOf(raw) + 3 * chunksectors;
    ImagePrefetcher prefetch(&src, total, chunksectors, SS);
    prefetch.start();
    QByteArray got;
    QString error;
    bool inorder = true, shortlast = false;
    unsigned long long expect = 0;
    ImagePrefetcher::Chunk c;
    while (prefetch.next(&c))
    {
        inorder = inorder && (c.start == expect);
        expect += c.count;
        if (!c.ok)
        {
            error = c.error;
            prefetch.release(c.data);
            break;
        }
        got.append(c.data, (int)(c.got * SS));
        shortlast = (c.got < c.count);
        prefetch.release(c.data);
    }
    prefetch.stop();
    check(inorder, "chunks arrive in order, back to back");
    if (expectError)
    {
        check(error.contains("ends in the middle"), "the truncation is reported, as reading in order would");
        check(got.size() <= raw.size() && memcmp(got.constData(), raw.constData(), (size_t)got.size()) == 0,
              "everything before it matches the original");
    }
    else
    {
        check(error.isEmpty() && shortlast, "no error, and it stops at the end of the image");
        check(got.size() >= raw.size() && memcmp(got.constData(), raw.constData(), (size_t)raw.size()) == 0,
              "every byte matches the original");
        // The image is the caller's again once stopped.
        unsigned long long leftover = 99;
        char *extra = src.read(sectorsOf(raw), 1, &leftover);
        check(extra != NULL && leftover == 0, "after stop(), reading on from the end still works");
        delete[] extra;
    }
    src.close();
    printf("\n");
}

// Cancel: the caller stops after a few chunks with more queued and the worker
// possibly waiting for a buffer. stop() must return, not deadlock.
static void casePrefetchStop(const QString &file)
{
    printf("read ahead, stopped part way\n");
    ImageSource src;
    check(src.open(file, SS), "opened");
    {
        ImagePrefetcher prefetch(&src, 1000000, 16, SS);
        prefetch.start();
        ImagePrefetcher::Chunk c;
        for (int k = 0; k < 3 && prefetch.next(&c); ++k)
        {
            prefetch.release(c.data);
        }
        // One chunk kept, never released, as a caller bailing out mid-loop does.
        prefetch.next(&c);
        prefetch.stop();
    }
    check(true, "stop() returned with chunks still queued");
    src.close();
    printf("\n");
}

// A compressed Read feeds ImageSink through SinkWriter on a worker thread,
// in pieces of any size, the gap zeros included.
static void caseSinkWriter(const char *name, const QString &file,
                           ImageSink::Format format, const QByteArray &raw)
{
    printf("%s: compressed on a thread\n", name);
    ImageSink sink;
    bool ok = sink.open(file, format);
    check(ok, "the sink opened");
    SinkWriter writer(&sink, 64 * 1024);
    writer.start();
    const int sizes[] = { 777, 300 * 1024, 1, 4096 };
    int pos = 0;
    for (int i = 0; ok && pos < raw.size(); i = (i + 1) % 4)
    {
        const int n = qMin(sizes[i], raw.size() - pos);
        ok = writer.write(raw.constData() + pos, (unsigned long long)n);
        pos += n;
    }
    check(ok && writer.finish(), "every piece was written, in order");
    check(sink.finish(), "the sink finished the stream");
    printf("\n");
    caseRoundTrip("  ...read back", file, raw);
}

// A Read to, and a Write/Verify from, an image path past MAX_PATH. The app's
// CreateFileW calls pass the path as it is, with no \\?\ prefix, so this only
// works through the manifest's longPathAware -- this harness carries the same
// setting (imgtest.manifest). Windows also needs LongPathsEnabled; without it
// the case is skipped rather than failed.
static void caseLongPath(const QByteArray &raw)
{
    printf("an image path longer than MAX_PATH\n");
    DWORD enabled = 0, size = sizeof(enabled);
    if (RegGetValueW(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\FileSystem",
                     L"LongPathsEnabled", RRF_RT_REG_DWORD, NULL, &enabled, &size) != ERROR_SUCCESS
        || enabled != 1)
    {
        printf("  skip long paths are not enabled on this machine (LongPathsEnabled)\n\n");
        return;
    }

    wchar_t cwd[32768];
    if (!GetCurrentDirectoryW(32768, cwd))
    {
        check(false, "read the working directory");
        return;
    }
    QStringList dirs;
    QString dir = QString::fromWCharArray(cwd);
    for (int i = 0; i < 5; ++i)
    {
        dir += "\\" + QString("imgtest-long-path-segment-%1-").arg(i) + QString(40, QChar('x'));
        dirs.append(dir);
    }
    const QString file = dir + "\\image.img.xz";
    check(file.length() > MAX_PATH, "fixture: the path is longer than MAX_PATH");
    bool made = true;
    for (const QString &d : dirs)
    {
        made = made && (CreateDirectoryW((LPCWSTR)d.utf16(), NULL)
                        || GetLastError() == ERROR_ALREADY_EXISTS);
    }
    check(made, "the directories were created without a \\\\?\\ prefix");
    if (made)
    {
        ImageSink sink;
        check(sink.open(file, ImageSink::FORMAT_XZ)
                  && sink.write(raw.constData(), (unsigned long long)raw.size())
                  && sink.finish(),
              "ImageSink wrote the image there");
        printf("\n");
        caseRoundTrip("  ...and ImageSource read it back", file, raw);
        DeleteFileW((LPCWSTR)file.utf16());
    }
    for (int i = dirs.size() - 1; i >= 0; --i)
    {
        RemoveDirectoryW((LPCWSTR)dirs.at(i).utf16());
    }
    if (!made) printf("\n");
}

static void caseNames()
{
    printf("output names for Read\n");
    // A null format means an uncompressed Read.
    const ImageSink::Format GZ = ImageSink::FORMAT_GZIP, XZ = ImageSink::FORMAT_XZ,
                            BZ = ImageSink::FORMAT_BZIP2, ZS = ImageSink::FORMAT_ZSTD;
    struct { const char *typed; const ImageSink::Format *format; const char *want; } cases[] = {
        { "C:\\a\\myimage",          NULL, "C:\\a\\myimage.img" },
        { "C:\\a\\myimage",          &GZ,  "C:\\a\\myimage.img.gz" },
        { "C:\\a\\myimage",          &XZ,  "C:\\a\\myimage.img.xz" },
        { "C:\\a\\myimage",          &BZ,  "C:\\a\\myimage.img.bz2" },
        { "C:\\a\\myimage",          &ZS,  "C:\\a\\myimage.img.zst" },
        { "C:\\a\\myimage.img",      NULL, "C:\\a\\myimage.img" },
        { "C:\\a\\myimage.img",      &GZ,  "C:\\a\\myimage.img.gz" },
        { "C:\\a\\myimage.img",      &ZS,  "C:\\a\\myimage.img.zst" },
        { "C:\\a\\myimage.IMG",      &XZ,  "C:\\a\\myimage.IMG.xz" },
        { "C:\\a\\myimage.IMG",      &BZ,  "C:\\a\\myimage.IMG.bz2" },
        { "C:\\a\\myimage.img.gz",   &GZ,  "C:\\a\\myimage.img.gz" },
        { "C:\\a\\myimage.img.gz",   &XZ,  "C:\\a\\myimage.img.gz.img.xz" },
        { "C:\\a\\myimage.img.zst",  &ZS,  "C:\\a\\myimage.img.zst" },
        { "C:\\a\\myimage.img.bz2",  &ZS,  "C:\\a\\myimage.img.bz2.img.zst" },
        { "C:\\a\\myimage.img.xz",   NULL, "C:\\a\\myimage.img.xz.img" },
        { "C:\\a\\disk.bin",         NULL, "C:\\a\\disk.bin.img" },
    };
    for (const auto &c : cases)
    {
        const bool compressed = (c.format != NULL);
        const ImageSink::Format format = compressed ? *c.format : GZ;
        const QString got = ImageSink::readTargetName(QString::fromLatin1(c.typed), compressed, format);
        const QByteArray what = QByteArray(c.typed)
                                + (compressed ? " +" + ImageSink::extension(format).mid(1).toLatin1()
                                              : QByteArray(" raw"))
                                + " -> " + c.want;
        check(got == QString::fromLatin1(c.want), what.constData());
    }
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
    const QByteArray bz = bzip2Of(raw);
    const QByteArray zst = zstdOf(raw);
    const QByteArray xzb = xzBlocksOf(raw);

    printf("fixtures\n");
    check(!gz.isEmpty(), "gzip fixture built");
    check(!xz.isEmpty(), "xz fixture built");
    check(!bz.isEmpty(), "bzip2 fixture built");
    check(!zst.isEmpty(), "zstd fixture built");
    check(!xzb.isEmpty(), "multi-block xz fixture built");
    check((raw.size() % (int)SS) != 0, "the image is not a whole number of sectors");
    printf("\n");
    if (gz.isEmpty() || xz.isEmpty() || bz.isEmpty() || zst.isEmpty())
    {
        printf("%d checks, %d failures\n", checks, failures);
        return 1;
    }

    // Split at an offset that is deliberately not sector-aligned, so a member
    // boundary falls in the middle of a sector.
    const int cut = 1234567;
    const QByteArray gz1 = gzipOf(raw.left(cut)), gz2 = gzipOf(raw.mid(cut));
    const QByteArray xz1 = xzOf(raw.left(cut)),   xz2 = xzOf(raw.mid(cut));
    const QByteArray bz1 = bzip2Of(raw.left(cut)), bz2 = bzip2Of(raw.mid(cut));
    const QByteArray zst1 = zstdOf(raw.left(cut)), zst2 = zstdOf(raw.mid(cut));

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
          && writeFile("imgtest-raw-named.img.gz", raw)
          && writeFile("imgtest.img.bz2", bz)
          && writeFile("imgtest.img.zst", zst)
          && writeFile("imgtest-multi.img.bz2", bz1 + bz2)
          && writeFile("imgtest-multi.img.zst", zst1 + zst2)
          // pzstd's layout: a skippable frame ahead of each data frame.
          && writeFile("imgtest-pzstd.img.zst", zstdSkippable(12) + zst1 + zstdSkippable(0) + zst2)
          && writeFile("imgtest-nosize.img.zst", zstdOf(raw, false))
          // No content size, so the frame keeps its declared 256 MiB window
          // rather than one shrunk to fit the data.
          && writeFile("imgtest-long.img.zst", zstdOf(raw, false, false, 28))
          && writeFile("imgtest-padded.img.bz2", bz + QByteArray(512, '\0'))
          && writeFile("imgtest-padded.img.zst", zst + QByteArray(512, '\0'))
          && writeFile("imgtest-trunc.img.bz2", bz.left(bz.size() / 2))
          && writeFile("imgtest-trunc.img.zst", zst.left(zst.size() / 2))
          && writeFile("imgtest-zst-named.img", zst)
          && writeFile("imgtest-blocks.img.xz", xzb)
          && writeFile("imgtest-blocks-trunc.img.xz", xzb.left(xzb.size() / 2)),
          "every fixture file was written");
    printf("\n");

    caseRoundTrip("raw", "imgtest.img", raw);
    caseRoundTrip("gzip", "imgtest.img.gz", raw);
    caseRoundTrip("xz", "imgtest.img.xz", raw);
    caseRoundTrip("gzip, two members", "imgtest-multi.img.gz", raw);
    caseRoundTrip("xz, two streams", "imgtest-multi.img.xz", raw);
    caseRoundTrip("xz, two streams with padding between", "imgtest-padded.img.xz", raw);
    caseRoundTrip("xz, many blocks (decoded in parallel)", "imgtest-blocks.img.xz", raw);
    caseRoundTrip("bzip2", "imgtest.img.bz2", raw);
    caseRoundTrip("zstd", "imgtest.img.zst", raw);
    // pbzip2/lbzip2 write one stream per chunk; stopping after the first is
    // the multi-member gzip bug over again.
    caseRoundTrip("bzip2, two streams", "imgtest-multi.img.bz2", raw);
    caseRoundTrip("zstd, two frames", "imgtest-multi.img.zst", raw);
    caseRoundTrip("zstd, skippable frames between data frames (pzstd)", "imgtest-pzstd.img.zst", raw);
    caseRoundTrip("zstd, no content size in the frame", "imgtest-nosize.img.zst", raw);
    caseRoundTrip("zstd with a 256 MiB window (zstd --long)", "imgtest-long.img.zst", raw);
    // Zeros after the stream, as a block-oriented writer leaves: the image
    // ends with the stream, as for gzip.
    caseRoundTrip("bzip2 followed by zero padding", "imgtest-padded.img.bz2", raw);
    caseRoundTrip("zstd followed by zero padding", "imgtest-padded.img.zst", raw);
    {
        // The first frame ends two bytes short of the 1 MiB input buffer, so
        // the next frame's 4-byte magic is split across two reads.
        const int INPUT = 1024 * 1024;
        QByteArray head = noise(INPUT - 64, 7);
        QByteArray f1 = zstdOf(head);
        for (int i = 0; i < 4 && !f1.isEmpty() && f1.size() != INPUT - 2; ++i)
        {
            head = noise(head.size() + (INPUT - 2 - f1.size()), 7);
            f1 = zstdOf(head);
        }
        check(f1.size() == INPUT - 2, "fixture: first zstd frame ends 2 bytes short of the input buffer");
        const QByteArray tail = pattern(200000);
        const QByteArray split = head + tail;
        check(writeFile("imgtest-split.img.zst", f1 + zstdOf(tail)), "fixture: split-header zstd written");
        caseRoundTrip("zstd, next frame's header split across the input buffer",
                      "imgtest-split.img.zst", split);
        DeleteFileA("imgtest-split.img.zst");
    }

    // Truncation must be reported, or half an image is written and called done.
    caseRejected("gzip that stops in the middle", "imgtest-trunc.img.gz", raw);
    caseRejected("xz that stops in the middle", "imgtest-trunc.img.xz", raw);
    caseRejected("xz of many blocks that stops in the middle", "imgtest-blocks-trunc.img.xz", raw);
    caseRejected("bzip2 that stops in the middle", "imgtest-trunc.img.bz2", raw);
    caseRejected("zstd that stops in the middle", "imgtest-trunc.img.zst", raw);

    caseMisnamed("gzip named .img", "imgtest-gz-named.img", raw, true);
    caseMisnamed("raw named .img.gz", "imgtest-raw-named.img.gz", raw, false);
    caseMisnamed("zstd named .img", "imgtest-zst-named.img", raw, true);
    {
        // "BZh9" is four bytes a raw image could start with; without the block
        // magic after it, it must stay raw.
        QByteArray bzlike = raw;
        bzlike.replace(0, 4, "BZh9");
        writeFile("imgtest-bzlike.img", bzlike);
        caseMisnamed("raw that starts with \"BZh9\"", "imgtest-bzlike.img", bzlike, false);
        DeleteFileA("imgtest-bzlike.img");
    }

    caseSize("raw", "imgtest.img", raw);
    caseSize("gzip", "imgtest.img.gz", raw);
    caseSize("xz", "imgtest.img.xz", raw);
    // Its trailer records the last member's size only.
    caseSize("gzip, two members", "imgtest-multi.img.gz", raw);
    caseSize("xz, two streams", "imgtest-multi.img.xz", raw);
    caseSize("xz, two streams with padding between", "imgtest-padded.img.xz", raw);
    caseSize("xz, many blocks", "imgtest-blocks.img.xz", raw);
    caseSize("bzip2", "imgtest.img.bz2", raw);
    caseSize("zstd", "imgtest.img.zst", raw);
    // The first frame's size alone, which must not pass for the whole image's.
    caseSize("zstd, two frames", "imgtest-multi.img.zst", raw);
    caseSize("zstd, skippable frames between data frames (pzstd)", "imgtest-pzstd.img.zst", raw);
    caseSize("zstd, no content size in the frame", "imgtest-nosize.img.zst", raw);
    {
        printf("size estimates\n");
        ImageSource src;
        check(src.open("imgtest.img.zst", SS) && !src.sizeKnown()
                  && src.sizeInSectors() == sectorsOf(raw),
              "zstd: one frame's declared size is the estimate, not claimed exact");
        src.close();
        check(src.open("imgtest-pzstd.img.zst", SS) && !src.sizeKnown()
                  && src.sizeInSectors() == sectorsOf(raw.left(cut)),
              "zstd: a leading skippable frame is stepped over to the first data frame");
        src.close();
        check(src.open("imgtest.img.bz2", SS) && !src.sizeKnown() && src.sizeInSectors() == 0,
              "bzip2: no size and no estimate");
        src.close();
        printf("\n");
    }
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
    caseSeek("xz, many blocks", "imgtest-blocks.img.xz", raw, true);
    caseSeek("bzip2", "imgtest.img.bz2", raw, true);
    caseSeek("zstd", "imgtest.img.zst", raw, true);

    casePrefetch("raw", "imgtest.img", raw, 8192, false);
    casePrefetch("raw", "imgtest.img", raw, 7, false);
    casePrefetch("gzip", "imgtest.img.gz", raw, 8192, false);
    casePrefetch("xz, many blocks", "imgtest-blocks.img.xz", raw, 8192, false);
    casePrefetch("bzip2, two streams", "imgtest-multi.img.bz2", raw, 1000, false);
    casePrefetch("zstd", "imgtest.img.zst", raw, 8192, false);
    casePrefetch("gzip that stops in the middle", "imgtest-trunc.img.gz", raw, 8192, true);
    casePrefetch("xz of many blocks that stops in the middle", "imgtest-blocks-trunc.img.xz", raw, 100, true);
    casePrefetchStop("imgtest.img.zst");

    caseEndProbe("raw", "imgtest.img", raw, false);
    caseEndProbe("gzip", "imgtest.img.gz", raw, false);
    caseEndProbe("xz", "imgtest.img.xz", raw, false);
    caseEndProbe("xz, many blocks", "imgtest-blocks.img.xz", raw, false);
    {
        // A block's check fails in the middle of the stream, while other
        // blocks are being decoded alongside it.
        QByteArray bad = xzb;
        bad[bad.size() / 3] = (char)(bad.at(bad.size() / 3) ^ 0x55);
        writeFile("imgtest-blocks-bad.img.xz", bad);
        QByteArray got;
        QString why;
        printf("xz of many blocks, one damaged\n");
        check(!readBack("imgtest-blocks-bad.img.xz", &got, &why), "reported as a failure");
        check(!why.isEmpty() && got.size() < raw.size(), "with a reason, before the whole image");
        printf("  -> %s\n\n", why.toLocal8Bit().constData());
        DeleteFileA("imgtest-blocks-bad.img.xz");
    }
    caseEndProbe("bzip2", "imgtest.img.bz2", raw, false);
    caseEndProbe("zstd", "imgtest.img.zst", raw, false);
    {
        // bzip2's whole-stream CRC is in its last bytes. It is not byte
        // aligned, but the final byte always holds at least one bit of it.
        QByteArray badcrc = bz;
        badcrc[badcrc.size() - 1] = (char)(badcrc.at(badcrc.size() - 1) ^ 0xFF);
        check(writeFile("imgtest-badcrc.img.bz2", badcrc), "fixture: bzip2 with a bad CRC written");
        caseEndProbe("bzip2 with a bad CRC", "imgtest-badcrc.img.bz2", raw, true);
        DeleteFileA("imgtest-badcrc.img.bz2");

        // The content checksum the zstd tool adds by default is the frame's
        // last four bytes.
        QByteArray badsum = zstdOf(raw, true, true);
        badsum[badsum.size() - 1] = (char)(badsum.at(badsum.size() - 1) ^ 0xFF);
        check(writeFile("imgtest-badsum.img.zst", badsum), "fixture: zstd with a bad checksum written");
        caseEndProbe("zstd with a bad checksum", "imgtest-badsum.img.zst", raw, true);
        DeleteFileA("imgtest-badsum.img.zst");
    }
    {
        // The gzip CRC-32 is the trailer's first four bytes.
        QByteArray badcrc = gz;
        badcrc[badcrc.size() - 8] = (char)(badcrc.at(badcrc.size() - 8) ^ 0xFF);
        check(writeFile("imgtest-badcrc.img.gz", badcrc), "fixture: gzip with a bad CRC written");
        caseEndProbe("gzip with a bad CRC", "imgtest-badcrc.img.gz", raw, true);
        DeleteFileA("imgtest-badcrc.img.gz");
    }

    caseLongPath(raw);

    caseSinkRoundTrip("ImageSink, gzip", "imgtest-sink.img.gz", ImageSink::FORMAT_GZIP, raw);
    caseSinkRoundTrip("ImageSink, xz", "imgtest-sink.img.xz", ImageSink::FORMAT_XZ, raw);
    caseSinkRoundTrip("ImageSink, bzip2", "imgtest-sink.img.bz2", ImageSink::FORMAT_BZIP2, raw);
    caseSinkRoundTrip("ImageSink, zstd", "imgtest-sink.img.zst", ImageSink::FORMAT_ZSTD, raw);
    caseSinkWriter("SinkWriter, gzip", "imgtest-sink.img.gz", ImageSink::FORMAT_GZIP, raw);
    caseSinkWriter("SinkWriter, xz", "imgtest-sink.img.xz", ImageSink::FORMAT_XZ, raw);
    caseSinkWriter("SinkWriter, bzip2", "imgtest-sink.img.bz2", ImageSink::FORMAT_BZIP2, raw);
    caseSinkWriter("SinkWriter, zstd", "imgtest-sink.img.zst", ImageSink::FORMAT_ZSTD, raw);
    caseSinkAbort("ImageSink, gzip aborted", "imgtest-abort.img.gz", ImageSink::FORMAT_GZIP, raw);
    caseSinkAbort("ImageSink, xz aborted", "imgtest-abort.img.xz", ImageSink::FORMAT_XZ, raw);
    caseSinkAbort("ImageSink, bzip2 aborted", "imgtest-abort.img.bz2", ImageSink::FORMAT_BZIP2, raw);
    caseSinkAbort("ImageSink, zstd aborted", "imgtest-abort.img.zst", ImageSink::FORMAT_ZSTD, raw);
    {
        // The zstd sink adds the content checksum the zstd tool does, so a
        // damaged image it wrote is caught on the way back in.
        printf("ImageSink, zstd: checksummed\n");
        QFile f("imgtest-sink.img.zst");
        QByteArray made;
        if (f.open(QIODevice::ReadOnly))
        {
            made = f.readAll();
            f.close();
        }
        check(made.size() > 4 && (made.at(4) & 0x04) != 0, "the frame header's checksum flag is set");
        printf("\n");
        if (!made.isEmpty())
        {
            made[made.size() - 1] = (char)(made.at(made.size() - 1) ^ 0xFF);
            writeFile("imgtest-sink-badsum.img.zst", made);
            caseEndProbe("ImageSink, zstd, last checksum byte changed",
                         "imgtest-sink-badsum.img.zst", raw, true);
            DeleteFileA("imgtest-sink-badsum.img.zst");
        }
    }

    const char *leftovers[] = {
        "imgtest.img", "imgtest.img.gz", "imgtest.img.xz",
        "imgtest-multi.img.gz", "imgtest-multi.img.xz", "imgtest-padded.img.xz",
        "imgtest-trunc.img.gz", "imgtest-trunc.img.xz",
        "imgtest-sink.img.gz", "imgtest-sink.img.xz", "imgtest-sink.img.bz2", "imgtest-sink.img.zst",
        "imgtest-abort.img.gz", "imgtest-abort.img.xz", "imgtest-abort.img.bz2", "imgtest-abort.img.zst",
        "imgtest-gz-named.img", "imgtest-raw-named.img.gz",
        "imgtest.img.bz2", "imgtest.img.zst", "imgtest-multi.img.bz2", "imgtest-multi.img.zst",
        "imgtest-pzstd.img.zst", "imgtest-nosize.img.zst", "imgtest-long.img.zst",
        "imgtest-blocks.img.xz", "imgtest-blocks-trunc.img.xz",
        "imgtest-padded.img.bz2", "imgtest-padded.img.zst",
        "imgtest-trunc.img.bz2", "imgtest-trunc.img.zst", "imgtest-zst-named.img",
    };
    for (size_t i = 0; i < sizeof(leftovers) / sizeof(leftovers[0]); ++i)
    {
        DeleteFileA(leftovers[i]);
    }

    printf("%d checks, %d failures\n", checks, failures);
    return failures ? 1 : 0;
}
