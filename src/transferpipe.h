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

#ifndef TRANSFERPIPE_H
#define TRANSFERPIPE_H

// Overlaps the two halves of a transfer. Write and Verify decompress the
// image on a worker thread while the UI thread writes or reads the device;
// a compressed Read compresses on a worker while the UI thread reads the
// device. Device I/O stays on the UI thread, because its errors are reported
// in message boxes; the image and the compressor report through strings.
//
// Both keep a fixed set of buffers and pass them back and forth, so nothing
// is allocated per chunk.

#include <QMutex>
#include <QQueue>
#include <QString>
#include <QThread>
#include <QWaitCondition>
#include <vector>

class ImageSource;
class ImageSink;

// Reads an ImageSource from sector 0 in chunks of chunksectors, up to total
// sectors, ahead of the caller. The chunks are exactly those a loop calling
// image->read() in order would get, and like such a loop it stops after a
// short read or an error. While it runs, nothing else may use the image;
// stop() (or destruction) ends that.
class ImagePrefetcher
{
public:
    struct Chunk
    {
        unsigned long long start;   // first sector
        unsigned long long count;   // sectors asked for
        unsigned long long got;     // sectors that hold image data
        char *data;                 // count sectors; hand back with release()
        bool ok;                    // false: error says why, data is still valid
        QString error;
    };

    ImagePrefetcher(ImageSource *image, unsigned long long total,
                    unsigned long long chunksectors, unsigned long long sectorsize);
    ~ImagePrefetcher();

    void start();
    // The next chunk, in order, waiting for it if need be. False once every
    // chunk has been handed out.
    bool next(Chunk *chunk);
    // Returns a chunk's buffer for reuse.
    void release(char *data);
    // Waits for the worker to finish the read it is in, and ends it.
    void stop();

private:
    void run();

    ImageSource *myImage;
    unsigned long long myTotal, myChunkSectors, mySectorSize;
    std::vector<char *> myBuffers;
    QQueue<char *> myFree;
    QQueue<Chunk> myReady;
    bool myDone, myStopping;
    QMutex myMutex;
    QWaitCondition myChanged;
    QThread *myThread;
};

// Feeds an ImageSink from a worker thread, in the order data was queued.
// Nothing else may use the sink between start() and finish() or stop().
class SinkWriter
{
public:
    SinkWriter(ImageSink *sink, size_t bufferbytes);
    ~SinkWriter();

    void start();
    // Queues len bytes for writing, copying them into a free buffer (waiting
    // for one if all are queued). Any length: it is split as needed. False
    // once a write has failed.
    bool write(const char *data, unsigned long long len);
    // Waits until everything queued is written. False if a write failed;
    // errorString() says why.
    bool finish();
    // Drops whatever is still queued and ends the worker.
    void stop();
    bool failed();
    QString errorString();

private:
    struct Job
    {
        char *data;
        size_t len;
    };
    void run();

    ImageSink *mySink;
    size_t myBufferBytes;
    std::vector<char *> myBuffers;
    QQueue<char *> myFree;
    QQueue<Job> myQueue;
    bool myFinishing, myStopping, myFailed;
    QString myError;
    QMutex myMutex;
    QWaitCondition myChanged;
    QThread *myThread;
};

#endif // TRANSFERPIPE_H
