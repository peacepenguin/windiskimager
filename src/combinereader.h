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

#ifndef COMBINEREADER_H
#define COMBINEREADER_H

#include <QMap>
#include <QString>
#include <QStringList>
#include "combine.h"

class ImageSource;

// A combined image, as one stream from sector 0 to plan.totalsectors: the
// table, each range taken from its image, zeros between, the backup GPT at
// the end. For writing it to an image file, which a compressed stream can only
// take in order. Ranges are produced in device order, so an image whose
// partitions go on in another order than it holds them is reopened and read
// again from the start for a range behind where it has got to.
class CombineReader
{
public:
    CombineReader(const CombinePlan &plan, const QStringList &paths,
                  unsigned long long sectorsize);
    ~CombineReader();
    // Its segments point into its own copy of the plan.
    CombineReader(const CombineReader &) = delete;
    CombineReader &operator=(const CombineReader &) = delete;

    unsigned long long totalSectors() const { return myPlan.totalsectors; }

    // The next count sectors, into buf, continuing where the last call
    // stopped; short only at the end, with *got saying how many. False on an
    // error, errorString() saying what.
    bool read(char *buf, unsigned long long count, unsigned long long *got);
    const QString &errorString() const { return myError; }

private:
    // One stretch of the output, in order.
    struct Segment
    {
        unsigned long long first, length;
        int image;                  // -1: from memory (data), -2: zeros
        unsigned long long srcfirst;
        const QByteArray *data;
    };

    bool fromImage(const Segment &s, unsigned long long offset, char *buf,
                   unsigned long long count);

    CombinePlan myPlan;
    QStringList myPaths;
    unsigned long long mySectorSize;
    QList<Segment> mySegments;
    int mySegment = 0;
    unsigned long long myPos = 0;
    // Open images, and the sector each will next produce.
    QMap<int, ImageSource *> myImages;
    QMap<int, unsigned long long> myNext;
    QString myError;
};

#endif // COMBINEREADER_H
