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

#ifndef IMAGESOURCE_H
#define IMAGESOURCE_H

#ifndef WINVER
#define WINVER 0x0601
#endif

#include <QString>
#include <windows.h>
#include <vector>

// Sector-oriented reader over a raw, gzip, xz, bzip2 or zstd image.
// Compressed images are decompressed on the fly, never expanded to disk.
//
// Raw images allow random access. Compressed ones are forward-only: read() may
// skip ahead (decompressing and discarding) but never back.
class ImageSource
{
public:
    enum Format { FORMAT_RAW, FORMAT_GZIP, FORMAT_XZ, FORMAT_BZIP2, FORMAT_ZSTD };

    ImageSource();
    ~ImageSource();

    // "gzip" / "xz" / "bzip2" / "zstd" / "raw", for messages.
    static QString formatName(Format f);

    // Detects the format and, where possible, the uncompressed size. Returns
    // false and sets errorString() on failure.
    bool open(const QString &path, unsigned long long sectorsize);
    void close();

    bool isCompressed() const { return myFormat != FORMAT_RAW; }
    // True when sizeInSectors() is exact: always for raw, for xz whenever its
    // index could be read, never for the others -- gzip's trailer records only
    // the last member's size, mod 4 GiB; bzip2 records none; zstd records each
    // frame's own, with nothing to say no more frames follow. When false, the
    // image has to be written until the stream ends.
    bool sizeKnown() const { return mySizeKnown; }
    // Exact when sizeKnown(); otherwise a progress estimate, or 0. Never use it
    // to decide where the image ends unless sizeKnown().
    unsigned long long sizeInSectors() const { return mySectors; }
    const QString &errorString() const { return myError; }

    // Reads count sectors starting at startsector into a newly allocated
    // buffer the caller deletes with delete[]. Short reads at the end of the
    // image are zero-padded and reported through sectorsread, which counts the
    // sectors that actually contain image data. Returns NULL on error.
    // For a compressed image startsector must not go backwards.
    char *read(unsigned long long startsector, unsigned long long count,
               unsigned long long *sectorsread);
    // read() into the caller's buffer of count sectors, for reuse across
    // chunks. Returns false, with errorString() set, on error.
    bool readInto(char *data, unsigned long long startsector, unsigned long long count,
                  unsigned long long *sectorsread);

private:
    bool initDecoder();
    // Decompresses up to len bytes into buf; *produced is short only at the
    // end of the stream. Returns false and sets errorString() on error.
    bool fill(char *buf, unsigned long long len, unsigned long long *produced);
    // gzip, bzip2, zstd: whether another stream or frame starts right after
    // the one that just ended, rather than padding or the end of the file.
    bool nextMemberFollows(bool *follows);
    bool skipTo(unsigned long long startsector);
    // Size probe convention: false alone means only "size unknown", which the
    // write survives by running until the stream ends; a set errorString()
    // means the file could not be read. Keeping them apart stops a disk error
    // passing for "size unknown" and the write running to the end of the device.
    bool readAt(unsigned long long offset, void *buf, unsigned long len);
    bool readGzipSize(unsigned long long filesize);
    bool readXzSize(unsigned long long filesize);
    bool readZstdSize(unsigned long long filesize);
    static bool isBzip2Header(const unsigned char *p);
    static bool isZstdFrame(const unsigned char *p);

    HANDLE myHandle;
    Format myFormat;
    unsigned long long mySectorSize;
    unsigned long long mySectors;
    unsigned long long myCompressedSize;
    unsigned long long myPos;          // next sector the stream will produce
    bool mySizeKnown;
    bool myEof;
    bool myFinishing;                  // xz/bzip2/zstd: input ended, flushing the decoder
    QString myError;

    void *myDecoder;                   // z_stream, lzma_stream, bz_stream or ZSTD_DStream, owned
    bool refillInput(size_t kept, DWORD *got);
    std::vector<unsigned char> myInput;
    unsigned char *myNextIn;
    unsigned long long myAvailIn;
};

// Streaming gzip/xz/bzip2/zstd writer for "Compress during Read". No
// seeking: write() takes the image's bytes in order from the start, and a gap
// must be written as explicit zeros, since a compressed stream cannot leave a
// region unwritten the way a sparse raw file can.
class ImageSink
{
public:
    enum Format { FORMAT_GZIP, FORMAT_XZ, FORMAT_BZIP2, FORMAT_ZSTD };

    ImageSink();
    ~ImageSink();

    // ".gz", ".xz", ".bz2" or ".zst".
    static QString extension(Format format);

    // The file a Read should write, given the name typed in: it ends in .img,
    // or .img plus the format's extension when compressed, and is only ever
    // appended to -- the extension after a plain .img, otherwise the whole
    // ending.
    static QString readTargetName(const QString &typed, bool compressed, Format format);

    bool open(const QString &path, Format format);

    // Nothing is guaranteed to be on disk, or even decodable, until finish()
    // succeeds.
    bool write(const char *data, unsigned long long len);

    // Flushes the compressor and the file, then closes it. Required for a
    // valid image; call abort() instead to give up.
    bool finish();

    // Closes without flushing the compressor, leaving a truncated stream on
    // disk, as canceling a raw read leaves a short raw file.
    void abort();

    const QString &errorString() const { return myError; }

private:
    bool drain(bool finishing);
    void setInput(const unsigned char *in, size_t len);

    HANDLE myHandle;
    Format myFormat;
    QString myError;
    void *myEncoder;                   // z_stream, lzma_stream, bz_stream or ZstdEncoder, owned
    std::vector<unsigned char> myOutput;
};

#endif // IMAGESOURCE_H
