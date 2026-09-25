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

#include "transferpipe.h"
#include "imagesource.h"

#include <cstring>

// Buffers each pipe keeps: one in the caller's hands, one being filled or
// drained by the worker, and two queued between them to absorb uneven speed.
static const int PIPE_BUFFERS = 4;

ImagePrefetcher::ImagePrefetcher(ImageSource *image, unsigned long long total,
                                 unsigned long long chunksectors, unsigned long long sectorsize)
    : myImage(image), myTotal(total), myChunkSectors(chunksectors ? chunksectors : 1ull),
      mySectorSize(sectorsize), myDone(false), myStopping(false), myThread(NULL)
{
    for (int i = 0; i < PIPE_BUFFERS; ++i)
    {
        char *buf = new char[(size_t)(myChunkSectors * mySectorSize)];
        myBuffers.push_back(buf);
        myFree.enqueue(buf);
    }
}

ImagePrefetcher::~ImagePrefetcher()
{
    stop();
    for (char *buf : myBuffers)
    {
        delete[] buf;
    }
}

void ImagePrefetcher::start()
{
    myThread = QThread::create([this]() { run(); });
    myThread->start();
}

void ImagePrefetcher::run()
{
    for (unsigned long long i = 0ull; i < myTotal; i += myChunkSectors)
    {
        char *buf = NULL;
        {
            QMutexLocker lock(&myMutex);
            while (myFree.isEmpty() && !myStopping)
            {
                myChanged.wait(&myMutex);
            }
            if (myStopping)
            {
                break;
            }
            buf = myFree.dequeue();
        }
        Chunk chunk;
        chunk.start = i;
        chunk.count = (myTotal - i >= myChunkSectors) ? myChunkSectors : (myTotal - i);
        chunk.got = 0ull;
        chunk.data = buf;
        chunk.ok = myImage->readInto(buf, i, chunk.count, &chunk.got);
        if (!chunk.ok)
        {
            chunk.error = myImage->errorString();
        }
        {
            QMutexLocker lock(&myMutex);
            myReady.enqueue(chunk);
            myChanged.wakeAll();
        }
        // Where a loop of image->read() calls would stop too.
        if (!chunk.ok || chunk.got < chunk.count)
        {
            break;
        }
    }
    QMutexLocker lock(&myMutex);
    myDone = true;
    myChanged.wakeAll();
}

bool ImagePrefetcher::next(Chunk *chunk)
{
    QMutexLocker lock(&myMutex);
    while (myReady.isEmpty() && !myDone)
    {
        myChanged.wait(&myMutex);
    }
    if (myReady.isEmpty())
    {
        return false;
    }
    *chunk = myReady.dequeue();
    return true;
}

void ImagePrefetcher::release(char *data)
{
    QMutexLocker lock(&myMutex);
    myFree.enqueue(data);
    myChanged.wakeAll();
}

void ImagePrefetcher::stop()
{
    {
        QMutexLocker lock(&myMutex);
        myStopping = true;
        myChanged.wakeAll();
    }
    if (myThread != NULL)
    {
        myThread->wait();
        delete myThread;
        myThread = NULL;
    }
}

SinkWriter::SinkWriter(ImageSink *sink, size_t bufferbytes)
    : mySink(sink), myBufferBytes(bufferbytes ? bufferbytes : 1),
      myFinishing(false), myStopping(false), myFailed(false), myThread(NULL)
{
    for (int i = 0; i < PIPE_BUFFERS; ++i)
    {
        char *buf = new char[myBufferBytes];
        myBuffers.push_back(buf);
        myFree.enqueue(buf);
    }
}

SinkWriter::~SinkWriter()
{
    stop();
    for (char *buf : myBuffers)
    {
        delete[] buf;
    }
}

void SinkWriter::start()
{
    myThread = QThread::create([this]() { run(); });
    myThread->start();
}

void SinkWriter::run()
{
    for (;;)
    {
        Job job;
        bool skip = false;
        {
            QMutexLocker lock(&myMutex);
            while (myQueue.isEmpty() && !myFinishing && !myStopping)
            {
                myChanged.wait(&myMutex);
            }
            if (myStopping || myQueue.isEmpty())
            {
                break;
            }
            job = myQueue.dequeue();
            // After a failure the rest is only handed back, never written:
            // the stream is already broken.
            skip = myFailed;
        }
        const bool ok = skip || mySink->write(job.data, job.len);
        QMutexLocker lock(&myMutex);
        if (!ok)
        {
            myFailed = true;
            myError = mySink->errorString();
        }
        myFree.enqueue(job.data);
        myChanged.wakeAll();
    }
}

bool SinkWriter::write(const char *data, unsigned long long len)
{
    while (len > 0ull)
    {
        char *buf = NULL;
        {
            QMutexLocker lock(&myMutex);
            while (myFree.isEmpty() && !myFailed)
            {
                myChanged.wait(&myMutex);
            }
            if (myFailed)
            {
                return false;
            }
            buf = myFree.dequeue();
        }
        const size_t n = (len > myBufferBytes) ? myBufferBytes : (size_t)len;
        memcpy(buf, data, n);
        {
            QMutexLocker lock(&myMutex);
            myQueue.enqueue(Job{buf, n});
            myChanged.wakeAll();
        }
        data += n;
        len -= n;
    }
    return !failed();
}

bool SinkWriter::finish()
{
    {
        QMutexLocker lock(&myMutex);
        myFinishing = true;
        myChanged.wakeAll();
    }
    if (myThread != NULL)
    {
        myThread->wait();
        delete myThread;
        myThread = NULL;
    }
    return !failed();
}

void SinkWriter::stop()
{
    {
        QMutexLocker lock(&myMutex);
        myStopping = true;
        myChanged.wakeAll();
    }
    if (myThread != NULL)
    {
        myThread->wait();
        delete myThread;
        myThread = NULL;
    }
}

bool SinkWriter::failed()
{
    QMutexLocker lock(&myMutex);
    return myFailed;
}

QString SinkWriter::errorString()
{
    QMutexLocker lock(&myMutex);
    return myError;
}
