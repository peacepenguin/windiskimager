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
#include <QMap>
#include <QStringList>
#include <functional>
#include "combine.h"
#include "imagesource.h"

// A device the result can be written to, as the device list shows it.
struct CombineTarget
{
    int number;                     // N in \\.\PhysicalDriveN
    QString text;                   // as the main window's device list words it
    QString description;            // vendor and product
    unsigned long long bytes;
};
// The devices to offer, fixed disks included when showAll is set.
typedef std::function<QList<CombineTarget>(bool showAll)> CombineDeviceLister;
// Disk n's sector size, or 0 if it cannot be read (having said why).
typedef std::function<unsigned long long(int n)> CombineSectorSizeProbe;

class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QListWidget;
class QPushButton;
class QRadioButton;
class QTreeWidget;
class QTreeWidgetItem;

// "Combine images": the user adds sources -- image files, and disks -- ticks
// the partitions of each to put on the device, orders them, and may pick one
// source's lead-in. Each
// image's partition table is read from its first sectors only; a full scan,
// which decompresses the whole image, is only ever done when asked for. The
// layout is planned (planCombine) and previewed as it changes, and the dialog
// accepts only a plan that fits the device. Nothing is written here.
//
// The result can go to a device, chosen here from its own list, or to a new
// image file, raw or compressed as a Read's can be; an image file is planned
// exactly as big as the layout. The main window supplies how devices are
// listed and their sector size read, which need the device list code and
// Administrator; the harness supplies stand-ins.
class CombineDialog : public QDialog
{
    Q_OBJECT
public:
    // preselect is the device to offer first (the main window's), or -1;
    // showAll starts "Show all devices" as the main window has it. sectorsize
    // is what every source is read in, and a device must have.
    CombineDialog(QWidget *parent, CombineDeviceLister listDevices,
                  CombineSectorSizeProbe sectorSizeOf, int preselect, bool showAll,
                  unsigned long long sectorsize,
                  const QString &startDir, const QStringList &fileFilters);

    // The device chosen to write to (when !toFile()): its number, its name
    // as listed, and its size in sectors.
    int targetDevice() const { return myTargetDevice; }
    QString targetText() const;
    unsigned long long targetSectors() const { return myDeviceSectors; }

    // After exec() returns Accepted: the plan, the image files its ranges
    // index, and whether to verify after writing.
    const CombinePlan &plan() const { return myPlan; }
    // Whether to write an image file rather than the device; and if so,
    // which file, compressed or not, and how.
    bool toFile() const;
    QString outputPath() const { return myOutputPath; }
    bool outputCompressed() const;
    ImageSink::Format outputFormat() const;
    // Whether plan() is one that can be written.
    bool planIsValid() const { return myPlanOk; }
    // What "Add images..." does with the files it is given; the harness calls
    // it directly.
    void addImageFiles(const QStringList &paths);
    QStringList imagePaths() const;
    // The disks among the sources the plan reads from, by number.
    QList<int> sourceDisks() const;
    bool verifyAfter() const;

private slots:
    void addImages();
    void addDisks();
    void removeImage();
    void scanImage();
    void moveUp();
    void moveDown();
    void itemChanged(QTreeWidgetItem *item, int column);
    void browseOutput();
    void destinationChanged();
    void refreshDevices();
    void deviceChanged();
    void confirm();

private:
    struct Source
    {
        QString path;               // a file, or ImageSource::devicePath() for a disk
        int disk = -1;              // the disk's number, or -1 for a file
        QString label;              // a disk's description, as the device list has it
        QString format;             // "gzip", "raw", ...
        unsigned long long sectors; // exact when sizeKnown, else an estimate or 0
        bool sizeKnown;
        bool scanned;               // a full scan read it to the end without error
        ImageLayout layout;
    };

    bool loadImage(const QString &path, Source *src, QString *why);
    // Adds disk `number` as a source; reports if it cannot be read.
    void addDisk(int number, const QString &label);
    // Reads the whole image, to learn its exact size and that it decompresses.
    // False if cancelled or it fails (reported).
    bool fullScan(Source *src);
    void rebuildImages();
    void rebuildOrder();
    void rebuildLeadIn();
    void replan();
    int selectedImage() const;
    QString sourceName(int image) const;
    QString partitionText(int image, int partition) const;
    QString sizeText(unsigned long long sectors) const;

    CombineDeviceLister myListDevices;
    CombineSectorSizeProbe mySectorSizeOf;
    QList<CombineTarget> myTargets;
    QMap<int, unsigned long long> mySectorSizes;    // probed once per device
    int myPreselect;
    int myTargetDevice = -1;
    unsigned long long myDeviceSectors = 0;
    QString myDeviceProblem;        // why the chosen device cannot be written, if it cannot
    unsigned long long mySectorSize;
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
    QRadioButton *myToDevice, *myToFile;
    QComboBox *myDeviceBox;
    QCheckBox *myShowAll;
    QLineEdit *myOutFile;
    QPushButton *myBrowse;
    QCheckBox *myCompress;
    QComboBox *myFormat;
    QString myOutputPath;
    QCheckBox *myVerify;
    QPushButton *myWrite;
};

#endif // COMBINEDIALOG_H
