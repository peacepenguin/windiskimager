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

#ifndef WINVER
#define WINVER 0x0601
#endif

#include "combinereader.h"
#include "imagesource.h"

#include <QFileInfo>
#include <QObject>
#include <algorithm>
#include <cstring>

CombineReader::CombineReader(const CombinePlan &plan, const QStringList &paths,
                             unsigned long long sectorsize)
    : myPlan(plan), myPaths(paths), mySectorSize(sectorsize)
{
    // Everything that is not zeros, in device order; the gaps are filled in
    // below.
    QList<Segment> parts;
    parts.append(Segment{0ull, myPlan.headersectors, -1, 0ull, &myPlan.headerregion});
    for (const CombineRange &r : myPlan.ranges)
    {
        parts.append(Segment{r.dstfirst, r.length, r.image, r.srcfirst, NULL});
    }
    if (!myPlan.backupregion.isEmpty())
    {
        parts.append(Segment{myPlan.backupfirst,
                             (unsigned long long)myPlan.backupregion.size() / mySectorSize,
                             -1, 0ull, &myPlan.backupregion});
    }
    std::sort(parts.begin(), parts.end(), [](const Segment &a, const Segment &b)
    {
        return a.first < b.first;
    });
    unsigned long long at = 0ull;
    for (const Segment &s : parts)
    {
        if (s.first > at)
        {
            mySegments.append(Segment{at, s.first - at, -2, 0ull, NULL});
        }
        mySegments.append(s);
        at = s.first + s.length;
    }
    if (myPlan.totalsectors > at)
    {
        mySegments.append(Segment{at, myPlan.totalsectors - at, -2, 0ull, NULL});
    }
}

CombineReader::~CombineReader()
{
    for (ImageSource *image : myImages)
    {
        delete image;
    }
}

bool CombineReader::fromImage(const Segment &s, unsigned long long offset, char *buf,
                              unsigned long long count)
{
    const unsigned long long src = s.srcfirst + offset;
    ImageSource *image = myImages.value(s.image, NULL);
    // Behind where a compressed image has got to: it cannot seek back, so
    // it starts again. A raw one can, and is left open.
    if (image && image->isCompressed() && src < myNext.value(s.image))
    {
        delete image;
        myImages.remove(s.image);
        image = NULL;
    }
    if (!image)
    {
        image = new ImageSource();
        if (!image->open(myPaths[s.image], mySectorSize))
        {
            myError = image->errorString();
            delete image;
            return false;
        }
        myImages.insert(s.image, image);
    }
    unsigned long long got = 0ull;
    if (!image->readInto(buf, src, count, &got))
    {
        myError = QObject::tr("%1: %2").arg(QFileInfo(myPaths[s.image]).fileName(),
                                          image->errorString());
        return false;
    }
    myNext.insert(s.image, src + got);
    if (got < count)
    {
        myError = QObject::tr("%1 ends at sector %2, before the partition it is to supply "
                              "there does: the image is incomplete.")
                      .arg(QFileInfo(myPaths[s.image]).fileName()).arg(src + got);
        return false;
    }
    return true;
}

bool CombineReader::read(char *buf, unsigned long long count, unsigned long long *got)
{
    *got = 0ull;
    while (*got < count && mySegment < mySegments.size())
    {
        const Segment &s = mySegments[mySegment];
        const unsigned long long offset = myPos - s.first;
        const unsigned long long n = qMin(count - *got, s.length - offset);
        char *out = buf + *got * mySectorSize;
        if (s.image == -2)
        {
            memset(out, 0, (size_t)(n * mySectorSize));
        }
        else if (s.image == -1)
        {
            memcpy(out, s.data->constData() + offset * mySectorSize, (size_t)(n * mySectorSize));
        }
        else if (!fromImage(s, offset, out, n))
        {
            return false;
        }
        *got += n;
        myPos += n;
        if (myPos == s.first + s.length)
        {
            ++mySegment;
        }
    }
    return true;
}
