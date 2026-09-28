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

#ifndef COMBINEDIALOG_H
#define COMBINEDIALOG_H

#include <QDialog>
#include <QStringList>
#include "combine.h"

class QCheckBox;
class QComboBox;
class QLabel;
class QListWidget;
class QPushButton;
class QTreeWidget;
class QTreeWidgetItem;

// "Combine images": the user adds image files, ticks the partitions of each to
// put on the device, orders them, and may pick one image's lead-in. Each
// image's partition table is read from its first sectors only; a full scan,
// which decompresses the whole image, is only ever done when asked for. The
// layout is planned (planCombine) and previewed as it changes, and the dialog
// accepts only a plan that fits the device. Nothing is written here.
class CombineDialog : public QDialog
{
    Q_OBJECT
public:
    CombineDialog(QWidget *parent, const QString &deviceText,
                  unsigned long long devicesectors, unsigned long long sectorsize,
                  const QString &startDir, const QStringList &fileFilters);

    // After exec() returns Accepted: the plan, the image files its ranges
    // index, and whether to verify after writing.
    const CombinePlan &plan() const { return myPlan; }
    // Whether plan() is one that can be written.
    bool planIsValid() const { return myPlanOk; }
    // What "Add images..." does with the files it is given; the harness calls
    // it directly.
    void addImageFiles(const QStringList &paths);
    QStringList imagePaths() const;
    bool verifyAfter() const;

private slots:
    void addImages();
    void removeImage();
    void scanImage();
    void moveUp();
    void moveDown();
    void itemChanged(QTreeWidgetItem *item, int column);
    void confirm();

private:
    struct Source
    {
        QString path;
        QString format;             // "gzip", "raw", ...
        unsigned long long sectors; // exact when sizeKnown, else an estimate or 0
        bool sizeKnown;
        bool scanned;               // a full scan read it to the end without error
        ImageLayout layout;
    };

    bool loadImage(const QString &path, Source *src, QString *why);
    // Reads the whole image, to learn its exact size and that it decompresses.
    // False if cancelled or it fails (reported).
    bool fullScan(Source *src);
    void rebuildImages();
    void rebuildOrder();
    void rebuildLeadIn();
    void replan();
    int selectedImage() const;
    QString partitionText(int image, int partition) const;
    QString sizeText(unsigned long long sectors) const;

    QString myDeviceText;
    unsigned long long myDeviceSectors, mySectorSize;
    QString myStartDir;
    QStringList myFilters;

    QList<Source> mySources;
    QList<CombineChoice> myOrder;
    CombinePlan myPlan;
    bool myPlanOk = false;
    bool myNewGuids = false;
    bool myKeepGuidAnswer = false;      // replan() from confirm(): keep myNewGuids
    bool myRebuilding = false;

    QTreeWidget *myImages;
    QPushButton *myRemove, *myScan;
    QListWidget *myOrderList;
    QPushButton *myUp, *myDown;
    QComboBox *myLead;
    QTreeWidget *myPreview;
    QLabel *myStatus;
    QCheckBox *myVerify;
    QPushButton *myWrite;
};

#endif // COMBINEDIALOG_H
