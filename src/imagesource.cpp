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

#include "imagesource.h"

#include <QObject>
#include <QFileInfo>
#include <cstring>
#include <zlib.h>
#include <lzma.h>
#include <bzlib.h>
#include <zstd.h>

// Compressed bytes read per ReadFile: large enough that syscalls do not limit
// throughput.
static const unsigned long INPUT_CHUNK = 1024ul * 1024ul;

// Bytes as whole sectors, rounding up.
static inline unsigned long long sectorsFor(unsigned long long bytes,
                                            unsigned long long sectorsize)
{
    return (bytes / sectorsize) + ((bytes % sectorsize) ? 1ull : 0ull);
}


ImageSource::ImageSource()
    : myHandle(INVALID_HANDLE_VALUE), myFormat(FORMAT_RAW), mySectorSize(0ull),
      mySectors(0ull), myCompressedSize(0ull), myPos(0ull), mySizeKnown(false),
      myEof(false), myFinishing(false), myDecoder(NULL), myNextIn(NULL), myAvailIn(0ull)
{
}

ImageSource::~ImageSource()
{
    close();
}

QString ImageSource::formatName(Format f)
{
    switch (f)
    {
        case FORMAT_GZIP: return QString("gzip");
        case FORMAT_XZ:   return QString("xz");
        case FORMAT_BZIP2: return QString("bzip2");
        case FORMAT_ZSTD: return QString("zstd");
        default:          return QString("raw");
    }
}

void ImageSource::close()
{
    if (myDecoder != NULL)
    {
        if (myFormat == FORMAT_GZIP)
        {
            inflateEnd((z_stream *)myDecoder);
            delete (z_stream *)myDecoder;
        }
        else if (myFormat == FORMAT_XZ)
        {
            lzma_end((lzma_stream *)myDecoder);
            delete (lzma_stream *)myDecoder;
        }
        else if (myFormat == FORMAT_BZIP2)
        {
            BZ2_bzDecompressEnd((bz_stream *)myDecoder);
            delete (bz_stream *)myDecoder;
        }
        else if (myFormat == FORMAT_ZSTD)
        {
            ZSTD_freeDStream((ZSTD_DStream *)myDecoder);
        }
        myDecoder = NULL;
    }
    if (myHandle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(myHandle);
        myHandle = INVALID_HANDLE_VALUE;
    }
    myInput.clear();
    myNextIn = NULL;
    myAvailIn = 0ull;
    myFormat = FORMAT_RAW;
    mySectorSize = 0ull;
    mySectors = 0ull;
    myCompressedSize = 0ull;
    myPos = 0ull;
    mySizeKnown = false;
    myEof = false;
    myFinishing = false;
}

bool ImageSource::open(const QString &path, unsigned long long sectorsize)
{
    close();
    myError.clear();
    if (!sectorsize)
    {
        myError = QObject::tr("The device reports a sector size of zero.");
        return false;
    }
    mySectorSize = sectorsize;

    myHandle = CreateFileW((LPCWSTR)path.utf16(), GENERIC_READ, FILE_SHARE_READ,
                           NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if (myHandle == INVALID_HANDLE_VALUE)
    {
        myError = QObject::tr("The image file could not be opened (error %1).")
                      .arg(GetLastError());
        return false;
    }

    LARGE_INTEGER filesize;
    if (!GetFileSizeEx(myHandle, &filesize))
    {
        myError = QObject::tr("The size of the image file could not be read (error %1).")
                      .arg(GetLastError());
        close();
        return false;
    }
    myCompressedSize = (unsigned long long)filesize.QuadPart;

    // Decide by content, not by name: a renamed image should still work, and a
    // raw image called .img.gz should not be fed to a decompressor.
    unsigned char magic[10];
    DWORD magicread = 0;
    if (!ReadFile(myHandle, magic, sizeof(magic), &magicread, NULL))
    {
        myError = QObject::tr("The image file could not be read (error %1).")
                      .arg(GetLastError());
        close();
        return false;
    }
    if (magicread >= 2 && magic[0] == 0x1f && magic[1] == 0x8b)
    {
        myFormat = FORMAT_GZIP;
    }
    else if (magicread >= 6 && memcmp(magic, "\xfd" "7zXZ\x00", 6) == 0)
    {
        myFormat = FORMAT_XZ;
    }
    else if (magicread >= 10 && isBzip2Header(magic)
             && (memcmp(magic + 4, "\x31\x41\x59\x26\x53\x59", 6) == 0     // first block
                 || memcmp(magic + 4, "\x17\x72\x45\x38\x50\x90", 6) == 0))  // empty stream
    {
        // "BZh1" alone is four bytes a raw image could start with; the block
        // magic after it is not.
        myFormat = FORMAT_BZIP2;
    }
    else if (magicread >= 4 && isZstdFrame(magic))
    {
        myFormat = FORMAT_ZSTD;
    }
    else
    {
        myFormat = FORMAT_RAW;
    }

    if (myFormat == FORMAT_RAW)
    {
        mySectors = sectorsFor(myCompressedSize, mySectorSize);
        mySizeKnown = true;
        // No rewind: raw read() seeks before every read.
        return true;
    }

    // False is recoverable, a set myError is not; see imagesource.h. bzip2
    // records no size anywhere, so it has not even an estimate.
    bool gotsize = false;
    switch (myFormat)
    {
        case FORMAT_GZIP: gotsize = readGzipSize(myCompressedSize); break;
        case FORMAT_XZ:   gotsize = readXzSize(myCompressedSize); break;
        case FORMAT_ZSTD: gotsize = readZstdSize(myCompressedSize); break;
        default:          break;
    }
    if (!myError.isEmpty())
    {
        close();
        return false;
    }
    mySizeKnown = gotsize;

    LARGE_INTEGER zero;
    zero.QuadPart = 0;
    if (!SetFilePointerEx(myHandle, zero, NULL, FILE_BEGIN))
    {
        myError = QObject::tr("The image file could not be rewound (error %1).")
                      .arg(GetLastError());
        close();
        return false;
    }
    if (!initDecoder())
    {
        close();
        return false;
    }
    myInput.resize(INPUT_CHUNK);
    return true;
}

// Callers only pass ranges already checked to lie inside the file, so any
// failure here is real I/O trouble and sets myError.
bool ImageSource::readAt(unsigned long long offset, void *buf, DWORD len)
{
    LARGE_INTEGER pos;
    pos.QuadPart = (LONGLONG)offset;
    if (!SetFilePointerEx(myHandle, pos, NULL, FILE_BEGIN))
    {
        myError = QObject::tr("The image file could not be read (error %1).")
                      .arg(GetLastError());
        return false;
    }
    DWORD got = 0;
    if (!ReadFile(myHandle, buf, len, &got, NULL) || got != len)
    {
        myError = QObject::tr("The image file could not be read (error %1).")
                      .arg(GetLastError());
        return false;
    }
    return true;
}

// gzip's trailing ISIZE is the uncompressed size mod 4 GiB, and of the last
// member only: a 6 GiB image records 2 GiB, and two concatenated members
// record just the second. Neither case can be told apart from the real size
// without decompressing the whole file, so the size is never reported exact
// and the write runs to the end of the stream. ISIZE is kept only as a
// progress estimate.
//
// Always returns false; mySectors is set whenever the value is usable as an
// estimate.
bool ImageSource::readGzipSize(unsigned long long filesize)
{
    if (filesize < 18ull)
    {
        // Too small to hold a header and a trailer.
        return false;
    }
    unsigned char isize[4];
    if (!readAt(filesize - 4ull, isize, 4))
    {
        return false;
    }
    unsigned long long size = (unsigned long long)isize[0] |
                              ((unsigned long long)isize[1] << 8) |
                              ((unsigned long long)isize[2] << 16) |
                              ((unsigned long long)isize[3] << 24);
    if (size < filesize)
    {
        // Smaller than the compressed file: wrapped an unknown number of times,
        // or incompressible data. Either way, not even an estimate.
        return false;
    }
    mySectors = sectorsFor(size, mySectorSize);
    return false;
}

// xz indexes every block, so the size is exact when the index can be read.
// Walks the file backwards stream by stream (footer, index, previous stream),
// as xz --list does. liblzma's lzma_file_info_decoder would do this but is
// not in every version we build against.
bool ImageSource::readXzSize(unsigned long long filesize)
{
    unsigned long long pos = filesize;
    unsigned long long total = 0ull;
    std::vector<unsigned char> buf;
    std::vector<unsigned char> padbuf(65536);

    while (pos > 0ull)
    {
        // Streams may be separated by padding: whole groups of four zero
        // bytes. Scanned back a block at a time; one read per group would
        // stall open() on a file with megabytes of padding.
        bool padding = true;
        while (padding && pos >= 4ull)
        {
            const unsigned long long n = qMin(pos, (unsigned long long)padbuf.size()) & ~3ull;
            if (!readAt(pos - n, &padbuf[0], (DWORD)n))
            {
                return false;
            }
            unsigned long long zeros = 0ull;
            while (zeros < n)
            {
                const unsigned char *g = &padbuf[(size_t)(n - zeros - 4ull)];
                if (g[0] || g[1] || g[2] || g[3])
                {
                    padding = false;
                    break;
                }
                zeros += 4ull;
            }
            pos -= zeros;
        }
        if (pos == 0ull)
        {
            break;
        }
        if (pos < LZMA_STREAM_HEADER_SIZE)
        {
            return false;
        }

        unsigned char footer[LZMA_STREAM_HEADER_SIZE];
        if (!readAt(pos - LZMA_STREAM_HEADER_SIZE, footer, sizeof(footer)))
        {
            return false;
        }
        lzma_stream_flags flags;
        if (lzma_stream_footer_decode(&flags, footer) != LZMA_OK)
        {
            return false;
        }
        if (flags.backward_size > pos - LZMA_STREAM_HEADER_SIZE ||
            flags.backward_size > (1ull << 26))   // 64 MiB sanity cap on the index
        {
            return false;
        }

        buf.resize((size_t)flags.backward_size);
        if (!readAt(pos - LZMA_STREAM_HEADER_SIZE - flags.backward_size,
                    &buf[0], (DWORD)flags.backward_size))
        {
            return false;
        }

        lzma_index *index = NULL;
        // The 64 MiB cap bounds the index's bytes, not the tree decoded from
        // them, which a crafted index of tiny records can make many times
        // larger. Past this, the size is just reported unknown.
        uint64_t memlimit = 256ull * 1024ull * 1024ull;
        size_t inpos = 0;
        if (lzma_index_buffer_decode(&index, &memlimit, NULL, &buf[0], &inpos,
                                     buf.size()) != LZMA_OK)
        {
            if (index != NULL)
            {
                lzma_index_end(index, NULL);
            }
            return false;
        }
        const unsigned long long add = lzma_index_uncompressed_size(index);
        if (add > ~0ull - total)
        {
            // A wrapped total would be reported as an exact, and wrong, size.
            lzma_index_end(index, NULL);
            return false;
        }
        total += add;
        unsigned long long streamsize = lzma_index_stream_size(index);
        lzma_index_end(index, NULL);
        if (streamsize > pos)
        {
            return false;
        }
        pos -= streamsize;
    }

    if (total == 0ull)
    {
        return false;
    }
    mySectors = sectorsFor(total, mySectorSize);
    return true;
}

// zstd frames carry their own content size, but the first frame's is all that
// can be read without decompressing: the format has no index or footer, and
// pzstd, or zstd reading from a pipe, writes one frame after another (or none
// at all). So like gzip the size is never exact, and it is a lower bound when
// the first frame declares one. A skippable frame ahead of it, as pzstd
// writes, is stepped over.
//
// Always returns false; mySectors is set whenever the value is usable.
bool ImageSource::readZstdSize(unsigned long long filesize)
{
    unsigned long long pos = 0ull;
    for (int skipped = 0; skipped < 16; ++skipped)
    {
        unsigned char head[18];                 // ZSTD_FRAMEHEADERSIZE_MAX
        const DWORD n = (DWORD)qMin(filesize - pos, (unsigned long long)sizeof(head));
        if (n < 8 || !readAt(pos, head, n))
        {
            return false;
        }
        if ((head[0] & 0xf0) == 0x50 && head[1] == 0x2a && head[2] == 0x4d && head[3] == 0x18)
        {
            const unsigned long long len = (unsigned long long)head[4] |
                                           ((unsigned long long)head[5] << 8) |
                                           ((unsigned long long)head[6] << 16) |
                                           ((unsigned long long)head[7] << 24);
            if (len > filesize - pos - 8ull)
            {
                return false;
            }
            pos += 8ull + len;
            continue;
        }
        const unsigned long long size = ZSTD_getFrameContentSize(head, n);
        if (size != ZSTD_CONTENTSIZE_UNKNOWN && size != ZSTD_CONTENTSIZE_ERROR)
        {
            mySectors = sectorsFor(size, mySectorSize);
        }
        return false;
    }
    return false;
}

bool ImageSource::isBzip2Header(const unsigned char *p)
{
    return p[0] == 'B' && p[1] == 'Z' && p[2] == 'h' && p[3] >= '1' && p[3] <= '9';
}

// A zstd frame, or a skippable frame (magic 0x184D2A50..5F).
bool ImageSource::isZstdFrame(const unsigned char *p)
{
    return (p[0] == 0x28 && p[1] == 0xb5 && p[2] == 0x2f && p[3] == 0xfd)
        || ((p[0] & 0xf0) == 0x50 && p[1] == 0x2a && p[2] == 0x4d && p[3] == 0x18);
}

bool ImageSource::initDecoder()
{
    if (myFormat == FORMAT_BZIP2)
    {
        bz_stream *bs = new bz_stream;
        memset(bs, 0, sizeof(*bs));
        int ret = BZ2_bzDecompressInit(bs, 0, 0);
        if (ret != BZ_OK)
        {
            delete bs;
            myError = QObject::tr("The bzip2 decompressor could not be started "
                                  "(bzip2 error %1).").arg(ret);
            return false;
        }
        myDecoder = bs;
        return true;
    }
    if (myFormat == FORMAT_ZSTD)
    {
        ZSTD_DStream *ds = ZSTD_createDStream();
        if (ds == NULL)
        {
            myError = QObject::tr("The zstd decompressor could not be started "
                                  "(zstd error %1).").arg((int)ZSTD_error_memory_allocation);
            return false;
        }
        // Past zstd's default 128 MiB window, so an image made with --long
        // (up to 2 GiB) still decodes. The memory is only taken if a frame
        // asks for it.
        size_t ret = ZSTD_DCtx_setParameter(ds, ZSTD_d_windowLogMax, 31);
        if (ZSTD_isError(ret))
        {
            ZSTD_freeDStream(ds);
            myError = QObject::tr("The zstd decompressor could not be started "
                                  "(zstd error %1).").arg((int)ZSTD_getErrorCode(ret));
            return false;
        }
        myDecoder = ds;
        return true;
    }

    if (myFormat == FORMAT_GZIP)
    {
        z_stream *zs = new z_stream;
        memset(zs, 0, sizeof(*zs));
        // 15 + 16: gzip wrapper, the only one .img.gz uses.
        int ret = inflateInit2(zs, 15 + 16);
        if (ret != Z_OK)
        {
            delete zs;
            myError = QObject::tr("The gzip decompressor could not be started "
                                  "(zlib error %1).").arg(ret);
            return false;
        }
        myDecoder = zs;
        return true;
    }

    lzma_stream *ls = new lzma_stream;
    memset(ls, 0, sizeof(*ls));
    // LZMA_CONCATENATED: xz files may be several streams appended together.
    lzma_ret ret = lzma_stream_decoder(ls, UINT64_MAX, LZMA_CONCATENATED);
    if (ret != LZMA_OK)
    {
        delete ls;
        myError = QObject::tr("The xz decompressor could not be started "
                              "(lzma error %1).").arg((int)ret);
        return false;
    }
    myDecoder = ls;
    return true;
}

// Reads into the input buffer after the first `kept` bytes, which the caller
// has already placed at its front. Sets myError on failure.
bool ImageSource::refillInput(size_t kept, DWORD *got)
{
    *got = 0;
    if (!ReadFile(myHandle, &myInput[kept], (DWORD)(myInput.size() - kept), got, NULL))
    {
        myError = QObject::tr("The image file could not be read (error %1).")
                      .arg(GetLastError());
        return false;
    }
    return true;
}

// Sets *follows when the bytes after the stream that just ended start another
// one -- a gzip member, a bzip2 stream, a zstd frame -- refilling the input if
// needed. Returns false only on a read error; running out of file means none
// follows.
bool ImageSource::nextMemberFollows(bool *follows)
{
    *follows = false;
    const unsigned long long need = (myFormat == FORMAT_GZIP) ? 2ull : 4ull;
    if (myAvailIn < need)
    {
        // Keep the leftover bytes: they may be the start of the next header.
        size_t kept = (size_t)myAvailIn;
        if (kept > 0)
        {
            memmove(&myInput[0], myNextIn, kept);
        }
        DWORD got = 0;
        if (!refillInput(kept, &got))
        {
            return false;
        }
        myNextIn = &myInput[0];
        myAvailIn = (unsigned long long)kept + got;
    }
    if (myAvailIn >= need)
    {
        switch (myFormat)
        {
            case FORMAT_GZIP:  *follows = (myNextIn[0] == 0x1f && myNextIn[1] == 0x8b); break;
            case FORMAT_BZIP2: *follows = isBzip2Header(myNextIn); break;
            case FORMAT_ZSTD:  *follows = isZstdFrame(myNextIn); break;
            default:           break;
        }
    }
    return true;
}

bool ImageSource::fill(char *buf, unsigned long long len, unsigned long long *produced)
{
    *produced = 0ull;
    while (*produced < len && !myEof)
    {
        if (myAvailIn == 0ull && !myFinishing)
        {
            DWORD got = 0;
            if (!refillInput(0, &got))
            {
                return false;
            }
            if (got == 0)
            {
                // End of file, with the stream not yet ended. gzip signals its
                // own end, so this means it is truncated. xz in
                // LZMA_CONCATENATED mode must be told via LZMA_FINISH before it
                // reports the end; bzip2 and zstd may still hold decoded output
                // to hand over before the cut shows.
                if (myFormat == FORMAT_GZIP)
                {
                    myError = QObject::tr("The image file ends in the middle of the "
                                          "compressed data. It is truncated or damaged.");
                    return false;
                }
                myFinishing = true;
            }
            else
            {
                myNextIn = &myInput[0];
                myAvailIn = got;
            }
        }

        if (myFormat == FORMAT_GZIP)
        {
            z_stream *zs = (z_stream *)myDecoder;
            zs->next_in = myNextIn;
            zs->avail_in = (uInt)((myAvailIn > 0xffffffffull) ? 0xffffffffull : myAvailIn);
            zs->next_out = (Bytef *)(buf + *produced);
            zs->avail_out = (uInt)((len - *produced > 0xffffffffull) ? 0xffffffffull
                                                                    : (len - *produced));
            uInt availin = zs->avail_in;
            uInt availout = zs->avail_out;
            int ret = inflate(zs, Z_NO_FLUSH);
            *produced += availout - zs->avail_out;
            myAvailIn -= availin - zs->avail_in;
            myNextIn = (unsigned char *)zs->next_in;
            if (ret == Z_STREAM_END)
            {
                // Another member may follow, or trailing padding (e.g. zeros
                // from a block-oriented writer). Decoding padding would call a
                // fully written image damaged, so the image ends here unless a
                // real member header follows.
                bool another = false;
                if (!nextMemberFollows(&another))
                {
                    return false;
                }
                if (!another)
                {
                    myEof = true;
                }
                else if (inflateReset(zs) != Z_OK)
                {
                    myError = QObject::tr("The gzip image could not be decompressed.");
                    return false;
                }
            }
            else if (ret != Z_OK && ret != Z_BUF_ERROR)
            {
                myError = QObject::tr("The gzip image is damaged (zlib error %1).").arg(ret);
                return false;
            }
        }
        else if (myFormat == FORMAT_BZIP2)
        {
            bz_stream *bs = (bz_stream *)myDecoder;
            bs->next_in = (char *)myNextIn;
            bs->avail_in = (unsigned int)((myAvailIn > 0xffffffffull) ? 0xffffffffull : myAvailIn);
            bs->next_out = buf + *produced;
            bs->avail_out = (unsigned int)((len - *produced > 0xffffffffull) ? 0xffffffffull
                                                                             : (len - *produced));
            const unsigned int availin = bs->avail_in;
            const unsigned int availout = bs->avail_out;
            int ret = BZ2_bzDecompress(bs);
            const unsigned int madeout = availout - bs->avail_out;
            *produced += madeout;
            myAvailIn -= availin - bs->avail_in;
            myNextIn = (unsigned char *)bs->next_in;
            if (ret == BZ_STREAM_END)
            {
                // pbzip2 and lbzip2 write one stream per chunk of the image,
                // so another may follow; anything else ends the image, as for
                // gzip.
                bool another = false;
                if (!nextMemberFollows(&another))
                {
                    return false;
                }
                if (!another)
                {
                    myEof = true;
                }
                else
                {
                    BZ2_bzDecompressEnd(bs);
                    memset(bs, 0, sizeof(*bs));
                    if (BZ2_bzDecompressInit(bs, 0, 0) != BZ_OK)
                    {
                        // Freed by close(), which cannot tell it never started.
                        delete bs;
                        myDecoder = NULL;
                        myError = QObject::tr("The bzip2 image could not be decompressed.");
                        return false;
                    }
                }
            }
            else if (ret != BZ_OK)
            {
                myError = QObject::tr("The bzip2 image is damaged (bzip2 error %1).").arg(ret);
                return false;
            }
            else if (myFinishing && madeout == 0)
            {
                myError = QObject::tr("The image file ends in the middle of the "
                                      "compressed data. It is truncated or damaged.");
                return false;
            }
        }
        else if (myFormat == FORMAT_ZSTD)
        {
            ZSTD_DStream *ds = (ZSTD_DStream *)myDecoder;
            ZSTD_inBuffer in = { myNextIn, (size_t)myAvailIn, 0 };
            ZSTD_outBuffer out = { buf + *produced, (size_t)(len - *produced), 0 };
            const size_t ret = ZSTD_decompressStream(ds, &out, &in);
            *produced += out.pos;
            myNextIn += in.pos;
            myAvailIn -= in.pos;
            if (ZSTD_isError(ret))
            {
                myError = QObject::tr("The zstd image is damaged (zstd error %1).")
                              .arg((int)ZSTD_getErrorCode(ret));
                return false;
            }
            if (ret == 0)
            {
                // A frame is complete and all of it handed over. The decoder
                // starts on the next frame by itself, so the only question is
                // whether one follows or the image ends here.
                bool another = false;
                if (!nextMemberFollows(&another))
                {
                    return false;
                }
                if (!another)
                {
                    myEof = true;
                }
            }
            else if (myFinishing && out.pos == 0)
            {
                myError = QObject::tr("The image file ends in the middle of the "
                                      "compressed data. It is truncated or damaged.");
                return false;
            }
        }
        else
        {
            lzma_stream *ls = (lzma_stream *)myDecoder;
            ls->next_in = myNextIn;
            ls->avail_in = (size_t)myAvailIn;
            ls->next_out = (uint8_t *)(buf + *produced);
            ls->avail_out = (size_t)(len - *produced);
            size_t availout = ls->avail_out;
            lzma_ret ret = lzma_code(ls, myFinishing ? LZMA_FINISH : LZMA_RUN);
            size_t madeout = availout - ls->avail_out;
            *produced += madeout;
            myAvailIn = ls->avail_in;
            myNextIn = (unsigned char *)ls->next_in;
            if (ret == LZMA_STREAM_END)
            {
                myEof = true;
            }
            else if (ret != LZMA_OK)
            {
                myError = QObject::tr("The xz image is damaged (lzma error %1).")
                              .arg((int)ret);
                return false;
            }
            else if (myFinishing && madeout == 0)
            {
                // Out of input, still not at the end of the stream, and nothing
                // more coming out: the file is truncated.
                myError = QObject::tr("The image file ends in the middle of the "
                                      "compressed data. It is truncated or damaged.");
                return false;
            }
        }
    }
    return true;
}

bool ImageSource::skipTo(unsigned long long startsector)
{
    if (startsector < myPos)
    {
        myError = QObject::tr("A compressed image can only be read forwards.");
        return false;
    }
    std::vector<char> scrap;
    while (myPos < startsector && !myEof)
    {
        unsigned long long chunk = startsector - myPos;
        if (chunk > 1024ull)
        {
            chunk = 1024ull;
        }
        unsigned long long bytes = chunk * mySectorSize;
        if (scrap.size() < bytes)
        {
            scrap.resize((size_t)bytes);
        }
        unsigned long long produced = 0ull;
        if (!fill(&scrap[0], bytes, &produced))
        {
            return false;
        }
        myPos += sectorsFor(produced, mySectorSize);
    }
    return true;
}

// Compressed bytes per WriteFile; see INPUT_CHUNK.
static const size_t OUTPUT_CHUNK = 1024ul * 1024ul;

// zstd keeps the input position in a buffer the caller owns rather than in
// its context, so the sink holds the two together.
struct ZstdEncoder
{
    ZSTD_CCtx *cctx;
    ZSTD_inBuffer in;
};

ImageSink::ImageSink()
    : myHandle(INVALID_HANDLE_VALUE), myFormat(FORMAT_GZIP), myEncoder(NULL)
{
}

QString ImageSink::extension(Format format)
{
    switch (format)
    {
        case FORMAT_GZIP:  return QString(".gz");
        case FORMAT_XZ:    return QString(".xz");
        case FORMAT_BZIP2: return QString(".bz2");
        default:           return QString(".zst");
    }
}

QString ImageSink::readTargetName(const QString &typed, bool compressed, Format format)
{
    const QString ext = compressed ? extension(format) : QString();
    const QString want = ".img" + ext;
    if (typed.endsWith(want, Qt::CaseInsensitive))
    {
        return typed;
    }
    if (compressed && typed.endsWith(".img", Qt::CaseInsensitive))
    {
        return typed + ext;
    }
    return typed + want;
}

ImageSink::~ImageSink()
{
    abort();
}

void ImageSink::abort()
{
    if (myEncoder != NULL)
    {
        switch (myFormat)
        {
            case FORMAT_GZIP:
                deflateEnd((z_stream *)myEncoder);
                delete (z_stream *)myEncoder;
                break;
            case FORMAT_XZ:
                lzma_end((lzma_stream *)myEncoder);
                delete (lzma_stream *)myEncoder;
                break;
            case FORMAT_BZIP2:
                BZ2_bzCompressEnd((bz_stream *)myEncoder);
                delete (bz_stream *)myEncoder;
                break;
            case FORMAT_ZSTD:
                ZSTD_freeCCtx(((ZstdEncoder *)myEncoder)->cctx);
                delete (ZstdEncoder *)myEncoder;
                break;
        }
        myEncoder = NULL;
    }
    if (myHandle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(myHandle);
        myHandle = INVALID_HANDLE_VALUE;
    }
    myOutput.clear();
}

bool ImageSink::open(const QString &path, Format format)
{
    abort();
    myError.clear();
    myFormat = format;

    myHandle = CreateFileW((LPCWSTR)path.utf16(), GENERIC_WRITE, 0, NULL,
                           CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (myHandle == INVALID_HANDLE_VALUE)
    {
        myError = QObject::tr("The image file could not be created (error %1).")
                      .arg(GetLastError());
        return false;
    }

    if (myFormat == FORMAT_GZIP)
    {
        z_stream *zs = new z_stream;
        memset(zs, 0, sizeof(*zs));
        // 15 + 16: gzip wrapper, matching what ImageSource expects to read back.
        int ret = deflateInit2(zs, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 15 + 16, 8,
                               Z_DEFAULT_STRATEGY);
        if (ret != Z_OK)
        {
            delete zs;
            myError = QObject::tr("The gzip compressor could not be started "
                                  "(zlib error %1).").arg(ret);
        }
        else
        {
            myEncoder = zs;
        }
    }
    else if (myFormat == FORMAT_XZ)
    {
        lzma_stream *ls = new lzma_stream;
        memset(ls, 0, sizeof(*ls));
        lzma_ret ret = lzma_easy_encoder(ls, LZMA_PRESET_DEFAULT, LZMA_CHECK_CRC64);
        if (ret != LZMA_OK)
        {
            delete ls;
            myError = QObject::tr("The xz compressor could not be started "
                                  "(lzma error %1).").arg((int)ret);
        }
        else
        {
            myEncoder = ls;
        }
    }
    else if (myFormat == FORMAT_BZIP2)
    {
        bz_stream *bs = new bz_stream;
        memset(bs, 0, sizeof(*bs));
        // 9: 900 KB blocks, what the bzip2 tool uses by default.
        int ret = BZ2_bzCompressInit(bs, 9, 0, 0);
        if (ret != BZ_OK)
        {
            delete bs;
            myError = QObject::tr("The bzip2 compressor could not be started "
                                  "(bzip2 error %1).").arg(ret);
        }
        else
        {
            myEncoder = bs;
        }
    }
    else
    {
        // The zstd tool's defaults: level 3, with the content checksum that
        // lets a damaged image be told from a good one. One thread: that is
        // already faster than most devices read.
        ZSTD_CCtx *cctx = ZSTD_createCCtx();
        int code = (cctx == NULL) ? (int)ZSTD_error_memory_allocation : 0;
        if (cctx != NULL)
        {
            size_t ret = ZSTD_CCtx_setParameter(cctx, ZSTD_c_compressionLevel,
                                                ZSTD_CLEVEL_DEFAULT);
            if (!ZSTD_isError(ret))
            {
                ret = ZSTD_CCtx_setParameter(cctx, ZSTD_c_checksumFlag, 1);
            }
            if (ZSTD_isError(ret))
            {
                code = (int)ZSTD_getErrorCode(ret);
                ZSTD_freeCCtx(cctx);
            }
        }
        if (code != 0)
        {
            myError = QObject::tr("The zstd compressor could not be started "
                                  "(zstd error %1).").arg(code);
        }
        else
        {
            ZstdEncoder *enc = new ZstdEncoder;
            enc->cctx = cctx;
            enc->in.src = NULL;
            enc->in.size = 0;
            enc->in.pos = 0;
            myEncoder = enc;
        }
    }
    if (myEncoder == NULL)
    {
        CloseHandle(myHandle);
        myHandle = INVALID_HANDLE_VALUE;
        return false;
    }
    myOutput.resize(OUTPUT_CHUNK);
    return true;
}

// Runs the encoder over the input set on it and writes out all output
// produced. Without finishing, returns once the input is consumed and nothing
// more comes out; with finishing, loops until the encoder ends the stream.
bool ImageSink::drain(bool finishing)
{
    for (;;)
    {
        size_t produced = 0;
        bool streamended = false;
        bool inputleft = false;
        if (myFormat == FORMAT_GZIP)
        {
            z_stream *zs = (z_stream *)myEncoder;
            zs->next_out = &myOutput[0];
            zs->avail_out = (uInt)myOutput.size();
            int ret = deflate(zs, finishing ? Z_FINISH : Z_NO_FLUSH);
            if (ret != Z_OK && ret != Z_STREAM_END && ret != Z_BUF_ERROR)
            {
                myError = QObject::tr("The gzip compressor failed (zlib error %1).").arg(ret);
                return false;
            }
            produced = myOutput.size() - zs->avail_out;
            streamended = (ret == Z_STREAM_END);
            inputleft = zs->avail_in != 0;
        }
        else if (myFormat == FORMAT_XZ)
        {
            lzma_stream *ls = (lzma_stream *)myEncoder;
            ls->next_out = &myOutput[0];
            ls->avail_out = myOutput.size();
            lzma_ret ret = lzma_code(ls, finishing ? LZMA_FINISH : LZMA_RUN);
            if (ret != LZMA_OK && ret != LZMA_STREAM_END)
            {
                myError = QObject::tr("The xz compressor failed (lzma error %1).")
                              .arg((int)ret);
                return false;
            }
            produced = myOutput.size() - ls->avail_out;
            streamended = (ret == LZMA_STREAM_END);
            inputleft = ls->avail_in != 0;
        }
        else if (myFormat == FORMAT_BZIP2)
        {
            bz_stream *bs = (bz_stream *)myEncoder;
            bs->next_out = (char *)&myOutput[0];
            bs->avail_out = (unsigned int)myOutput.size();
            int ret = BZ2_bzCompress(bs, finishing ? BZ_FINISH : BZ_RUN);
            // BZ_RUN reports making no progress as BZ_PARAM_ERROR, which with
            // the input used up and nothing to flush is just "done".
            if (!finishing && ret == BZ_PARAM_ERROR && bs->avail_in == 0)
            {
                return true;
            }
            if (ret != (finishing ? BZ_FINISH_OK : BZ_RUN_OK) && ret != BZ_STREAM_END)
            {
                myError = QObject::tr("The bzip2 compressor failed (bzip2 error %1).").arg(ret);
                return false;
            }
            produced = myOutput.size() - bs->avail_out;
            streamended = (ret == BZ_STREAM_END);
            inputleft = bs->avail_in != 0;
        }
        else
        {
            ZstdEncoder *enc = (ZstdEncoder *)myEncoder;
            ZSTD_outBuffer out = { &myOutput[0], myOutput.size(), 0 };
            // With ZSTD_e_end the return is what is still to be flushed.
            size_t ret = ZSTD_compressStream2(enc->cctx, &out, &enc->in,
                                              finishing ? ZSTD_e_end : ZSTD_e_continue);
            if (ZSTD_isError(ret))
            {
                myError = QObject::tr("The zstd compressor failed (zstd error %1).")
                              .arg((int)ZSTD_getErrorCode(ret));
                return false;
            }
            produced = out.pos;
            streamended = finishing && ret == 0;
            inputleft = enc->in.pos < enc->in.size;
        }

        if (produced > 0)
        {
            DWORD written = 0;
            if (!WriteFile(myHandle, &myOutput[0], (DWORD)produced, &written, NULL)
                || written != produced)
            {
                myError = QObject::tr("The image file could not be written (error %1).")
                              .arg(GetLastError());
                return false;
            }
        }

        if (streamended)
        {
            return true;
        }
        if (!finishing && produced == 0 && !inputleft)
        {
            return true;
        }
    }
}

// Points the encoder at len bytes of input, or at none.
void ImageSink::setInput(const unsigned char *in, size_t len)
{
    switch (myFormat)
    {
        case FORMAT_GZIP:
            ((z_stream *)myEncoder)->next_in = (Bytef *)in;
            ((z_stream *)myEncoder)->avail_in = (uInt)len;
            break;
        case FORMAT_XZ:
            ((lzma_stream *)myEncoder)->next_in = in;
            ((lzma_stream *)myEncoder)->avail_in = len;
            break;
        case FORMAT_BZIP2:
            ((bz_stream *)myEncoder)->next_in = (char *)in;
            ((bz_stream *)myEncoder)->avail_in = (unsigned int)len;
            break;
        case FORMAT_ZSTD:
            ((ZstdEncoder *)myEncoder)->in.src = in;
            ((ZstdEncoder *)myEncoder)->in.size = len;
            ((ZstdEncoder *)myEncoder)->in.pos = 0;
            break;
    }
}

bool ImageSink::write(const char *data, unsigned long long len)
{
    myError.clear();
    if (myEncoder == NULL)
    {
        // Not opened, or already finished/aborted.
        myError = QObject::tr("The image file is not open for writing.");
        return false;
    }
    const unsigned char *in = (const unsigned char *)data;
    while (len > 0ull)
    {
        // zlib's and bzip2's avail_in are 32-bit; the others use the same
        // chunk for simplicity.
        size_t chunk = (len > 0xffffffffull) ? 0xffffffffu : (size_t)len;
        setInput(in, chunk);
        if (!drain(false))
        {
            return false;
        }
        in += chunk;
        len -= chunk;
    }
    return true;
}

bool ImageSink::finish()
{
    myError.clear();
    if (myEncoder == NULL)
    {
        // Not opened, or already finished/aborted (finish() ends in abort(),
        // which frees the encoder).
        myError = QObject::tr("The image file is not open for writing.");
        return false;
    }
    setInput(NULL, 0);
    if (!drain(true))
    {
        return false;
    }
    bool flushed = FlushFileBuffers(myHandle) != 0;
    // Before abort(), whose cleanup calls can overwrite it.
    const DWORD flusherror = flushed ? 0 : GetLastError();
    abort();
    if (!flushed)
    {
        myError = QObject::tr("The image file could not be flushed (error %1).")
                      .arg(flusherror);
        return false;
    }
    return true;
}

char *ImageSource::read(unsigned long long startsector, unsigned long long count,
                        unsigned long long *sectorsread)
{
    myError.clear();
    if (sectorsread != NULL)
    {
        *sectorsread = 0ull;
    }
    if (count == 0ull)
    {
        return NULL;
    }

    if (myFormat == FORMAT_RAW)
    {
        char *data = new char[(size_t)(mySectorSize * count)];
        DWORD bytesread = 0;
        LARGE_INTEGER li;
        li.QuadPart = (LONGLONG)(startsector * mySectorSize);
        if (!SetFilePointerEx(myHandle, li, NULL, FILE_BEGIN) ||
            !ReadFile(myHandle, data, (DWORD)(mySectorSize * count), &bytesread, NULL))
        {
            myError = QObject::tr("The image file could not be read (error %1).")
                          .arg(GetLastError());
            delete[] data;
            return NULL;
        }
        if (bytesread < mySectorSize * count)
        {
            memset(data + bytesread, 0, (size_t)(mySectorSize * count - bytesread));
        }
        if (sectorsread != NULL)
        {
            *sectorsread = sectorsFor(bytesread, mySectorSize);
        }
        return data;
    }

    if (!skipTo(startsector))
    {
        return NULL;
    }

    char *data = new char[(size_t)(mySectorSize * count)];
    unsigned long long produced = 0ull;
    if (!fill(data, mySectorSize * count, &produced))
    {
        delete[] data;
        return NULL;
    }
    if (produced < mySectorSize * count)
    {
        memset(data + produced, 0, (size_t)(mySectorSize * count - produced));
    }
    unsigned long long full = sectorsFor(produced, mySectorSize);
    myPos += full;
    if (sectorsread != NULL)
    {
        *sectorsread = full;
    }
    return data;
}
