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

#include "combinedialog.h"
#include "imagesource.h"
#include "tooltips.h"

#include <QtWidgets>

// What is read of each image to find its partition table: 1 MiB, which holds
// any ordinary GPT and the superblocks the filesystem guess looks at. A GPT
// whose entries lie further in asks for more, up to 64 MiB.
static const unsigned long long HEAD_BYTES = 1024ull * 1024ull;
static const unsigned long long HEAD_MAX_BYTES = 64ull * 1024ull * 1024ull;
static const unsigned long long ALIGN_BYTES = 1024ull * 1024ull;
// A full scan reads in pieces of this size.
static const unsigned long long SCAN_BYTES = 4ull * 1024ull * 1024ull;

enum { ROLE_IMAGE = Qt::UserRole, ROLE_PARTITION = Qt::UserRole + 1 };

CombineDialog::CombineDialog(QWidget *parent, const QString &deviceText,
                             unsigned long long devicesectors, unsigned long long sectorsize,
                             const QString &startDir, const QStringList &fileFilters)
    : QDialog(parent), myDeviceText(deviceText), myDeviceSectors(devicesectors),
      mySectorSize(sectorsize), myStartDir(startDir), myFilters(fileFilters)
{
    setWindowTitle(tr("Combine images"));
    QVBoxLayout *top = new QVBoxLayout(this);

    QLabel *intro = new QLabel(
        tr("Add image files, tick the partitions to put on the device or in a new image "
           "file, and order them. Each image's partition table is read from its first "
           "sectors; nothing else is read until you write, or ask for a full scan."), this);
    intro->setWordWrap(true);
    top->addWidget(intro);

    // Images and their partitions.
    QGroupBox *imagesBox = new QGroupBox(tr("Images"), this);
    QVBoxLayout *imagesLayout = new QVBoxLayout(imagesBox);
    myImages = new QTreeWidget(imagesBox);
    myImages->setColumnCount(3);
    myImages->setHeaderLabels({ tr("Image / partition"), tr("Type"), tr("Size") });
    myImages->setRootIsDecorated(true);
    myImages->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    myImages->header()->setStretchLastSection(false);
    myImages->setMinimumHeight(myImages->fontMetrics().height() * 10);
    imagesLayout->addWidget(myImages);
    QHBoxLayout *imageButtons = new QHBoxLayout();
    QPushButton *add = new QPushButton(tr("Add images..."), imagesBox);
    myRemove = new QPushButton(tr("Remove"), imagesBox);
    myScan = new QPushButton(tr("Full scan"), imagesBox);
    myScan->setToolTip(tr("Read and decompress the whole image, to learn its exact size and "
                          "check that it holds every partition to its end. Only needed for "
                          "an image with no partition table whose size the file does not "
                          "record, or to check a compressed image before writing."));
    imageButtons->addWidget(add);
    imageButtons->addWidget(myRemove);
    imageButtons->addWidget(myScan);
    imageButtons->addStretch();
    imagesLayout->addLayout(imageButtons);
    top->addWidget(imagesBox, 5);

    // The device: order, lead-in, preview.
    QGroupBox *deviceBox = new QGroupBox(tr("Layout"), this);
    QGridLayout *deviceLayout = new QGridLayout(deviceBox);
    myOrderList = new QListWidget(deviceBox);
    // About six rows: the order of a handful of partitions, no more.
    myOrderList->setMaximumHeight(myOrderList->fontMetrics().height() * 7);
    deviceLayout->addWidget(new QLabel(tr("Partitions, in order:"), deviceBox), 0, 0);
    deviceLayout->addWidget(myOrderList, 1, 0, 2, 1);
    myUp = new QPushButton(tr("Up"), deviceBox);
    myDown = new QPushButton(tr("Down"), deviceBox);
    deviceLayout->addWidget(myUp, 1, 1);
    deviceLayout->addWidget(myDown, 2, 1, Qt::AlignTop);

    QHBoxLayout *leadRow = new QHBoxLayout();
    leadRow->addWidget(new QLabel(tr("Lead-in from:"), deviceBox));
    myLead = new QComboBox(deviceBox);
    myLead->setToolTip(tr("Copy this image's boot code, and the space between its partition "
                          "table and its first partition (up to 32 MiB), where a bootloader "
                          "may be stored. The device then gets the same kind of partition "
                          "table as this image, and the first partition starts where this "
                          "image's did."));
    leadRow->addWidget(myLead, 1);
    deviceLayout->addLayout(leadRow, 3, 0, 1, 2);

    myPreview = new QTreeWidget(deviceBox);
    myPreview->setColumnCount(4);
    myPreview->setHeaderLabels({ tr("On the device"), tr("Start"), tr("Size"), tr("From") });
    myPreview->setRootIsDecorated(false);
    myPreview->header()->setSectionResizeMode(3, QHeaderView::Stretch);
    myPreview->setMinimumHeight(myPreview->fontMetrics().height() * 9);
    deviceLayout->addWidget(myPreview, 4, 0, 1, 2);
    myStatus = new QLabel(deviceBox);
    myStatus->setWordWrap(true);
    deviceLayout->addWidget(myStatus, 5, 0, 1, 2);
    top->addWidget(deviceBox, 3);

    // Where it goes.
    QGroupBox *destBox = new QGroupBox(tr("Write to"), this);
    QGridLayout *destLayout = new QGridLayout(destBox);
    myToDevice = new QRadioButton(deviceText.isEmpty() ? tr("The device (none is selected)")
                                                       : tr("The device: %1").arg(deviceText),
                                  destBox);
    myToFile = new QRadioButton(tr("An image file:"), destBox);
    myOutFile = new QLineEdit(destBox);
    myOutFile->setPlaceholderText(tr("combined.img"));
    myBrowse = new QPushButton(tr("Browse..."), destBox);
    myCompress = new QCheckBox(tr("Compress to"), destBox);
    myFormat = new QComboBox(destBox);
    // In ImageSink's order, as the main window's Compress during Read list.
    myFormat->addItem(".img.gz", (int)ImageSink::FORMAT_GZIP);
    myFormat->addItem(".img.xz", (int)ImageSink::FORMAT_XZ);
    myFormat->addItem(".img.bz2", (int)ImageSink::FORMAT_BZIP2);
    myFormat->addItem(".img.zst", (int)ImageSink::FORMAT_ZSTD);
    destLayout->addWidget(myToDevice, 0, 0, 1, 4);
    destLayout->addWidget(myToFile, 1, 0);
    destLayout->addWidget(myOutFile, 1, 1, 1, 2);
    destLayout->addWidget(myBrowse, 1, 3);
    QHBoxLayout *compressRow = new QHBoxLayout();
    compressRow->addWidget(myCompress);
    compressRow->addWidget(myFormat);
    compressRow->addStretch();
    destLayout->addLayout(compressRow, 2, 1, 1, 3);
    destLayout->setColumnStretch(2, 1);
    top->addWidget(destBox);
    if (devicesectors == 0)
    {
        myToDevice->setEnabled(false);
        myToFile->setChecked(true);
    }
    else
    {
        myToDevice->setChecked(true);
    }

    myVerify = new QCheckBox(tr("Verify after writing"), this);
    myVerify->setChecked(true);
    top->addWidget(myVerify);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Cancel, this);
    myWrite = buttons->addButton(tr("Write..."), QDialogButtonBox::AcceptRole);
    top->addWidget(buttons);

    connect(add, &QPushButton::clicked, this, &CombineDialog::addImages);
    connect(myRemove, &QPushButton::clicked, this, &CombineDialog::removeImage);
    connect(myScan, &QPushButton::clicked, this, &CombineDialog::scanImage);
    connect(myUp, &QPushButton::clicked, this, &CombineDialog::moveUp);
    connect(myDown, &QPushButton::clicked, this, &CombineDialog::moveDown);
    connect(myImages, &QTreeWidget::itemChanged, this, &CombineDialog::itemChanged);
    connect(myImages, &QTreeWidget::currentItemChanged, this, [this]() {
        const int i = selectedImage();
        myRemove->setEnabled(i >= 0);
        myScan->setEnabled(i >= 0);
    });
    connect(myOrderList, &QListWidget::currentRowChanged, this, [this](int row) {
        myUp->setEnabled(row > 0);
        myDown->setEnabled(row >= 0 && row < myOrderList->count() - 1);
    });
    connect(myLead, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            [this]() { if (!myRebuilding) replan(); });
    connect(myToDevice, &QRadioButton::toggled, this, &CombineDialog::destinationChanged);
    connect(myCompress, &QCheckBox::toggled, this, &CombineDialog::destinationChanged);
    connect(myBrowse, &QPushButton::clicked, this, &CombineDialog::browseOutput);
    connect(buttons, &QDialogButtonBox::accepted, this, &CombineDialog::confirm);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    myRemove->setEnabled(false);
    myScan->setEnabled(false);
    myUp->setEnabled(false);
    myDown->setEnabled(false);
    rebuildLeadIn();
    destinationChanged();
    // As the main window's: once every tooltip is set.
    wrapLongToolTips(this);
    resize(780, 820);
}

QStringList CombineDialog::imagePaths() const
{
    QStringList paths;
    for (const Source &s : mySources)
    {
        paths.append(s.path);
    }
    return paths;
}

bool CombineDialog::verifyAfter() const
{
    return myVerify->isChecked();
}

bool CombineDialog::toFile() const
{
    return myToFile->isChecked();
}

bool CombineDialog::outputCompressed() const
{
    return myCompress->isChecked();
}

ImageSink::Format CombineDialog::outputFormat() const
{
    return (ImageSink::Format)myFormat->currentData().toInt();
}

void CombineDialog::destinationChanged()
{
    const bool file = toFile();
    myOutFile->setEnabled(file);
    myBrowse->setEnabled(file);
    myCompress->setEnabled(file);
    myFormat->setEnabled(file && myCompress->isChecked());
    // The file is sized to the layout, the device is what it is: replanned.
    replan();
}

void CombineDialog::browseOutput()
{
    const QString file = QFileDialog::getSaveFileName(
        this, tr("Save the combined image as"),
        myOutFile->text().isEmpty() ? myStartDir : myOutFile->text(),
        tr("Disk Images (*.img *.img.gz *.img.xz *.img.bz2 *.img.zst)"), NULL,
        // confirm() asks, once it knows the name with its extension.
        QFileDialog::DontConfirmOverwrite);
    if (!file.isEmpty())
    {
        myOutFile->setText(QDir::toNativeSeparators(file));
    }
}

QString CombineDialog::sizeText(unsigned long long sectors) const
{
    // Card capacities are quoted in powers of ten; so is this.
    const double bytes = (double)sectors * (double)mySectorSize;
    if (bytes >= 1e9) return tr("%1 GB").arg(bytes / 1e9, 0, 'f', 2);
    if (bytes >= 1e6) return tr("%1 MB").arg(bytes / 1e6, 0, 'f', 1);
    return tr("%1 KB").arg(bytes / 1e3, 0, 'f', 0);
}

// ------------------------------------------------------------------ images ---

bool CombineDialog::loadImage(const QString &path, Source *src, QString *why)
{
    src->path = path;
    src->scanned = false;
    unsigned long long headsectors = HEAD_BYTES / mySectorSize;
    for (int attempt = 0; attempt < 2; ++attempt)
    {
        ImageSource image;
        if (!image.open(path, mySectorSize))
        {
            *why = image.errorString();
            return false;
        }
        src->format = ImageSource::formatName(image.format());
        src->sizeKnown = image.sizeKnown();
        src->sectors = image.sizeInSectors();
        QByteArray head((int)(headsectors * mySectorSize), 0);
        unsigned long long got = 0;
        if (!image.readInto(head.data(), 0, headsectors, &got))
        {
            *why = image.errorString();
            return false;
        }
        head.resize((int)(got * mySectorSize));
        unsigned long long need = 0;
        QString detail;
        if (parseImageLayout(head, mySectorSize, src->sizeKnown ? src->sectors : 0ull,
                             &src->layout, &need, &detail))
        {
            return true;
        }
        if (need > got && need * mySectorSize <= HEAD_MAX_BYTES && attempt == 0)
        {
            // A GPT whose entry array lies past what was read: read that far.
            headsectors = need;
            continue;
        }
        *why = detail.isEmpty() ? tr("the image ends inside its own partition table") : detail;
        return false;
    }
    *why = tr("the partition table could not be read");
    return false;
}

void CombineDialog::addImages()
{
    addImageFiles(QFileDialog::getOpenFileNames(this, tr("Add images"), myStartDir,
                                                myFilters.join(";;")));
}

void CombineDialog::addImageFiles(const QStringList &files)
{
    for (const QString &file : files)
    {
        const QString path = QDir::toNativeSeparators(file);
        Source src;
        QString why;
        QApplication::setOverrideCursor(Qt::WaitCursor);
        const bool ok = loadImage(path, &src, &why);
        QApplication::restoreOverrideCursor();
        if (!ok)
        {
            QMessageBox::warning(this, tr("Combine images"),
                tr("%1 cannot be used: %2.").arg(QFileInfo(path).fileName(), why));
            continue;
        }
        myStartDir = QFileInfo(path).absolutePath();
        mySources.append(src);
        const int index = mySources.size() - 1;
        if (src.layout.wholeImage && src.layout.partitions[0].sectors == 0)
        {
            if (QMessageBox::question(this, tr("Combine images"),
                    tr("%1 has no partition table, so it is taken as one partition: the "
                       "whole image. The file does not record how big that is, so it has "
                       "to be read to the end to find out.\n\nScan it now?")
                        .arg(QFileInfo(path).fileName()),
                    QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
            {
                fullScan(&mySources[index]);
            }
        }
    }
    rebuildImages();
    rebuildLeadIn();
    replan();
}

void CombineDialog::removeImage()
{
    const int index = selectedImage();
    if (index < 0)
    {
        return;
    }
    const int lead = myLead->currentData().toInt();
    for (int i = myOrder.size() - 1; i >= 0; --i)
    {
        if (myOrder[i].image == index)
        {
            myOrder.removeAt(i);
        }
        else if (myOrder[i].image > index)
        {
            --myOrder[i].image;
        }
    }
    mySources.removeAt(index);
    rebuildImages();
    rebuildLeadIn();
    // The lead-in stays with its image, or goes with it.
    const int newlead = (lead == index) ? -1 : (lead > index ? lead - 1 : lead);
    myRebuilding = true;
    myLead->setCurrentIndex(qMax(0, myLead->findData(newlead)));
    myRebuilding = false;
    rebuildOrder();
    replan();
}

bool CombineDialog::fullScan(Source *src)
{
    ImageSource image;
    if (!image.open(src->path, mySectorSize))
    {
        QMessageBox::critical(this, tr("Full scan"), image.errorString());
        return false;
    }
    const QString name = QFileInfo(src->path).fileName();
    // A known or estimated size gives the bar a range; without one it is busy.
    const unsigned long long estimate = image.sizeInSectors();
    QProgressDialog progress(tr("Scanning %1...").arg(name), tr("Cancel"), 0,
                             estimate ? 1000 : 0, this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(0);
    const unsigned long long chunk = SCAN_BYTES / mySectorSize;
    QByteArray buffer((int)(chunk * mySectorSize), 0);
    unsigned long long total = 0;
    for (;;)
    {
        unsigned long long got = 0;
        if (!image.readInto(buffer.data(), total, chunk, &got))
        {
            QMessageBox::critical(this, tr("Full scan"),
                tr("%1 could not be read to the end: %2").arg(name, image.errorString()));
            return false;
        }
        total += got;
        if (got < chunk)
        {
            break;
        }
        progress.setLabelText(tr("Scanning %1: %2 read...").arg(name, sizeText(total)));
        if (estimate)
        {
            progress.setValue((int)qMin<unsigned long long>(999, total * 1000 / estimate));
        }
        QCoreApplication::processEvents();
        if (progress.wasCanceled())
        {
            return false;
        }
    }
    progress.reset();

    // The exact size, and the table checked against it.
    src->sizeKnown = true;
    src->sectors = total;
    src->scanned = true;
    if (src->layout.wholeImage)
    {
        src->layout.partitions[0].sectors = total;
        return true;
    }
    for (const CombinePartition &p : src->layout.partitions)
    {
        if (p.first + p.sectors > total)
        {
            QMessageBox::critical(this, tr("Full scan"),
                tr("%1 ends at %2, before its partition %3 does: the image is incomplete, "
                   "and that partition cannot be copied whole.")
                    .arg(name, sizeText(total)).arg(p.slot + 1));
            src->scanned = false;
            return false;
        }
    }
    return true;
}

void CombineDialog::scanImage()
{
    const int index = selectedImage();
    if (index < 0)
    {
        return;
    }
    fullScan(&mySources[index]);
    rebuildImages();
    replan();
}

int CombineDialog::selectedImage() const
{
    QTreeWidgetItem *item = myImages->currentItem();
    return item ? item->data(0, ROLE_IMAGE).toInt() : -1;
}

QString CombineDialog::partitionText(int image, int partition) const
{
    const Source &s = mySources[image];
    const CombinePartition &p = s.layout.partitions[partition];
    const QString file = QFileInfo(s.path).fileName();
    if (p.slot < 0)
    {
        return file;
    }
    return p.name.isEmpty() ? tr("%1, partition %2").arg(file).arg(p.slot + 1)
                            : tr("%1, partition %2 (%3)").arg(file).arg(p.slot + 1).arg(p.name);
}

void CombineDialog::rebuildImages()
{
    myRebuilding = true;
    const int current = selectedImage();
    myImages->clear();
    for (int i = 0; i < mySources.size(); ++i)
    {
        const Source &s = mySources[i];
        QTreeWidgetItem *top = new QTreeWidgetItem(myImages);
        top->setText(0, QFileInfo(s.path).fileName());
        top->setToolTip(0, s.path);
        const QString table = (s.layout.table == COMBINE_TABLE_GPT) ? tr("GPT")
                            : (s.layout.table == COMBINE_TABLE_MBR) ? tr("MBR")
                                                                    : tr("no partition table");
        top->setText(1, tr("%1, %2").arg(table, s.format));
        top->setText(2, s.sizeKnown ? sizeText(s.sectors)
                                    : tr("size not recorded"));
        if (s.scanned)
        {
            top->setText(2, tr("%1, scanned").arg(sizeText(s.sectors)));
        }
        top->setData(0, ROLE_IMAGE, i);
        top->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        for (int j = 0; j < s.layout.partitions.size(); ++j)
        {
            const CombinePartition &p = s.layout.partitions[j];
            QTreeWidgetItem *child = new QTreeWidgetItem(top);
            child->setText(0, (p.slot < 0) ? tr("whole image")
                              : p.name.isEmpty() ? tr("Partition %1").arg(p.slot + 1)
                                                 : tr("Partition %1: %2").arg(p.slot + 1).arg(p.name));
            child->setText(1, p.typeLabel);
            child->setText(2, p.sectors ? sizeText(p.sectors) : tr("unknown: scan the image"));
            child->setData(0, ROLE_IMAGE, i);
            child->setData(0, ROLE_PARTITION, j);
            child->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
            bool chosen = false;
            for (const CombineChoice &c : myOrder)
            {
                chosen = chosen || (c.image == i && c.partition == j);
            }
            child->setCheckState(0, chosen ? Qt::Checked : Qt::Unchecked);
        }
        top->setExpanded(true);
        if (i == current)
        {
            myImages->setCurrentItem(top);
        }
    }
    for (int c = 1; c < 3; ++c)
    {
        myImages->resizeColumnToContents(c);
    }
    myRebuilding = false;
    myRemove->setEnabled(selectedImage() >= 0);
    myScan->setEnabled(selectedImage() >= 0);
}

void CombineDialog::rebuildLeadIn()
{
    myRebuilding = true;
    const int current = myLead->count() ? myLead->currentData().toInt() : -1;
    myLead->clear();
    myLead->addItem(tr("None: a new, empty table"), -1);
    for (int i = 0; i < mySources.size(); ++i)
    {
        // An image with no table has no lead-in to give.
        if (mySources[i].layout.table != COMBINE_TABLE_NONE)
        {
            myLead->addItem(QFileInfo(mySources[i].path).fileName(), i);
        }
    }
    myLead->setCurrentIndex(qMax(0, myLead->findData(current)));
    myRebuilding = false;
}

void CombineDialog::rebuildOrder()
{
    const int row = myOrderList->currentRow();
    myOrderList->clear();
    for (const CombineChoice &c : myOrder)
    {
        myOrderList->addItem(partitionText(c.image, c.partition));
    }
    myOrderList->setCurrentRow(qMin(row, myOrderList->count() - 1));
}

void CombineDialog::itemChanged(QTreeWidgetItem *item, int column)
{
    if (myRebuilding || column != 0 || !item->data(0, ROLE_PARTITION).isValid())
    {
        return;
    }
    const int image = item->data(0, ROLE_IMAGE).toInt();
    const int partition = item->data(0, ROLE_PARTITION).toInt();
    int at = -1;
    for (int i = 0; i < myOrder.size(); ++i)
    {
        if (myOrder[i].image == image && myOrder[i].partition == partition) at = i;
    }
    if (item->checkState(0) == Qt::Checked && at < 0)
    {
        myOrder.append(CombineChoice{image, partition});
    }
    else if (item->checkState(0) != Qt::Checked && at >= 0)
    {
        myOrder.removeAt(at);
    }
    rebuildOrder();
    replan();
}

void CombineDialog::moveUp()
{
    const int row = myOrderList->currentRow();
    if (row <= 0) return;
    myOrder.swapItemsAt(row, row - 1);
    rebuildOrder();
    myOrderList->setCurrentRow(row - 1);
    replan();
}

void CombineDialog::moveDown()
{
    const int row = myOrderList->currentRow();
    if (row < 0 || row >= myOrder.size() - 1) return;
    myOrder.swapItemsAt(row, row + 1);
    rebuildOrder();
    myOrderList->setCurrentRow(row + 1);
    replan();
}

// ------------------------------------------------------------------- plan ---

void CombineDialog::replan()
{
    // An answer about duplicate GUIDs was for the layout it was given on;
    // which copy is "first" can change with it. confirm() asks again.
    if (!myKeepGuidAnswer)
    {
        myNewGuids = false;
    }
    myPreview->clear();
    myPlanOk = false;
    myWrite->setEnabled(false);
    if (myOrder.isEmpty())
    {
        myStatus->setText(tr("Tick the partitions to put on the device."));
        myStatus->setStyleSheet(QString());
        return;
    }
    QList<ImageLayout> layouts;
    for (const Source &s : mySources)
    {
        layouts.append(s.layout);
    }
    const int lead = myLead->currentData().toInt();
    QString why;
    // An image file has no size of its own: 0 plans it to fit the layout.
    if (!planCombine(layouts, myOrder, lead, mySectorSize, toFile() ? 0ull : myDeviceSectors,
                     ALIGN_BYTES / mySectorSize, myNewGuids, &myPlan, &why))
    {
        myStatus->setText(tr("This cannot be written: %1.").arg(why));
        myStatus->setStyleSheet("color: #c00000;");
        return;
    }

    auto row = [&](const QString &what, unsigned long long first, unsigned long long count,
                   const QString &from) {
        QTreeWidgetItem *it = new QTreeWidgetItem(myPreview);
        it->setText(0, what);
        it->setText(1, QString::number(first));
        it->setText(2, sizeText(count));
        it->setText(3, from);
    };
    const QString table = (myPlan.table == COMBINE_TABLE_GPT) ? tr("GPT") : tr("MBR");
    row(tr("Partition table (%1)").arg(table), 0, myPlan.headersectors, QString());
    int next = 0;
    if (lead >= 0 && !myPlan.ranges.isEmpty() && myPlan.ranges.first().image == lead
        && myPlan.ranges.size() > myPlan.placed.size())
    {
        const CombineRange &r = myPlan.ranges.first();
        row(tr("Lead-in"), r.dstfirst, r.length, QFileInfo(mySources[lead].path).fileName());
    }
    for (const CombinePlaced &p : myPlan.placed)
    {
        row(tr("Partition %1").arg(++next), p.first, p.sectors, partitionText(p.image, p.partition));
    }
    if (myPlan.table == COMBINE_TABLE_GPT)
    {
        row(tr("Backup GPT"), myPlan.backupfirst,
            (unsigned long long)myPlan.backupregion.size() / mySectorSize, QString());
    }
    for (int c = 0; c < 3; ++c)
    {
        myPreview->resizeColumnToContents(c);
    }

    QString text;
    if (toFile())
    {
        text = tr("%1 partition table, %2 partitions. The image will hold %3.")
                   .arg(table).arg(myPlan.placed.size()).arg(sizeText(myPlan.totalsectors));
    }
    else
    {
        const unsigned long long free = myDeviceSectors - myPlan.usedsectors
            - (myPlan.table == COMBINE_TABLE_GPT ? myPlan.backupregion.size() / mySectorSize : 0);
        text = tr("%1 partition table, %2 partitions. %3 used of %4; %5 left unused.")
                   .arg(table).arg(myPlan.placed.size())
                   .arg(sizeText(myPlan.usedsectors), sizeText(myDeviceSectors), sizeText(free));
    }
    bool unchecked = false;
    for (const CombineChoice &c : myOrder)
    {
        unchecked = unchecked || (!mySources[c.image].sizeKnown && !mySources[c.image].scanned);
    }
    if (unchecked)
    {
        text += "\n" + tr("An image that does not record its size is only known to hold its "
                          "partitions once it is written or scanned.");
    }
    if (!myPlan.duplicateGuids.isEmpty())
    {
        text += "\n" + tr("Some partitions share a unique GUID; you will be asked about it.");
    }
    myStatus->setText(text);
    myStatus->setStyleSheet(QString());
    myPlanOk = true;
    myWrite->setEnabled(true);
}

void CombineDialog::confirm()
{
    if (!myPlanOk)
    {
        return;
    }
    if (toFile())
    {
        // The name as a Read would make it: .img, or .img and the format's
        // extension, appended to what was typed.
        QString typed = myOutFile->text().trimmed();
        if (typed.isEmpty())
        {
            QMessageBox::warning(this, tr("Combine images"), tr("Name the image file to write."));
            return;
        }
        if (QFileInfo(typed).isRelative())
        {
            typed = QDir(myStartDir).filePath(typed);
        }
        const QString path = QDir::toNativeSeparators(QFileInfo(ImageSink::readTargetName(
            typed, outputCompressed(), outputFormat())).absoluteFilePath());
        for (const Source &s : mySources)
        {
            if (QFileInfo(s.path).absoluteFilePath().compare(QFileInfo(path).absoluteFilePath(),
                                                             Qt::CaseInsensitive) == 0)
            {
                QMessageBox::warning(this, tr("Combine images"),
                    tr("%1 is one of the images being combined; choose another name.").arg(path));
                return;
            }
        }
        if (QFileInfo::exists(path)
            && QMessageBox::question(this, tr("Combine images"),
                   tr("%1 already exists. Overwrite it?").arg(path),
                   QMessageBox::Yes | QMessageBox::No, QMessageBox::No) != QMessageBox::Yes)
        {
            return;
        }
        myOutputPath = path;
    }
    if (!myPlan.duplicateGuids.isEmpty() && !myNewGuids)
    {
        QMessageBox box(QMessageBox::Warning, tr("Duplicate partition GUIDs"),
            tr("These unique partition GUIDs belong to more than one of the chosen "
               "partitions:\n\n%1\n\nThe copies are usually the same partition taken from "
               "two copies of one image. With duplicate GUIDs a system that finds its "
               "partitions by PARTUUID -- in fstab or on the kernel command line -- may use "
               "the wrong one.\n\nNew GUIDs can be generated for the later copies; the first "
               "keeps its own. Anything that names a regenerated partition by its old "
               "PARTUUID will then no longer find it.")
                .arg(myPlan.duplicateGuids.join("\n")),
            QMessageBox::NoButton, this);
        QPushButton *regen = box.addButton(tr("Generate new GUIDs"), QMessageBox::AcceptRole);
        QPushButton *keep = box.addButton(tr("Keep them"), QMessageBox::DestructiveRole);
        box.addButton(QMessageBox::Cancel);
        box.setDefaultButton(regen);
        box.exec();
        if (box.clickedButton() == regen)
        {
            myNewGuids = true;
            myKeepGuidAnswer = true;
            replan();
            myKeepGuidAnswer = false;
            if (!myPlanOk)
            {
                return;
            }
        }
        else if (box.clickedButton() != keep)
        {
            return;
        }
    }
    accept();
}
