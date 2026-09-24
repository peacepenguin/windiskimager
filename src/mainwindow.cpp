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
 *  Copyright (C) 2009, Justin Davis <tuxdavis@gmail.com>             *
 *  Copyright (C) 2009-2017 ImageWriter developers                    *
 *                 https://sourceforge.net/projects/win32diskimager/  *
 *  ---                                                               *
 *  Modified 2026 by peacepenguin (fork not affiliated      *
 *  with the upstream ImageWriter project):                           *
 *  GPT repair, device list, dialogs, hashing                         *
 *  https://github.com/peacepenguin/windiskimager                   *
 **********************************************************************/

#ifndef WINVER
#define WINVER 0x0601
#endif

#include <QtWidgets>
#include <QCoreApplication>
#include <QFileInfo>
#include <QDirIterator>
#include <QClipboard>
#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <winioctl.h>
#include <dbt.h>
#include <shlobj.h>
#include <shellapi.h>
#include <shobjidl.h>
#include <iostream>
#include <string>
#include <vector>
#include <climits>

#include "disk.h"
#include "mainwindow.h"
#include "imagesource.h"
#include "elapsedtimer.h"

MainWindow* MainWindow::instance = NULL;

// Sectors moved per pass of every transfer loop.
static const unsigned long long TRANSFER_SECTORS = 1024ull;

// QProgressBar counts in int, which a multi-terabyte disk's sector count
// overflows; progress is shifted right by this much so the bar cannot wrap.
static int progressShift(unsigned long long total)
{
    int shift = 0;
    while ((total >> shift) > (unsigned long long)INT_MAX)
    {
        ++shift;
    }
    return shift;
}

// The format of each item in the .ui's "Compress during Read" dropdown, in
// order: .img.gz, .img.xz, .img.bz2, .img.zst.
static ImageSink::Format readFormatFor(int index)
{
    static const ImageSink::Format formats[] = {
        ImageSink::FORMAT_GZIP, ImageSink::FORMAT_XZ,
        ImageSink::FORMAT_BZIP2, ImageSink::FORMAT_ZSTD,
    };
    const int n = (int)(sizeof(formats) / sizeof(formats[0]));
    return formats[(index >= 0 && index < n) ? index : 0];
}

// With only an estimated image size the loop runs to the device size, but the
// bar tracks the estimate so it describes the image rather than the card.
static unsigned long long progressTotalFor(const ImageSource &image,
                                           unsigned long long numsectors)
{
    if (!image.sizeKnown() && image.sizeInSectors() > 0ull
        && image.sizeInSectors() < numsectors)
    {
        return image.sizeInSectors();
    }
    return numsectors;
}

// An estimate can be passed (gzip's size field wraps at 4 GiB), which would
// pin the bar at 100% with the image still being written. Past it, fall back
// to the device size, re-deriving the shift so the range still fits an int.
static void growProgressTotal(QProgressBar *bar, unsigned long long done,
                              unsigned long long devicetotal,
                              unsigned long long *total, int *shift)
{
    if (done <= *total || *total >= devicetotal)
    {
        return;
    }
    *total = devicetotal;
    *shift = progressShift(devicetotal);
    bar->setRange(0, (int)(devicetotal >> *shift));
}

// Qt word-wraps a tooltip only if it looks like rich text, so a long plain one
// becomes a single line clipped at both screen edges. Break it into lines here
// and keep it plain: an HTML wrapper needs a fixed pixel width that every long
// tooltip gets padded to. Done at runtime so translations are wrapped too and
// the strings translators see stay free of markup.
static QString wrapToolTipText(const QString &tip, int maxWidthPx, const QFontMetrics &fm)
{
    QStringList out;
    QString line;

    const QStringList words = tip.split(QChar(' '), Qt::SkipEmptyParts);
    for (const QString &word : words)
    {
        const QString candidate = line.isEmpty() ? word : line + QChar(' ') + word;
        // A word wider than the budget gets its own line rather than a break.
        if (!line.isEmpty() && fm.horizontalAdvance(candidate) > maxWidthPx)
        {
            out.append(line);
            line = word;
        }
        else
        {
            line = candidate;
        }
    }
    if (!line.isEmpty())
    {
        out.append(line);
    }
    return out.join(QChar('\n'));
}

static void wrapLongToolTips(QWidget *root)
{
    // A ceiling, not a width: nothing is padded out to it.
    const int maxWidthPx = 380;
    const QFontMetrics fm(QToolTip::font());

    const QList<QWidget *> widgets = root->findChildren<QWidget *>();
    for (QWidget *w : widgets)
    {
        const QString tip = w->toolTip();
        // Leave tips that already fit, and ones already marked up.
        if (tip.isEmpty() || Qt::mightBeRichText(tip)
            || fm.horizontalAdvance(tip) <= maxWidthPx)
        {
            continue;
        }
        w->setToolTip(wrapToolTipText(tip, maxWidthPx, fm));
    }
}

// Sets the bar up for a run of total sectors, starts the clocks and resets
// showThroughput()'s marker. Returns the shift to apply before setValue().
int MainWindow::beginProgress(unsigned long long total, unsigned long long *lastsector)
{
    const int shift = progressShift(total);
    progressbar->setRange(0, (total == 0ull) ? 100 : (int)(total >> shift));
    *lastsector = 0ull;
    update_timer.start();
    elapsed_timer->start();
    return shift;
}

// The bar is hidden while idle but keeps its space (see the constructor), so
// the window does not shift when a run starts.
void MainWindow::showProgress(bool show)
{
    progressbar->reset();
    progressbar->setVisible(show);
}

// Locks and opens the device, then opens the image, for write and verify. On
// failure it has already reported, cleaned up and called endRun().
//
// Every run closes the disk handle before releasing the volume locks: unlocking
// lets mountmgr rescan the disk at once, and a rescan triggers the Windows GPT
// "repair" this program exists to avoid.
bool MainWindow::acquireDeviceAndImage(int deviceID, LockedVolumes &locked,
                                       ImageSource &image,
                                       unsigned long long *devicesectors,
                                       const QString &errorTitle,
                                       const QString &failedMessage)
{
    // Every volume on the disk is locked so no filesystem driver flushes
    // cached metadata into the middle of a run.
    if (!locked.lockAll(deviceID))
    {
        endRun(failedMessage);
        return false;
    }
    // Device first: the image reader needs its sector size. Read-write for
    // verify too, which may repair the GPT and offlines the disk when done.
    hRawDisk = getHandleOnDevice(deviceID, GENERIC_READ | GENERIC_WRITE);
    if (hRawDisk == INVALID_HANDLE_VALUE)
    {
        locked.release();
        endRun(failedMessage);
        return false;
    }
    bool geometryreported = false;
    *devicesectors = getNumberOfSectors(hRawDisk, &sectorsize, &geometryreported);
    if (!*devicesectors && !geometryreported)
    {
        // A card reader whose card has been pulled stays present and reports
        // size zero, with no WM_DEVICECHANGE to say so.
        QMessageBox::critical(this, tr("Device Error"),
            tr("The device reports a size of zero. If it is a card reader, "
               "the card may have been removed."));
        CloseHandle(hRawDisk);
        hRawDisk = INVALID_HANDLE_VALUE;
        locked.release();
        endRun(failedMessage);
        return false;
    }
    if (!*devicesectors)
    {
        // getNumberOfSectors already said what went wrong.
        CloseHandle(hRawDisk);
        hRawDisk = INVALID_HANDLE_VALUE;
        locked.release();
        endRun(failedMessage);
        return false;
    }
    if (!image.open(leFile->text(), sectorsize))
    {
        QMessageBox::critical(this, errorTitle, image.errorString());
        CloseHandle(hRawDisk);
        hRawDisk = INVALID_HANDLE_VALUE;
        locked.release();
        endRun(failedMessage);
        return false;
    }
    return true;
}

// Whether the part of the image past the device end holds anything but zeros.
// A compressed image is forward-only, so its tail is not examined: that case
// returns false with *datafound false.
bool MainWindow::imageTailHasData(ImageSource &image, unsigned long long from,
                                  unsigned long long to, bool *datafound)
{
    *datafound = false;
    if (image.isCompressed())
    {
        return false;
    }
    for (unsigned long long at = from; at < to && !*datafound; )
    {
        unsigned long chunk = ((to - at) >= TRANSFER_SECTORS) ? TRANSFER_SECTORS
                                                     : (unsigned long)(to - at);
        char *data = image.read(at, chunk, NULL);
        if (data == NULL)
        {
            // A read error in a stretch that will not be written or compared
            // says nothing useful; stop looking rather than report it.
            break;
        }
        unsigned long long limit = (unsigned long long)chunk * sectorsize;
        for (unsigned long long j = 0ull; j < limit; ++j)
        {
            if (data[j] != 0)
            {
                *datafound = true;
                break;
            }
        }
        delete[] data;
        at += chunk;
    }
    return true;
}

// Transfer rate, at most once a second.
void MainWindow::showThroughput(unsigned long long sector, unsigned long long total,
                                unsigned long long *lastsector)
{
    if (update_timer.elapsed() < ONE_SEC_IN_MS)
    {
        return;
    }
    const double mbpersec =
        (((double)sectorsize * (sector - *lastsector))
         * ((double)ONE_SEC_IN_MS / update_timer.elapsed())) / 1024.0 / 1024.0;
    statusbar->showMessage(QString("%1 MB/s").arg(mbpersec));
    elapsed_timer->update(sector, total);
    update_timer.start();
    *lastsector = sector;
}

// Returns the window to idle after a run that fails or is cancelled early. A
// window close confirmed during the run (STATUS_EXIT) is carried out here,
// since resetting status would otherwise forget it.
void MainWindow::endRun(const QString &message)
{
    const bool exiting = (status == STATUS_EXIT);
    status = STATUS_IDLE;
    showProgress(false);
    elapsed_timer->stop();
    statusbar->showMessage(message);
    bCancel->setEnabled(false);
    setReadWriteButtonState();
    if (exiting)
    {
        close();
    }
}

// An empty status bar reads as blank space; a shade off the window colour and
// a hairline above give it an edge. Taken from the palette to follow the theme.
static void shadeStatusBar(QStatusBar *bar)
{
    const QColor window = bar->palette().color(QPalette::Window);
    const bool dark = window.lightness() < 128;
    const QColor fill = dark ? window.lighter(118) : window.darker(106);
    const QColor line = dark ? window.lighter(140) : window.darker(118);
    bar->setStyleSheet(QString("QStatusBar { background: %1; border-top: 1px solid %2; }"
                               "QStatusBar::item { border: none; }")
                           .arg(fill.name(), line.name()));
}

// The style shows "pressed" by dimming the label, which an icon-only button
// lacks, and its background shifts by one level out of 255. Give it a visible
// pressed fill from the palette. The selector names the button's own class:
// an unscoped rule would be inherited by its tooltip.
static void shadePressedIconButton(QAbstractButton *button)
{
    const QColor base = button->palette().color(QPalette::Button);
    const bool dark = base.lightness() < 128;
    const QColor fill = dark ? base.lighter(128) : base.darker(108);

    button->setStyleSheet(QString("%1:pressed { background-color: %2; }")
                              .arg(QString::fromLatin1(button->metaObject()->className()),
                                   fill.name()));
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);
    acceptDroppedFiles();
    wrapLongToolTips(this);
    elapsed_timer = new ElapsedTimer();
    shadeStatusBar(statusbar);
    shadePressedIconButton(tbBrowse);
    statusbar->addPermanentWidget(elapsed_timer);   // "addpermanent" puts it on the RHS of the statusbar
    status = STATUS_IDLE;
    {
        // Hiding a widget normally takes its space with it; this keeps it.
        QSizePolicy sp = progressbar->sizePolicy();
        sp.setRetainSizeWhenHidden(true);
        progressbar->setSizePolicy(sp);
    }
    showProgress(false);
    clipboard = QApplication::clipboard();
    hFile = INVALID_HANDLE_VALUE;
    hRawDisk = INVALID_HANDLE_VALUE;
    if (QCoreApplication::arguments().count() > 1)
    {
        QString fileLocation = QApplication::arguments().at(1);
        QFileInfo fileInfo(fileLocation);
        // Backslashes: Qt hands out '/' whatever the platform, and a Windows
        // user reading their own path expects the separator they type.
        leFile->setText(QDir::toNativeSeparators(fileInfo.absoluteFilePath()));
    }
    cboxHashType->addItem("MD5",QVariant(QCryptographicHash::Md5));
    cboxHashType->addItem("SHA1",QVariant(QCryptographicHash::Sha1));
    cboxHashType->addItem("SHA256",QVariant(QCryptographicHash::Sha256));
    connect(this->cboxHashType, SIGNAL(currentIndexChanged(int)), SLOT(on_cboxHashType_IdxChg()));
    // A command-line image counts as a selection; after the hash list is
    // filled, since this defaults the hash type.
    imageFileChanged();
    sectorData = NULL;
    sectorData2 = NULL;
    sectorsize = 0ul;

    // No settings are persisted. The GPT fix always starts on, so unchecking
    // it once cannot expose later writes, and only removable devices are
    // listed at first, so a fixed disk is never preselected.
    fixGptCheckBox->setChecked(true);
    shrinkOnReadCheckBox->setChecked(false);
    compressReadCheckBox->setChecked(false);
    // .img.gz, the first item, is the default; see readFormatFor().
    compressFormatComboBox->setCurrentIndex(0);
    compressFormatComboBox->setEnabled(false);
    choosePartitionsCheckBox->setChecked(false);
    showAllDevicesCheckBox->setChecked(false);
    // After showAllDevicesCheckBox is set, which the scan reads. Deferred
    // until the window is shown: a spun-down disk can take seconds to report
    // its geometry, and from here that wait comes before any window appears.
    QTimer::singleShot(0, this, [this]() { rescanDevices(); });
    // A card inserted into a reader that presents no volume sends no
    // WM_DEVICECHANGE, so the list is also rescanned as it opens. Do not poll
    // instead: querying every disk periodically keeps spinning disks awake.
    connect(cboxDevice, &DeviceComboBox::aboutToShowPopup,
            this, &MainWindow::rescanDevices);

    initializeHomeDir();
    // Opening decides the format by content (ImageSource::open), so these
    // only choose what the dialog lists.
    myFileType = tr("Disk Images (*.img *.IMG *.raw *.bin *.img.gz *.img.xz *.img.bz2 *.img.zst *.raw.gz *.raw.xz *.raw.bz2 *.raw.zst)");
    myFileTypeList << tr("Disk Images (*.img *.IMG *.raw *.bin *.img.gz *.img.xz *.img.bz2 *.img.zst *.raw.gz *.raw.xz *.raw.bz2 *.raw.zst)")
                   << tr("Compressed Disk Images (*.gz *.xz *.bz2 *.zst)")
                   << "*.*";

    // Last, once every string is set: the size needed depends on the
    // language. Use sizeHint() rather than the .ui's arbitrary 520x355; a
    // QCheckBox does not elide, so too small a window cuts labels off.
    resize(sizeHint());
}

MainWindow::~MainWindow()
{
    if (hRawDisk != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hRawDisk);
        hRawDisk = INVALID_HANDLE_VALUE;
    }
    if (hFile != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hFile);
        hFile = INVALID_HANDLE_VALUE;
    }
    if (sectorData != NULL)
    {
        delete[] sectorData;
        sectorData = NULL;
    }
    if (sectorData2 != NULL)
    {
        delete[] sectorData2;
        sectorData2 = NULL;
    }
    if (elapsed_timer != NULL)
    {
        delete elapsed_timer;
        elapsed_timer = NULL;
    }
    if (cboxHashType != NULL)
    {
       cboxHashType->clear();
    }
}


void MainWindow::initializeHomeDir()
{
    QString downloadPath = qgetenv("DiskImagesDir");
    if (downloadPath.isEmpty()) {
        PWSTR pPath = NULL;
        static GUID downloads = {0x374de290, 0x123f, 0x4565, {0x91, 0x64, 0x39,
                                 0xc4, 0x92, 0x5e, 0x46, 0x7b}};
        if (SHGetKnownFolderPath(downloads, 0, 0, &pPath) == S_OK) {
            downloadPath = QDir::fromNativeSeparators(QString::fromWCharArray(pPath));
            CoTaskMemFree(pPath);
        }
        // Also when the shell gave no answer at all, not just a stale one.
        if (downloadPath.isEmpty() || !QDir(downloadPath).exists()) {
            downloadPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
        }
    }
    if (downloadPath.isEmpty())
        downloadPath = QDir::currentPath();
    myHomeDir = downloadPath;
}

// A hash type is chosen (index 0 is "None") and the file has something to hash.
static bool hashableFile(const QString &file, int typeIndex)
{
    QFileInfo fi(file);
    return typeIndex != 0 && !file.isEmpty() && fi.exists() && fi.isFile()
           && fi.isReadable() && fi.size() > 0;
}

void MainWindow::setReadWriteButtonState()
{
    // The image field and device list stay live during a run and both end up
    // here; re-enabling the buttons would let a second run start inside the
    // first from one of its processEvents() calls.
    if (status != STATUS_IDLE)
    {
        bRead->setEnabled(false);
        bWrite->setEnabled(false);
        bVerify->setEnabled(false);
        bCheckGpt->setEnabled(false);
        // Hashing is synchronous: started from a transfer loop's
        // processEvents(), it would stall the transfer with the disk locked.
        bHashGen->setEnabled(false);
        return;
    }
    bHashGen->setEnabled(hashableFile(leFile->text(), cboxHashType->currentIndex()));
    bool fileSelected = !(leFile->text().isEmpty());
    bool deviceSelected = (cboxDevice->count() > 0);
    QFileInfo fi(leFile->text());

    bRead->setEnabled(deviceSelected && fileSelected && (fi.exists() ? fi.isWritable() : true));
    bWrite->setEnabled(deviceSelected && fileSelected && fi.isReadable());
    bVerify->setEnabled(deviceSelected && fileSelected && fi.isReadable());
    // This one needs no image: it only looks at the device.
    bCheckGpt->setEnabled(deviceSelected);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // Whole sentences per state, not assembled from parts, so each translates
    // as a unit.
    QString atstake;
    if (status == STATUS_READING)
    {
        atstake = tr("Exiting now will result in a corrupt image file.\n"
                     "Are you sure you want to exit?");
    }
    else if (status == STATUS_WRITING)
    {
        atstake = tr("Exiting now will result in a corrupt disk.\n"
                     "Are you sure you want to exit?");
    }
    else if (status == STATUS_VERIFYING)
    {
        atstake = tr("Exiting now will cancel verifying image.\n"
                     "Are you sure you want to exit?");
    }
    else
    {
        return;      // nothing running: let the window close
    }

    if (QMessageBox::warning(this, tr("Exit?"), atstake,
                             QMessageBox::Yes | QMessageBox::No,
                             QMessageBox::No) == QMessageBox::Yes)
    {
        status = STATUS_EXIT;
    }
    event->ignore();
}

// Windows' own Open dialog, not QFileDialog. Given no starting folder, it opens
// where this app last opened a file, remembered by Windows across runs --
// nothing is stored here. QFileDialog always sets a folder (the working
// directory at worst) through IFileDialog::SetFolder, which overrides that.
void MainWindow::on_tbBrowse_clicked()
{
    // See if there is a user-defined file extension.
    QString fileTypeEnv = qgetenv("DiskImagerFiles");

    QStringList fileTypesList = fileTypeEnv.split(";;", Qt::SkipEmptyParts) + myFileTypeList;
    int index = fileTypesList.indexOf(myFileType);
    if (index != -1) {
        fileTypesList.move(index, 0);
    }

    // Qt's "Name (*.a *.b)" becomes the dialog's name and "*.a;*.b" spec.
    std::vector<std::wstring> names, specs;
    for (const QString &filter : fileTypesList)
    {
        const int open = filter.lastIndexOf('('), close = filter.lastIndexOf(')');
        const QString spec = (open >= 0 && close > open)
            ? filter.mid(open + 1, close - open - 1) : QString("*");
        names.push_back(filter.toStdWString());
        specs.push_back(spec.split(' ', Qt::SkipEmptyParts).join(';').toStdWString());
    }
    std::vector<COMDLG_FILTERSPEC> filters;
    for (size_t i = 0; i < names.size(); ++i)
    {
        filters.push_back(COMDLG_FILTERSPEC{names[i].c_str(), specs[i].c_str()});
    }

    QString chosen;
    int chosenType = 0;
    const HRESULT com = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    IFileOpenDialog *dialog = NULL;
    if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER,
                                   IID_PPV_ARGS(&dialog))))
    {
        FILEOPENDIALOGOPTIONS opts = 0;
        dialog->GetOptions(&opts);
        // Read names a file that does not exist yet.
        dialog->SetOptions((opts | FOS_FORCEFILESYSTEM) & ~FOS_FILEMUSTEXIST);
        dialog->SetTitle((LPCWSTR)tr("Select a disk image").utf16());
        if (!filters.empty())
        {
            dialog->SetFileTypes((UINT)filters.size(), filters.data());
            dialog->SetFileTypeIndex(1);
        }

        // Start where a path already in the field points. Otherwise set no
        // folder, so Windows' remembered one applies; the image directory is
        // only the default for the first time ever.
        const QFileInfo current(leFile->text());
        IShellItem *folder = NULL;
        if (current.isAbsolute() && current.absoluteDir().exists()
            && SUCCEEDED(SHCreateItemFromParsingName(
                   (LPCWSTR)QDir::toNativeSeparators(current.absolutePath()).utf16(),
                   NULL, IID_PPV_ARGS(&folder))))
        {
            dialog->SetFolder(folder);
            dialog->SetFileName((LPCWSTR)current.fileName().utf16());
            folder->Release();
        }
        else if (SUCCEEDED(SHCreateItemFromParsingName(
                     (LPCWSTR)QDir::toNativeSeparators(myHomeDir).utf16(),
                     NULL, IID_PPV_ARGS(&folder))))
        {
            dialog->SetDefaultFolder(folder);
            folder->Release();
        }

        IShellItem *result = NULL;
        if (SUCCEEDED(dialog->Show((HWND)winId())) && SUCCEEDED(dialog->GetResult(&result)))
        {
            PWSTR path = NULL;
            if (SUCCEEDED(result->GetDisplayName(SIGDN_FILESYSPATH, &path)))
            {
                chosen = QString::fromWCharArray(path);
                CoTaskMemFree(path);
            }
            UINT type = 0;
            if (SUCCEEDED(dialog->GetFileTypeIndex(&type)))
            {
                chosenType = (int)type;
            }
            result->Release();
        }
        dialog->Release();
    }
    if (SUCCEEDED(com))
    {
        CoUninitialize();
    }

    if (chosen.isEmpty())
    {
        return;   // cancelled
    }
    if (chosenType >= 1 && chosenType <= fileTypesList.size())
    {
        myFileType = fileTypesList.at(chosenType - 1);
    }
    leFile->setText(QDir::toNativeSeparators(chosen));
    myHomeDir = QFileInfo(chosen).absolutePath();
    imageFileChanged();
}

void MainWindow::on_bHashCopy_clicked()
{
    QString hashSum(hashLabel->text());
    if ( !(hashSum.isEmpty()) )
    {
        clipboard->setText(hashSum);
    }
}

void MainWindow::generateHash(const QString &filename, int hashish)
{
    hashLabel->setText(tr("Generating..."));
    hashLabel->setVisible(true);
    QApplication::processEvents();

    QCryptographicHash filehash((QCryptographicHash::Algorithm)hashish);

    // may take a few secs - display a wait cursor
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    QFile file(filename);
    if (!file.open(QFile::ReadOnly))
    {
        hashLabel->setText(tr("Error"));
        bHashCopy->setEnabled(false);
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(this, tr("File Error"),
                              tr("Could not open the file to generate a checksum:\n%1").arg(file.errorString()));
        return;
    }
    // A read that stops part-way still returns a well-formed digest -- of the
    // wrong bytes. Reporting that as the file's checksum defeats the point.
    if (!filehash.addData(&file))
    {
        hashLabel->setText(tr("Error"));
        bHashCopy->setEnabled(false);
        QApplication::restoreOverrideCursor();
        QMessageBox::critical(this, tr("File Error"),
                              tr("Could not read the whole file to generate a checksum:\n%1").arg(file.errorString()));
        return;
    }

    hashLabel->setText(filehash.result().toHex());
    bHashCopy->setEnabled(true);
    QApplication::restoreOverrideCursor();
}


// Offers to repair a broken primary GPT. `lead` opens the message, since
// verify and the standalone check meet the damage in different circumstances.
// Returns true if the table was repaired.
bool MainWindow::offerGptRepair(HANDLE hDisk, unsigned long long disksectorsize,
                                unsigned long long devicesectors, const QString &lead)
{
    const int answer = QMessageBox::warning(this, tr("Partition table damaged"),
        tr("%1 the primary GPT header points at sectors the partition entries "
           "are not in.\n\n"
           "This is what Windows leaves behind when it rescans a card written "
           "without \"Fix GPT after write\". No data has been lost, but the "
           "device will not boot and most tools will refuse the table.\n\n"
           "Repair the partition table now?").arg(lead),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if (answer != QMessageBox::Yes)
    {
        return false;
    }

    QString detail;
    if (repairPrimaryGpt(hDisk, disksectorsize, devicesectors, &detail))
    {
        return true;
    }
    QMessageBox::critical(this, tr("Repair failed"),
        tr("The partition table could not be repaired: %1").arg(detail));
    return false;
}

// Look at the selected device's partition table on its own, writing and
// comparing nothing. A card Windows has already rescanned still matches its
// image sector for sector, so the table is the only place the damage shows.
void MainWindow::on_bCheckGpt_clicked()
{
    // Reachable mid-run via processEvents(); must not lock or rewrite a disk
    // under a running transfer.
    if (status != STATUS_IDLE)
    {
        return;
    }
    const int deviceID = selectedDeviceID();
    if (deviceID < 0)
    {
        QMessageBox::critical(this, tr("Device Error"), tr("Please select a device."));
        return;
    }

    // Locked so nothing else writes the disk while the table is read or
    // repaired.
    LockedVolumes locked;
    if (!locked.lockAll(deviceID))
    {
        statusbar->showMessage(tr("Could not lock the device."));
        return;
    }
    HANDLE hDisk = getHandleOnDevice(deviceID, GENERIC_READ | GENERIC_WRITE);
    if (hDisk == INVALID_HANDLE_VALUE)
    {
        locked.release();
        statusbar->showMessage(tr("Could not open the device."));
        return;
    }
    // Not the sectorsize member: checking another disk must not change it.
    unsigned long long disksectorsize = 0ull;
    unsigned long long devicesectors = getNumberOfSectors(hDisk, &disksectorsize);
    if (!devicesectors)
    {
        CloseHandle(hDisk);
        locked.release();
        QMessageBox::critical(this, tr("Device Error"),
            tr("The device reports a size of zero. If it is a card reader, "
               "the card may have been removed."));
        return;
    }

    switch (gptPrimaryState(hDisk, disksectorsize, devicesectors))
    {
    case GPT_PRIMARY_BROKEN:
        if (offerGptRepair(hDisk, disksectorsize, devicesectors,
                tr("This device's partition table is broken:")))
        {
            statusbar->showMessage(tr("Partition table repaired."));
        }
        else
        {
            statusbar->showMessage(tr("Partition table is still damaged."));
        }
        break;
    case GPT_PRIMARY_OK:
        statusbar->showMessage(tr("Partition table is valid."));
        QMessageBox::information(this, tr("Partition table"),
            tr("The GPT on this device is valid: the header and the partition "
               "entries it points at agree."));
        break;
    case GPT_PRIMARY_NO_GPT:
        statusbar->showMessage(tr("No GPT on this device."));
        QMessageBox::information(this, tr("Partition table"),
            tr("This device has no GPT, so it cannot have the damage this "
               "checks for."));
        break;
    default:
        statusbar->showMessage(tr("Could not read the partition table."));
        QMessageBox::warning(this, tr("Partition table"),
            tr("The partition table could not be read, or is damaged in some "
               "way other than the one this repairs."));
        break;
    }

    CloseHandle(hDisk);
    locked.release();
}

// Defaults to SHA256, the checksum publishers most often quote, but only when
// the image changes: editingFinished fires on every focus loss, and resetting
// each time would undo a hand-picked type.
void MainWindow::defaultHashTypeForFile()
{
    const QString file = leFile->text();
    if (file.isEmpty() || file == myHashDefaultedFor)
    {
        return;
    }
    myHashDefaultedFor = file;
    const int sha256 = cboxHashType->findText("SHA256");
    if (sha256 >= 0)
    {
        cboxHashType->setCurrentIndex(sha256);
    }
}

void MainWindow::on_leFile_editingFinished()
{
    // Presentation only (Qt and Win32 accept both separators); setText does
    // not re-emit this signal.
    const QString typed = leFile->text();
    const QString native = QDir::toNativeSeparators(typed);
    if (native != typed)
    {
        leFile->setText(native);
    }
    imageFileChanged();
}

void MainWindow::on_bCancel_clicked()
{
    if ( (status == STATUS_READING) || (status == STATUS_WRITING) )
    {
        if (QMessageBox::warning(this, tr("Cancel?"), tr("Canceling now will result in a corrupt destination.\n"
                                                         "Are you sure you want to cancel?"),
                                 QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
        {
            status = STATUS_CANCELED;
        }
    }
    else if (status == STATUS_VERIFYING)
    {
        if (QMessageBox::warning(this, tr("Cancel?"), tr("Cancel Verify.\n"
                                                         "Are you sure you want to cancel?"),
                                 QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
        {
            status = STATUS_CANCELED;
        }

    }
}

void MainWindow::on_bWrite_clicked()
{
    // Reachable mid-run through the transfer loops' processEvents() calls;
    // a second run inside the first would share its handles, buffers and
    // status. The same guard opens on_bRead_clicked()/on_bVerify_clicked().
    if (status != STATUS_IDLE)
    {
        return;
    }
    bool passfail = true;
    if (!leFile->text().isEmpty())
    {
        QFileInfo fileinfo(leFile->text());
        if (fileinfo.exists() && fileinfo.isFile() &&
                fileinfo.isReadable() && (fileinfo.size() > 0) )
        {
            if (fileIsOnSelectedDevice(leFile->text()))
            {
                QMessageBox::critical(this, tr("Write Error"), tr("Image file cannot be located on the target device."));
                return;
            }
            int deviceID = selectedDeviceID();
            if (deviceID < 0)
            {
                QMessageBox::critical(this, tr("Write Error"), tr("Please select a target device."));
                return;
            }
            // The dialogs below leave the event loop running: a device-change
            // notification can rebuild the list meanwhile, and Windows gives a
            // newly inserted disk the number of the one just removed. What the
            // user confirms is checked against what gets opened.
            const QString targetText = cboxDevice->currentText();
            const qulonglong targetBytes = cboxDevice->currentData(Qt::UserRole + 1).toULongLong();
            if (QMessageBox::warning(this, tr("Confirm overwrite"), tr("All files and data on this device will be deleted.\n"
                                                                       "(Target Device: %1)\n"
                                                                       "Are you sure you want to continue?").arg(targetText),
                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
            {
                return;
            }
            // A target carrying mounted volumes is the shape of a mistake: a
            // card straight from an imaging tool has no letter Windows can
            // mount, so letters usually mean this is someone's data drive.
            QString targetletters = driveLettersOnDevice((ULONG)deviceID);
            if (!targetletters.isEmpty())
            {
                if (QMessageBox::warning(this, tr("Device has mounted volumes"),
                        tr("%1 is mounted in Windows as %2.\n\n"
                           "Everything on this device, on every one of its partitions, will be "
                           "destroyed and cannot be recovered.\n\n"
                           "Check that %2 is not a drive you meant to keep.\n\n"
                           "Write to this device anyway?")
                            .arg(targetText).arg(targetletters),
                        QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
                {
                    return;
                }
            }
            if (selectedDeviceID() != deviceID || cboxDevice->currentText() != targetText)
            {
                QMessageBox::critical(this, tr("Write Error"),
                    tr("The device list changed while you were confirming. Check the "
                       "target device and try again."));
                return;
            }
            status = STATUS_WRITING;
            showProgress(true);
            bCancel->setEnabled(true);
            bWrite->setEnabled(false);
            bRead->setEnabled(false);
            bVerify->setEnabled(false);
            bCheckGpt->setEnabled(false);
            unsigned long long i, lasti, availablesectors, numsectors;
            LockedVolumes locked;
            ImageSource image;
            if (!acquireDeviceAndImage(deviceID, locked, image, &availablesectors,
                                       tr("Write Error"), tr("Write failed.")))
            {
                return;
            }
            // Both sizes come from the same DiskSize; a mismatch means the
            // disk behind this number is not the one that was confirmed.
            if (targetBytes != 0ull && availablesectors != targetBytes / sectorsize)
            {
                CloseHandle(hRawDisk);
                hRawDisk = INVALID_HANDLE_VALUE;
                locked.release();
                QMessageBox::critical(this, tr("Write Error"),
                    tr("The device list changed while you were confirming. Check the "
                       "target device and try again."));
                endRun(tr("Write failed."));
                return;
            }
            // Without an exact size (ImageSource::sizeKnown()) the loop runs
            // to the device size until the stream ends; the leftover check
            // after the loop says whether it all fitted.
            const bool sizeisestimate = !image.sizeKnown();
            const unsigned long long imagesectors = image.sizeInSectors();
            numsectors = sizeisestimate ? availablesectors : imagesectors;
            if (!numsectors)
            {
                // An empty file, or a compressed stream holding nothing.
                QMessageBox::critical(this, tr("File Error"),
                                      tr("The specified file contains no data."));
                CloseHandle(hRawDisk);
                hRawDisk = INVALID_HANDLE_VALUE;
                locked.release();
                endRun(tr("Write failed."));
                return;
            }
            // A lower bound that already exceeds the device is reason to warn
            // now rather than at the end of the card.
            if (sizeisestimate && imagesectors > availablesectors)
            {
                QString msg = tr("The image is larger than the device:\n"
                                 "  Image: at least %1 sectors\n"
                                 "  Available: %2 sectors\n  Sector Size: %3\n\n"
                                 "The end of the image will not be written, so the device "
                                 "will not hold a complete image.\n\nContinue Anyway?");
                msg = msg.arg(imagesectors).arg(availablesectors).arg(sectorsize);
                if (QMessageBox::warning(this, tr("Not enough available space!"), msg,
                                         QMessageBox::Ok, QMessageBox::Cancel) != QMessageBox::Ok)
                {
                    CloseHandle(hRawDisk);
                    hRawDisk = INVALID_HANDLE_VALUE;
                    locked.release();
                    endRun(tr("Write failed."));
                    return;
                }
            }
            if (numsectors > availablesectors)
            {
                bool datafound = false;
                bool tailchecked = imageTailHasData(image, availablesectors,
                                                    numsectors, &datafound);
                // Whole translatable sentences: tr() on text assembled at
                // runtime would never find a translation.
                QString msg = (!tailchecked)
                    ? tr("More space required than is available:\n  Required: %1 sectors\n"
                         "  Available: %2 sectors\n  Sector Size: %3\n\n"
                         "The extra space could not be checked for data, because the image "
                         "is compressed\n\nContinue Anyway?")
                    : (datafound)
                        ? tr("More space required than is available:\n  Required: %1 sectors\n"
                             "  Available: %2 sectors\n  Sector Size: %3\n\n"
                             "The extra space DOES appear to contain data\n\nContinue Anyway?")
                        : tr("More space required than is available:\n  Required: %1 sectors\n"
                             "  Available: %2 sectors\n  Sector Size: %3\n\n"
                             "The extra space does not appear to contain data\n\nContinue Anyway?");
                msg = msg.arg(numsectors).arg(availablesectors).arg(sectorsize);
                if(QMessageBox::warning(this, tr("Not enough available space!"),
                                        msg, QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok)
                {
                    // truncate the image at the device size...
                    numsectors = availablesectors;
                }
                else    // Cancel
                {
                    CloseHandle(hRawDisk);
                    hRawDisk = INVALID_HANDLE_VALUE;
                    locked.release();
                    endRun(tr("Write cancelled."));
                    return;
                }
            }

            // So no stale backup GPT survives at the end of the device.
            statusbar->showMessage(tr("Clearing old partition tables..."));
            QCoreApplication::processEvents();
            if (!wipePartitionTables(hRawDisk, sectorsize, availablesectors))
            {
                QMessageBox::critical(this, tr("Write Error"),
                    tr("Could not clear the existing partition tables on the device.")
                    + "\n\n" + tr("The device has been partially written and no longer holds "
                                  "a usable image. Write the image again before using it."));
                CloseHandle(hRawDisk);
                hRawDisk = INVALID_HANDLE_VALUE;
                locked.release();
                endRun(tr("Write failed."));
                return;
            }

            unsigned long long progresstotal = progressTotalFor(image, numsectors);
            int progshift = beginProgress(progresstotal, &lasti);
            // Otherwise "Clearing old partition tables" stays up until the
            // first throughput figure.
            statusbar->showMessage(tr("Writing..."));
            bool imagetruncated = false;
            for (i = 0ul; i < numsectors && status == STATUS_WRITING; i += TRANSFER_SECTORS)
            {
                unsigned long long chunk = (numsectors - i >= TRANSFER_SECTORS) ? TRANSFER_SECTORS : (numsectors - i);
                unsigned long long got = 0ull;
                sectorData = image.read(i, chunk, &got);
                if (sectorData == NULL)
                {
                    QMessageBox::critical(this, tr("Write Error"),
                        image.errorString()
                        + "\n\n" + tr("The device has been partially written and no longer holds "
                                      "a usable image. Write the image again before using it."));
                    CloseHandle(hRawDisk);
                    hRawDisk = INVALID_HANDLE_VALUE;
                    locked.release();
                    endRun(tr("Write failed."));
                    return;
                }
                if (got == 0ull)
                {
                    // The image ended exactly on the previous chunk.
                    delete[] sectorData;
                    sectorData = NULL;
                    numsectors = i;
                    break;
                }
                if (!writeSectorDataToHandle(hRawDisk, sectorData, i, got, sectorsize))
                {
                    // writeSectorDataToHandle has already reported what went
                    // wrong; this says what it leaves behind on the device.
                    QMessageBox::warning(this, tr("Write Error"),
                        tr("The device has been partially written and no longer holds "
                           "a usable image. Write the image again before using it."));
                    delete[] sectorData;
                    CloseHandle(hRawDisk);
                    locked.release();
                    sectorData = NULL;
                    hRawDisk = INVALID_HANDLE_VALUE;
                    endRun(tr("Write failed."));
                    return;
                }
                delete[] sectorData;
                sectorData = NULL;
                if (got < chunk)
                {
                    // Short read: the image ended inside this chunk.
                    numsectors = i + got;
                    progressbar->setValue(
                        (int)((numsectors > progresstotal ? progresstotal : numsectors) >> progshift));
                    QCoreApplication::processEvents();
                    break;
                }
                QCoreApplication::processEvents();
                // i is where this chunk started; the bar tracks its end.
                unsigned long long written = i + chunk;
                growProgressTotal(progressbar, written, numsectors, &progresstotal, &progshift);
                showThroughput(i, progresstotal, &lasti);
                progressbar->setValue(
                    (int)((written > progresstotal ? progresstotal : written) >> progshift));
                QCoreApplication::processEvents();
            }
            // Taken once: a Cancel or close during "Fixing GPT..." below must
            // not turn a finished write into one reported as merely "Done".
            // STATUS_WRITING survives the loop only if it completed; testing
            // for STATUS_CANCELED alone would miss STATUS_EXIT.
            const bool completed = (status == STATUS_WRITING);
            // Ask the stream whether image is left over: with an estimated
            // size the loop may have stopped at the device end. Also with a
            // known size (unless the user chose to truncate), since only
            // reading to the end makes a decoder check the trailing checksum
            // and, for xz, the index -- and, for gzip, bzip2 and zstd, whether
            // another stream follows.
            QString imagedamage;
            if (completed && (!image.sizeKnown() || numsectors == image.sizeInSectors()))
            {
                unsigned long long leftover = 0ull;
                char *extra = image.read(numsectors, 1ull, &leftover);
                if (extra == NULL)
                {
                    imagedamage = image.errorString();
                }
                delete[] extra;
                imagetruncated = (leftover > 0ull);
            }
            // The whole image went down: with only an estimated size the bar
            // can stand anywhere short of the end, so fill it before any
            // message about the result appears.
            if (completed && imagedamage.isEmpty() && !imagetruncated)
            {
                progressbar->setValue(progressbar->maximum());
                QCoreApplication::processEvents();
            }
            // Everything up to CloseHandle() below runs with the volumes still
            // locked; see acquireDeviceAndImage().
            flushDevice(hRawDisk);
            image.close();

            GptFixResult gptfix = GPT_FIX_DISABLED;
            QString gptdetail;
            // Ask before fixing anything: the fix rewrites the very header this
            // reads, so afterwards every image would look unaffected.
            GptRewriteRisk gptrisk = gptRewriteRisk(hRawDisk, sectorsize);
            // Only for wording the no-GPT message; read while the handle is open.
            bool mbr = deviceHasMbrTable(hRawDisk, sectorsize);
            if (fixGptCheckBox->isChecked() && completed && imagedamage.isEmpty())
            {
                statusbar->showMessage(tr("Fixing GPT..."));
                QCoreApplication::processEvents();
                gptfix = relocateBackupGPT(hRawDisk, sectorsize, availablesectors, &gptdetail);
                flushDevice(hRawDisk);
            }

            // Offline before releasing the locks so nothing is remounted, then
            // eject: the card should come out without Windows re-enumerating it.
            bool offline = setDiskOffline(hRawDisk, true);
            bool ejected = ejectDevice(hRawDisk);
            CloseHandle(hRawDisk);
            hRawDisk = INVALID_HANDLE_VALUE;
            locked.release();

            if (!completed)
            {
                passfail = false;
            }
            else if (!imagedamage.isEmpty())
            {
                QMessageBox::critical(this, tr("Write Error"),
                    imagedamage
                    + "\n\n" + tr("The device has been partially written and no longer holds "
                                  "a usable image. Write the image again before using it."));
                passfail = false;
            }
            else if (imagetruncated)
            {
                QMessageBox::critical(this, tr("Image truncated"),
                    tr("The image is larger than the device, so the end of it was not "
                       "written and the device does not hold a complete image.\n\n"
                       "This could only be detected once the device was full, because "
                       "the compressed image does not record its uncompressed size."));
                passfail = false;
            }
            // No GPT means nothing for Windows to "repair", fix or no fix:
            // wipePartitionTables() zeroed the first and last 34 sectors, so
            // no stale backup GPT survives.
            else if (gptfix == GPT_FIX_OK || gptfix == GPT_FIX_NOT_NEEDED
                     || gptfix == GPT_FIX_NO_GPT
                     || (gptrisk == GPT_RISK_NO_GPT && gptfix == GPT_FIX_DISABLED))
            {
                QString msg;
                if (gptfix == GPT_FIX_OK)
                {
                    msg = tr("Write successful.\n\nThe GPT now matches the device (%1), so "
                             "Windows has nothing to repair. Remove the device normally.")
                              .arg(gptdetail);
                }
                else if (gptfix == GPT_FIX_NO_GPT || gptrisk == GPT_RISK_NO_GPT)
                {
                    msg = mbr
                        ? tr("Write successful.\n\nThis image uses an MBR partition table, not "
                             "a GPT, so the Windows GPT rewrite bug cannot affect it. Remove "
                             "the device normally.")
                        : tr("Write successful.\n\nThis image has no partition table, so the "
                             "Windows GPT rewrite bug cannot affect it. Remove the device "
                             "normally.");
                }
                else
                {
                    msg = tr("Write successful.");
                }
                QMessageBox::information(this, tr("Write Successful"), msg);
            }
            else
            {
                QString state = (offline || ejected)
                    ? tr("The device is offline and ejected.")
                    : tr("The device could NOT be taken offline.");
                QString why = (gptfix == GPT_FIX_BAD_GPT)
                    ? tr("The GPT could not be fixed (%1).").arg(
                          gptdetail.isEmpty() ? tr("malformed GPT") : gptdetail)
                    : (gptfix == GPT_FIX_FAILED)
                        ? tr("Fixing the GPT failed (%1).").arg(
                              gptdetail.isEmpty() ? tr("write error") : gptdetail)
                        : tr("\"Fix GPT after write\" is off.");
                QString risk;
                if (gptrisk == GPT_RISK_AFFECTED)
                {
                    risk = tr("This image IS affected: it reserves space ahead of its first "
                              "partition, so a rescan points the primary table at the wrong "
                              "sectors. Windows still accepts the result; Linux does not, and "
                              "the device will not boot.");
                }
                else if (gptrisk == GPT_RISK_SAFE)
                {
                    risk = tr("This image is NOT affected: a rescan still rewrites the table, "
                              "but for this layout it writes the correct values. Removing the "
                              "device now keeps it identical to the image either way.");
                }
                else
                {
                    risk = tr("Whether this image is affected could not be determined. Assume "
                              "it is: a rescan can leave a table that Linux rejects and the "
                              "device will not boot.");
                }
                QMessageBox::warning(this, tr("Remove the device now"),
                    tr("Write successful, but the partition table is at risk.\n\n"
                       "%1 %2\n\n"
                       "%3\n\n"
                       "Remove the device NOW and do not re-insert it here. Put it straight "
                       "into the target hardware.").arg(why).arg(state).arg(risk));
            }
        }
        else if (!fileinfo.exists() || !fileinfo.isFile())
        {
            QMessageBox::critical(this, tr("File Error"), tr("The selected file does not exist."));
            passfail = false;
        }
        else if (!fileinfo.isReadable())
        {
            QMessageBox::critical(this, tr("File Error"), tr("You do not have permission to read the selected file."));
            passfail = false;
        }
        else if (fileinfo.size() == 0)
        {
            QMessageBox::critical(this, tr("File Error"), tr("The specified file contains no data."));
            passfail = false;
        }
        showProgress(false);
        statusbar->showMessage(tr("Done."));
        bCancel->setEnabled(false);
        if (passfail){
            statusbar->showMessage(tr("Write Successful."));
        }

    }
    else
    {
        QMessageBox::critical(this, tr("File Error"), tr("Please specify an image file to use."));
    }
    if (status == STATUS_EXIT)
    {
        close();
    }
    status = STATUS_IDLE;
    // Only after the reset: setReadWriteButtonState() keeps everything
    // disabled while a run is active.
    setReadWriteButtonState();
    elapsed_timer->stop();
}

// The image's directory, for GetDiskFreeSpaceEx; works for UNC paths as well
// as drive letters. It exists: the image file has already been created in it.
static QString volumeDirectoryFor(const QString &file)
{
    QString dir = QDir::toNativeSeparators(QFileInfo(file).absolutePath());
    // A UNC name is refused without a trailing backslash; a drive-letter path
    // accepts one either way, so both get one.
    if (!dir.endsWith(QChar('\\')))
    {
        dir += QChar('\\');
    }
    return dir;
}

static QString formatDeviceSize(unsigned long long bytes)
{
    // Card and stick capacities are quoted in powers of ten, so match that.
    static const char *units[] = { "KB", "MB", "GB", "TB" };
    double value = (double)bytes;
    int unit = -1;
    while (value >= 1000.0 && unit < 3)
    {
        value /= 1000.0;
        ++unit;
    }
    if (unit < 0)
    {
        return QString("%1 B").arg(bytes);
    }
    return QString("%1 %2").arg(value, 0, 'f', (value < 10.0) ? 1 : 0).arg(units[unit]);
}

// Lists the partitions, all checked, for the user to uncheck the ones to leave
// out; an empty selection is refused. Returns false if the user cancels.
bool MainWindow::choosePartitionsDialog(const QList<PartitionInfo> &partitions,
                                        unsigned long long sectorsize, int deviceID,
                                        QList<int> *excluded)
{
    QMap<unsigned long long, QString> driveLetters = driveLettersByOffset((ULONG)deviceID);
    // Windows' own partition numbers, falling back to slot + 1.
    QMap<unsigned long long, int> partitionNumbers;
    bool haveRealNumbers = diskPartitionNumbers(hRawDisk, sectorsize, &partitionNumbers);

    QDialog dialog(this);
    dialog.setWindowTitle(tr("Choose Partitions"));
    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    QLabel *label = new QLabel(
        tr("Select partitions to include in the Image."), &dialog);
    // Otherwise the label stretches the dialog to fit the sentence on one line.
    label->setWordWrap(true);
    layout->addWidget(label);
    dialog.setMinimumWidth(300);

    QListWidget *list = new QListWidget(&dialog);
    // Rows are in disk-position order, as listGptPartitions()/listMbrPartitions()
    // return them; the number shown is Windows' own (diskpart's) when available,
    // which need not follow that order.
    for (const PartitionInfo &p : partitions)
    {
        int number = haveRealNumbers ? partitionNumbers.value(p.firstSector, p.slot + 1)
                                      : p.slot + 1;
        QString sizeStr = formatDeviceSize(p.sectors * sectorsize);
        // A drive letter identifies a partition better than its name, which
        // is often blank (always, for MBR) or generic.
        QString label3 = driveLetters.value(p.firstSector * sectorsize, p.name);
        QString text = label3.isEmpty()
            ? tr("Partition %1 -- %2").arg(number).arg(sizeStr)
            : tr("Partition %1 -- %2 -- %3").arg(number).arg(sizeStr).arg(label3);
        QListWidgetItem *item = new QListWidgetItem(text, list);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Checked);
        item->setData(Qt::UserRole, p.slot);
    }
    layout->addWidget(list);

    QDialogButtonBox *buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    layout->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    for (;;)
    {
        if (dialog.exec() != QDialog::Accepted)
        {
            return false;
        }
        QList<int> excludeSlots;
        int checkedCount = 0;
        for (int i = 0; i < list->count(); ++i)
        {
            QListWidgetItem *item = list->item(i);
            if (item->checkState() == Qt::Checked)
            {
                ++checkedCount;
            }
            else
            {
                excludeSlots.append(item->data(Qt::UserRole).toInt());
            }
        }
        if (checkedCount == 0)
        {
            QMessageBox::warning(&dialog, tr("Choose Partitions"),
                tr("At least one partition must stay checked."));
            continue;
        }
        *excluded = excludeSlots;
        return true;
    }
}

void MainWindow::on_bRead_clicked()
{
    // Re-entrancy guard; see on_bWrite_clicked().
    if (status != STATUS_IDLE)
    {
        return;
    }
    QString myFile;
    if (!leFile->text().isEmpty())
    {
        myFile = leFile->text();
        // A relative name, "sub\foo.img" as much as "foo.img", goes in the
        // image directory rather than the working directory (usually the
        // install directory).
        if (QFileInfo(myFile).isRelative())
        {
            myFile = QDir::toNativeSeparators(QDir(myHomeDir).filePath(myFile));
        }
        const bool compressing = compressReadCheckBox->isChecked();
        const ImageSink::Format compressformat = readFormatFor(compressFormatComboBox->currentIndex());
        myFile = ImageSink::readTargetName(myFile, compressing, compressformat);
        // In step with myFile, or the overwrite prompt checks a different file
        // from the one getHandleOnFile truncates.
        QFileInfo fileinfo(myFile);
        if (myFile != leFile->text())
        {
            // Verify, Write and the hash controls all read the field, so it
            // must name the file actually written.
            leFile->setText(myFile);
            imageFileChanged();
        }
        // check whether source and target device is the same...
        if (fileIsOnSelectedDevice(myFile))
        {
            QMessageBox::critical(this, tr("Write Error"), tr("Image file cannot be located on the target device."));
            return;
        }
        int deviceID = selectedDeviceID();
        if (deviceID < 0)
        {
            QMessageBox::critical(this, tr("Read Error"), tr("Please select a source device."));
            return;
        }
        // confirm overwrite if the dest. file already exists
        if (fileinfo.exists())
        {
            if (QMessageBox::warning(this, tr("Confirm Overwrite"), tr("Are you sure you want to overwrite the specified file?"),
                                     QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::No)
            {
                return;
            }
        }
        bCancel->setEnabled(true);
        bWrite->setEnabled(false);
        bRead->setEnabled(false);
        bVerify->setEnabled(false);
        bCheckGpt->setEnabled(false);
        status = STATUS_READING;
        // The file is about to be truncated: a digest of what was there
        // before must not stay on screen as its checksum.
        updateHashControls();
        showProgress(true);
        unsigned long long i, lasti, numsectors, filesize, spaceneeded = 0ull;
        // Locked as in acquireDeviceAndImage().
        LockedVolumes locked;
        if (!locked.lockAll(deviceID))
        {
            endRun(tr("Read failed."));
            return;
        }
        // Open and size the device before the image file: getHandleOnFile
        // uses CREATE_ALWAYS, which would truncate the user's existing file
        // even if the device then proved unreadable.
        hRawDisk = getHandleOnDevice(deviceID, GENERIC_READ);
        if (hRawDisk == INVALID_HANDLE_VALUE)
        {
            locked.release();
            endRun(tr("Read failed."));
            return;
        }
        numsectors = getNumberOfSectors(hRawDisk, &sectorsize);
        if (!numsectors)
        {
            // A card pulled from its reader reports zero.
            CloseHandle(hRawDisk);
            hRawDisk = INVALID_HANDLE_VALUE;
            locked.release();
            QMessageBox::critical(this, tr("Device Error"),
                tr("The device reports a size of zero. If it is a card reader, "
                   "the card may have been removed."));
            endRun(tr("Read failed."));
            return;
        }
        // Shrink to the table and partitions, repacked 1MiB-aligned (see
        // planGptShrink()). No usable table, or nothing to gain, means a full
        // read, silently: neither is an error.
        bool shrinkPlanned = false;
        PartitionShrinkPlan shrinkPlan;
        if (shrinkOnReadCheckBox->isChecked() || choosePartitionsCheckBox->isChecked())
        {
            QString detail;
            unsigned long long alignsectors = (sectorsize >= 1048576ull) ? 1ull : (1048576ull / sectorsize);
            if (alignsectors == 0ull)
            {
                alignsectors = 1ull;
            }

            // Choosing partitions implies shrinking: only the shrink plan's
            // exclude filter can leave a partition out.
            QList<int> excludeSlots;
            bool haveSelection = false;
            bool selectionIsGpt = false;
            if (choosePartitionsCheckBox->isChecked())
            {
                QList<PartitionInfo> partitions;
                QString listdetail;
                selectionIsGpt = listGptPartitions(hRawDisk, sectorsize, numsectors, &partitions, &listdetail);
                if (!selectionIsGpt)
                {
                    listMbrPartitions(hRawDisk, sectorsize, numsectors, &partitions, &listdetail);
                }
                if (partitions.isEmpty())
                {
                    QMessageBox::information(this, tr("Choose Partitions"),
                        tr("No partition table was found on the device, so "
                           "there is nothing to choose from. The whole "
                           "device will be read."));
                }
                else if (!choosePartitionsDialog(partitions, sectorsize, deviceID, &excludeSlots))
                {
                    CloseHandle(hRawDisk);
                    hRawDisk = INVALID_HANDLE_VALUE;
                    locked.release();
                    endRun(tr("Read canceled."));
                    return;
                }
                else
                {
                    haveSelection = true;
                }
            }

            // With a selection, plan only against the table type it was
            // listed from: excludeSlots indexes that table's slots.
            bool planned = haveSelection
                ? (selectionIsGpt
                       ? planGptShrink(hRawDisk, sectorsize, numsectors, alignsectors, &shrinkPlan, &detail, &excludeSlots)
                       : planMbrShrink(hRawDisk, sectorsize, numsectors, alignsectors, &shrinkPlan, &detail, &excludeSlots))
                : (planGptShrink(hRawDisk, sectorsize, numsectors, alignsectors, &shrinkPlan, &detail)
                       || planMbrShrink(hRawDisk, sectorsize, numsectors, alignsectors, &shrinkPlan, &detail));
            if (planned)
            {
                shrinkPlanned = true;
                numsectors = shrinkPlan.totalsectors;
            }
            else if (haveSelection)
            {
                // Unlike a plain shrink, falling back to a full read would
                // silently include partitions the user excluded.
                CloseHandle(hRawDisk);
                hRawDisk = INVALID_HANDLE_VALUE;
                locked.release();
                QMessageBox::critical(this, tr("Read Error"), detail);
                endRun(tr("Read failed."));
                return;
            }
        }
        ImageSink sink;
        // Failure cleanup for either output backend.
        auto failRead = [&]()
        {
            CloseHandle(hRawDisk);
            hRawDisk = INVALID_HANDLE_VALUE;
            locked.release();
            if (compressing)
            {
                sink.abort();
            }
            else if (hFile != INVALID_HANDLE_VALUE)
            {
                CloseHandle(hFile);
                hFile = INVALID_HANDLE_VALUE;
            }
            endRun(tr("Read failed."));
        };

        if (compressing)
        {
            if (!sink.open(myFile, compressformat))
            {
                QString error = sink.errorString();
                CloseHandle(hRawDisk);
                hRawDisk = INVALID_HANDLE_VALUE;
                locked.release();
                QMessageBox::critical(this, tr("Read Error"), error);
                endRun(tr("Read failed."));
                return;
            }
            // Compressed size is unknown up front, so ask for the raw size:
            // running out of space mid-stream is the failure to avoid.
            spaceneeded = numsectors * sectorsize;
        }
        else
        {
            hFile = getHandleOnFile((LPCWSTR)myFile.utf16(), GENERIC_WRITE);
            if (hFile == INVALID_HANDLE_VALUE)
            {
                CloseHandle(hRawDisk);
                hRawDisk = INVALID_HANDLE_VALUE;
                locked.release();
                endRun(tr("Read failed."));
                return;
            }
            filesize = getFileSizeInSectors(hFile, sectorsize);
            spaceneeded = (filesize >= numsectors) ? 0ull
                : (unsigned long long)(numsectors - filesize) * (unsigned long long)(sectorsize);
        }
        if (!spaceAvailable(volumeDirectoryFor(myFile), spaceneeded))
        {
            QMessageBox::critical(this, tr("Write Error"), tr("Disk is not large enough for the specified image."));
            sectorData = NULL;
            failRead();
            return;
        }
        statusbar->showMessage(tr("Reading..."));
        const int progshift = beginProgress(numsectors, &lasti);
        // dstpos: image sector the next write starts at. A compressed sink
        // cannot seek, so every write, alignment-gap zeros included, must come
        // in image order, as the shrink plans' ranges are.
        unsigned long long dstpos = 0ull;
        auto writeOut = [&](const char *data, unsigned long long sectors) -> bool
        {
            bool ok = compressing
                ? sink.write(data, sectors * sectorsize)
                : writeSectorDataToHandle(hFile, (char *)data, dstpos, sectors, sectorsize);
            dstpos += sectors;
            return ok;
        };
        auto writeZeros = [&](unsigned long long sectors) -> bool
        {
            if (sectors == 0ull)
            {
                return true;
            }
            QByteArray zeros((size_t)(sectors * sectorsize), 0);
            return writeOut(zeros.constData(), sectors);
        };
        // For the header and backup regions, which come as one buffer: a raw
        // write's length goes through WriteFile's DWORD, and headerregion can
        // exceed 4GiB (it is bounded only by devicesectors / 2).
        auto writeOutChunked = [&](const char *data, unsigned long long sectors) -> bool
        {
            for (unsigned long long i = 0ull; i < sectors; i += TRANSFER_SECTORS)
            {
                unsigned long long chunk = (sectors - i >= TRANSFER_SECTORS) ? TRANSFER_SECTORS : (sectors - i);
                if (!writeOut(data + i * sectorsize, chunk))
                {
                    return false;
                }
            }
            return true;
        };

        if (shrinkPlanned)
        {
            if (!writeOutChunked(shrinkPlan.headerregion.constData(), shrinkPlan.headersectors))
            {
                failRead();
                return;
            }
            progressbar->setValue((int)(dstpos >> progshift));
        }
        QList<ShrinkCopyRange> ranges = shrinkPlanned ? shrinkPlan.ranges
            : QList<ShrinkCopyRange>{ ShrinkCopyRange{0ull, 0ull, numsectors} };
        for (const ShrinkCopyRange &range : ranges)
        {
            if (status != STATUS_READING)
            {
                break;
            }
            if (range.dstfirst > dstpos && !writeZeros(range.dstfirst - dstpos))
            {
                failRead();
                return;
            }
            for (i = 0ull; i < range.length && status == STATUS_READING; i += TRANSFER_SECTORS)
            {
                unsigned long long chunk = (range.length - i >= TRANSFER_SECTORS) ? TRANSFER_SECTORS : (range.length - i);
                sectorData = readSectorDataFromHandle(hRawDisk, range.srcfirst + i, chunk, sectorsize);
                if (sectorData == NULL)
                {
                    failRead();
                    return;
                }
                if (!writeOut(sectorData, chunk))
                {
                    delete[] sectorData;
                    sectorData = NULL;
                    failRead();
                    return;
                }
                delete[] sectorData;
                sectorData = NULL;
                showThroughput(dstpos, numsectors, &lasti);
                progressbar->setValue((int)((dstpos > numsectors ? numsectors : dstpos) >> progshift));
                QCoreApplication::processEvents();
            }
        }
        if (shrinkPlanned && status == STATUS_READING && shrinkPlan.backupsectors > 0ull)
        {
            // Precomputed by planGptShrink() and written in sequence, so it
            // works for a non-seekable sink. MBR plans have none.
            if (!writeOutChunked(shrinkPlan.backupregion.constData(), shrinkPlan.backupsectors))
            {
                failRead();
                return;
            }
        }
        if (compressing)
        {
            if (status == STATUS_READING && !sink.finish())
            {
                QString error = sink.errorString();
                CloseHandle(hRawDisk);
                hRawDisk = INVALID_HANDLE_VALUE;
                locked.release();
                QMessageBox::critical(this, tr("Read Error"), error);
                endRun(tr("Read failed."));
                return;
            }
            // Closes the file on the canceled path, where finish() was not
            // called; a no-op after finish().
            sink.abort();
        }
        else
        {
            CloseHandle(hFile);
            hFile = INVALID_HANDLE_VALUE;
        }
        CloseHandle(hRawDisk);
        hRawDisk = INVALID_HANDLE_VALUE;
        locked.release();
        showProgress(false);
        statusbar->showMessage(tr("Done."));
        bCancel->setEnabled(false);
        // Cancel or window close (see on_bWrite_clicked()): the image is
        // incomplete, and a compressed one was never finished.
        if (status != STATUS_READING){
            QMessageBox::information(this, tr("Complete"), tr("Read Canceled."));
        } else {
            QMessageBox::information(this, tr("Complete"), tr("Read Successful."));

        }
        updateHashControls();
    }
    else
    {
        QMessageBox::critical(this, tr("File Info"), tr("Please specify a file to save data to."));
    }
    if (status == STATUS_EXIT)
    {
        close();
    }
    status = STATUS_IDLE;
    // After the reset; see on_bWrite_clicked().
    setReadWriteButtonState();
    elapsed_timer->stop();
}

// Verify image with device
void MainWindow::on_bVerify_clicked()
{
    // Re-entrancy guard; see on_bWrite_clicked().
    if (status != STATUS_IDLE)
    {
        return;
    }
    bool passfail = true;
    bool verifyreported = false;
    if (!leFile->text().isEmpty())
    {
        QFileInfo fileinfo(leFile->text());
        if (fileinfo.exists() && fileinfo.isFile() &&
                fileinfo.isReadable() && (fileinfo.size() > 0) )
        {
            if (fileIsOnSelectedDevice(leFile->text()))
            {
                QMessageBox::critical(this, tr("Verify Error"), tr("Image file cannot be located on the target device."));
                return;
            }
            int deviceID = selectedDeviceID();
            if (deviceID < 0)
            {
                QMessageBox::critical(this, tr("Verify Error"), tr("Please select a device to verify against."));
                return;
            }
            status = STATUS_VERIFYING;
            showProgress(true);
            bCancel->setEnabled(true);
            bWrite->setEnabled(false);
            bRead->setEnabled(false);
            bVerify->setEnabled(false);
            bCheckGpt->setEnabled(false);
            unsigned long long i, lasti, availablesectors, numsectors, result;
            LockedVolumes locked;
            ImageSource image;
            if (!acquireDeviceAndImage(deviceID, locked, image, &availablesectors,
                                       tr("Verify Error"), tr("Verify failed.")))
            {
                return;
            }
            // Size estimate handled as in on_bWrite_clicked().
            const bool sizeisestimate = !image.sizeKnown();
            const unsigned long long imagesectors = image.sizeInSectors();
            numsectors = sizeisestimate ? availablesectors : imagesectors;
            if (!numsectors)
            {
                // An empty file, or a compressed stream holding nothing.
                QMessageBox::critical(this, tr("File Error"),
                                      tr("The specified file contains no data."));
                CloseHandle(hRawDisk);
                hRawDisk = INVALID_HANDLE_VALUE;
                locked.release();
                endRun(tr("Verify failed."));
                return;
            }
            if (sizeisestimate && imagesectors > availablesectors)
            {
                QString msg = tr("The image is larger than the device:\n"
                                 "  Image: at least %1 sectors\n"
                                 "  Device: %2 sectors\n  Sector Size: %3\n\n"
                                 "Only the part that fits can be compared.\n\n"
                                 "Continue Anyway?");
                msg = msg.arg(imagesectors).arg(availablesectors).arg(sectorsize);
                if (QMessageBox::warning(this, tr("Size Mismatch!"), msg,
                                         QMessageBox::Ok, QMessageBox::Cancel) != QMessageBox::Ok)
                {
                    CloseHandle(hRawDisk);
                    hRawDisk = INVALID_HANDLE_VALUE;
                    locked.release();
                    endRun(tr("Verify failed."));
                    return;
                }
            }
            if (numsectors > availablesectors)
            {
                bool datafound = false;
                bool tailchecked = imageTailHasData(image, availablesectors,
                                                    numsectors, &datafound);
                QString msg = (!tailchecked)
                    ? tr("Size of image larger than device:\n  Image: %1 sectors\n"
                         "  Device: %2 sectors\n  Sector Size: %3\n\n"
                         "The extra space could not be checked for data, because the image "
                         "is compressed\n\nContinue Anyway?")
                    : (datafound)
                        ? tr("Size of image larger than device:\n  Image: %1 sectors\n"
                             "  Device: %2 sectors\n  Sector Size: %3\n\n"
                             "The extra space DOES appear to contain data\n\nContinue Anyway?")
                        : tr("Size of image larger than device:\n  Image: %1 sectors\n"
                             "  Device: %2 sectors\n  Sector Size: %3\n\n"
                             "The extra space does not appear to contain data\n\nContinue Anyway?");
                msg = msg.arg(numsectors).arg(availablesectors).arg(sectorsize);
                if(QMessageBox::warning(this, tr("Size Mismatch!"),
                                        msg, QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok)
                {
                    // truncate the image at the device size...
                    numsectors = availablesectors;
                }
                else    // Cancel
                {
                    CloseHandle(hRawDisk);
                    hRawDisk = INVALID_HANDLE_VALUE;
                    locked.release();
                    endRun(tr("Verify cancelled."));
                    return;
                }
            }
            // "Fix GPT after write" rewrites sectors at both ends by design
            // (see gptOwnedSectors()); differences there are not failures.
            unsigned long long gptfrontend = 0ull, gpttailstart = 0ull;
            bool gptknown = gptOwnedSectors(hRawDisk, sectorsize, availablesectors,
                                            &gptfrontend, &gpttailstart);
            // It also zeroes the image's stale backup GPT, located from the
            // image's own header in the first chunk.
            unsigned long long stalefirst = 0ull, stalelast = 0ull;
            bool staleknown = false;
            bool gptonly = false;
            bool gptrepaired = false, gptleftdamaged = false;
            GptPrimaryState gptstate = GPT_PRIMARY_UNKNOWN;

            unsigned long long progresstotal = progressTotalFor(image, numsectors);
            int progshift = beginProgress(progresstotal, &lasti);
            statusbar->showMessage(tr("Verifying..."));
            for (i = 0ul; i < numsectors && status == STATUS_VERIFYING; i += TRANSFER_SECTORS)
            {
                unsigned long long got = 0ull;
                sectorData = image.read(i, (numsectors - i >= TRANSFER_SECTORS) ? TRANSFER_SECTORS:(numsectors - i), &got);
                if (sectorData == NULL)
                {
                    QMessageBox::critical(this, tr("Verify Error"), image.errorString());
                    CloseHandle(hRawDisk);
                    hRawDisk = INVALID_HANDLE_VALUE;
                    locked.release();
                    endRun(tr("Verify failed."));
                    return;
                }
                if (got == 0ull)
                {
                    // The image ended on the previous chunk; there is nothing
                    // left to compare.
                    delete[] sectorData;
                    sectorData = NULL;
                    numsectors = i;
                    break;
                }
                sectorData2 = readSectorDataFromHandle(hRawDisk, i, got, sectorsize);
                if (sectorData2 == NULL)
                {
                    // The device could not be read. That is not the image
                    // failing to match, which is what the other message says.
                    QMessageBox::critical(this, tr("Verify Error"),
                        tr("The device could not be read at sector %1.").arg(i));
                    delete[] sectorData;
                    sectorData = NULL;
                    CloseHandle(hRawDisk);
                    hRawDisk = INVALID_HANDLE_VALUE;
                    locked.release();
                    endRun(tr("Verify failed."));
                    return;
                }
                if (i == 0ull && got >= 2ull)
                {
                    staleknown = gptImageBackupRange(
                        (const unsigned char *)(sectorData + sectorsize),
                        sectorsize, &stalefirst, &stalelast);
                }
                unsigned long chunk = (unsigned long)got;
                result = memcmp(sectorData, sectorData2, chunk * sectorsize);
                if (result)
                {
                    // Find the first difference the GPT fix cannot account for.
                    bool bad = false;
                    unsigned long long badsector = i;
                    for (unsigned long s = 0ul; s < chunk && !bad; ++s)
                    {
                        if (memcmp(sectorData + s * sectorsize,
                                   sectorData2 + s * sectorsize, sectorsize) == 0)
                        {
                            continue;
                        }
                        unsigned long long lba = i + s;
                        // LBA 0 is the boot sector: the fix only rewrites the
                        // protective entry's size field (bytes 458-461), so
                        // any other difference there is a real one.
                        const char *imgsec = sectorData + s * sectorsize;
                        const char *devsec = sectorData2 + s * sectorsize;
                        bool fixable = (lba != 0ull)
                            || (memcmp(imgsec, devsec, 458) == 0
                                && memcmp(imgsec + 462, devsec + 462, sectorsize - 462) == 0);
                        if (gptknown && fixable && (lba < gptfrontend || lba >= gpttailstart))
                        {
                            gptonly = true;
                            continue;
                        }
                        if (staleknown && lba >= stalefirst && lba <= stalelast)
                        {
                            gptonly = true;
                            continue;
                        }
                        badsector = lba;
                        bad = true;
                    }
                    if (bad)
                    {
                        QMessageBox::critical(this, tr("Verify Failure"),
                            tr("Verification failed at sector: %1").arg(badsector));
                        passfail = false;
                        break;
                    }
                }
                // i is where this chunk started; the bar tracks what is done.
                unsigned long long checked = i + TRANSFER_SECTORS;
                growProgressTotal(progressbar, checked, numsectors, &progresstotal, &progshift);
                showThroughput(i, progresstotal, &lasti);
                delete[] sectorData;
                delete[] sectorData2;
                sectorData = NULL;
                sectorData2 = NULL;
                progressbar->setValue(
                    (int)((checked > progresstotal ? progresstotal : checked) >> progshift));
                QCoreApplication::processEvents();
            }
            // As after a write; comparing only the part that fits is not a
            // successful verify.
            // Also with a known size (unless the user chose to truncate): only
            // reading to the end of a compressed stream makes the decoder
            // check its trailing checksum and, for xz, the index.
            bool imageunchecked = false;
            if (status == STATUS_VERIFYING && passfail
                && (!image.sizeKnown() || numsectors == image.sizeInSectors()))
            {
                unsigned long long leftover = 0ull;
                char *extra = image.read(numsectors, 1ull, &leftover);
                if (extra == NULL)
                {
                    QMessageBox::critical(this, tr("Verify Error"), image.errorString());
                    passfail = false;
                    verifyreported = true;
                }
                delete[] extra;
                imageunchecked = (leftover > 0ull);
            }
            // As after a write: all of it compared, so the bar is full.
            if (status == STATUS_VERIFYING && passfail && !imageunchecked)
            {
                progressbar->setValue(progressbar->maximum());
                QCoreApplication::processEvents();
            }
            // Data can match while the table is ruined: a Windows rescan gets
            // the primary header wrong, and the compare forgives GPT sectors.
            // Check the table while the device is still held.
            if (status == STATUS_VERIFYING && passfail)
            {
                gptstate = gptPrimaryState(hRawDisk, sectorsize, availablesectors);
            }
            if (gptstate == GPT_PRIMARY_BROKEN)
            {
                if (offerGptRepair(hRawDisk, sectorsize, availablesectors,
                        tr("The device holds the image correctly, but its partition "
                           "table is broken:")))
                {
                    gptrepaired = true;
                }
                else
                {
                    gptleftdamaged = true;
                }
            }
            // Offline and eject before unlocking, as after a write, so a
            // verify cannot provoke the rescan the write avoided.
            bool offline = setDiskOffline(hRawDisk, true);
            bool ejected = ejectDevice(hRawDisk);
            CloseHandle(hRawDisk);
            locked.release();
            image.close();
            delete[] sectorData;
            delete[] sectorData2;
            sectorData = NULL;
            sectorData2 = NULL;
            hRawDisk = INVALID_HANDLE_VALUE;
            // Canceled or window closed: only part of the image was compared.
            if (status != STATUS_VERIFYING){
                passfail = false;
            }
            else if (imageunchecked)
            {
                QMessageBox::critical(this, tr("Image larger than device"),
                    tr("The image is larger than the device, so only the part that fits "
                       "could be compared. Everything compared matched, but the device "
                       "does not hold a complete image.\n\n"
                       "This could only be detected at the end of the device, because "
                       "the compressed image does not record its uncompressed size."));
                passfail = false;
                verifyreported = true;
            }
            else if (gptleftdamaged)
            {
                QMessageBox::warning(this, tr("Partition table damaged"),
                    tr("The device holds the image correctly, but its partition table is "
                       "still broken. Write the image again with \"Fix GPT after write\" "
                       "ticked, or run the verify again and accept the repair."));
                verifyreported = true;
            }
            else if (passfail)
            {
                QString msg = tr("Verify Successful.");
                if (gptrepaired)
                {
                    msg = tr("Verify Successful.\n\nThe device's partition table was "
                             "damaged and has been repaired.");
                }
                else if (gptonly && gptstate == GPT_PRIMARY_OK)
                {
                    msg = tr("Verify Successful.\n\nThe image and the device differ only "
                             "in the GPT, and the GPT on the device is valid.");
                }
                else if (gptonly)
                {
                    // Not broken, but not confirmed good either -- no GPT to
                    // check, or one this cannot judge. Do not claim it is valid.
                    msg = tr("Verify Successful.\n\nThe image and the device differ only "
                             "in the GPT.");
                }
                msg += (offline || ejected)
                    ? tr("\n\nThe device has been ejected. Remove it now.")
                    : tr("\n\nThe device could NOT be taken offline automatically.");
                QMessageBox::information(this, tr("Complete"), msg);
                verifyreported = true;
            }

        }
        else if (!fileinfo.exists() || !fileinfo.isFile())
        {
            QMessageBox::critical(this, tr("File Error"), tr("The selected file does not exist."));
            passfail = false;
        }
        else if (!fileinfo.isReadable())
        {
            QMessageBox::critical(this, tr("File Error"), tr("You do not have permission to read the selected file."));
            passfail = false;
        }
        else if (fileinfo.size() == 0)
        {
            QMessageBox::critical(this, tr("File Error"), tr("The specified file contains no data."));
            passfail = false;
        }
        showProgress(false);
        statusbar->showMessage(tr("Done."));
        bCancel->setEnabled(false);
        if (passfail && !verifyreported){
            QMessageBox::information(this, tr("Complete"), tr("Verify Successful."));
        }
    }
    else
    {
        QMessageBox::critical(this, tr("File Error"), tr("Please specify an image file to use."));
    }
    if (status == STATUS_EXIT)
    {
        close();
    }
    status = STATUS_IDLE;
    // After the reset; see on_bWrite_clicked().
    setReadWriteButtonState();
    elapsed_timer->stop();
}

// Delay before a click-triggered scan blocks the UI, so the control can finish
// drawing; see on_showAllDevicesCheckBox_toggled().
static const int SCAN_SETTLE_MS = 250;

// Rescan behind a status message and wait cursor: after the window appears,
// as the device list opens, and when "Show all devices" changes.
void MainWindow::rescanDevices()
{
    // Never mid-run (the device list stays clickable during one): the scan
    // queries every disk and blocks the transfer loop while it does.
    if (status != STATUS_IDLE)
    {
        return;
    }
    statusbar->showMessage(tr("Scanning disks..."));
    // Spinning a disk up can freeze the window for seconds.
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    // Paint the message, cursor and the clicked control now: getLogicalDrives()
    // blocks this thread, so anything merely posted would appear only after.
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

    getLogicalDrives();
    QApplication::restoreOverrideCursor();
    statusbar->clearMessage();
}

// Fills cboxDevice from physical disks rather than drive letters, so a card
// with no volume Windows has mounted still appears.
void MainWindow::getLogicalDrives()
{
    // Keep the user's selection across a refresh, since device arrival and the
    // "show all devices" toggle both rebuild the list underneath them.
    int previous = selectedDeviceID();

    QList<PhysicalDevice> devices = enumeratePhysicalDevices(showAllDevicesCheckBox->isChecked());

    // Rebuilding closes an open dropdown and resets the selection, so skip it
    // when nothing changed.
    QString signature;
    for (int i = 0; i < devices.size(); ++i)
    {
        signature += QString("%1:%2:%3;").arg(devices.at(i).deviceNumber)
                         .arg(devices.at(i).letters).arg(devices.at(i).sizeBytes);
    }
    if (signature == deviceSignature && cboxDevice->count() == devices.size())
    {
        return;
    }
    deviceSignature = signature;

    cboxDevice->clear();
    for (int i = 0; i < devices.size(); ++i)
    {
        const PhysicalDevice &dev = devices.at(i);
        // The disk number always (it names the device in Disk Management and
        // \\.\PhysicalDriveN), then any drive letters, as a separate bracket
        // so no new translatable string is needed.
        QString label = tr("[Disk %1]").arg(dev.deviceNumber);
        if (!dev.letters.isEmpty())
        {
            label += QString(" [%1]").arg(dev.letters);
        }
        cboxDevice->addItem(QString("%1 %2 - %3").arg(label)
                                .arg(formatDeviceSize(dev.sizeBytes))
                                .arg(dev.description),
                            (qulonglong)dev.deviceNumber);
        // For on_bWrite_clicked() to check the device it opens is this one.
        cboxDevice->setItemData(cboxDevice->count() - 1, (qulonglong)dev.sizeBytes,
                                Qt::UserRole + 1);
    }

    // The popup is otherwise as narrow as the closed box, which elides the very
    // names it exists to tell apart.
    QFontMetrics metrics(cboxDevice->view()->font());
    int widest = 0;
    for (int i = 0; i < cboxDevice->count(); ++i)
    {
        widest = qMax(widest, metrics.horizontalAdvance(cboxDevice->itemText(i)));
    }
    cboxDevice->view()->setMinimumWidth(widest + 2 * cboxDevice->view()->frameWidth()
                                        + QApplication::style()->pixelMetric(QStyle::PM_ScrollBarExtent));

    int restore = (previous >= 0) ? cboxDevice->findData((qulonglong)previous) : -1;
    cboxDevice->setCurrentIndex((restore >= 0) ? restore : 0);
    setReadWriteButtonState();
}

// Physical disk number of the device the user picked, or -1 if the list is empty.
int MainWindow::selectedDeviceID()
{
    if (cboxDevice->currentIndex() < 0)
    {
        return -1;
    }
    bool ok = false;
    int id = cboxDevice->currentData().toInt(&ok);
    return ok ? id : -1;
}

// True if the image file sits on a volume of the target disk, which would mean
// reading or writing it over itself.
bool MainWindow::fileIsOnSelectedDevice(const QString &file)
{
    int deviceID = selectedDeviceID();
    if (deviceID < 0 || file.isEmpty())
    {
        return false;
    }
    return pathIsOnDisk(file, (ULONG)deviceID);
}

void MainWindow::on_showAllDevicesCheckBox_toggled(bool)
{
    // Via rescanDevices(): this is the scan most likely to hit spun-down
    // fixed disks. Delayed by SCAN_SETTLE_MS, not 0: the Windows style
    // animates the tick only while the event loop runs, so an immediate scan
    // leaves the box drawn unticked for the whole wait.
    QTimer::singleShot(SCAN_SETTLE_MS, this, [this]() { rescanDevices(); });
}

void MainWindow::on_compressReadCheckBox_toggled(bool checked)
{
    compressFormatComboBox->setEnabled(checked);
}

void MainWindow::on_choosePartitionsCheckBox_toggled(bool checked)
{
    // A partition selection needs the shrink plan, so force "Shrink image on
    // Read" on and lock it while this is checked; unchecking puts back what
    // the user had.
    if (checked)
    {
        myShrinkBeforeChoose = shrinkOnReadCheckBox->isChecked();
        shrinkOnReadCheckBox->setChecked(true);
    }
    else
    {
        shrinkOnReadCheckBox->setChecked(myShrinkBeforeChoose);
    }
    shrinkOnReadCheckBox->setEnabled(!checked);
}

// Rebuilds the device list on WM_DEVICECHANGE arrival/removal.
// Adapted from http://www.known-issues.net/qt/qt-detect-event-windows.html
// Files dropped on the window fill in the image field. Qt's drag and drop is
// OLE, which Windows blocks from an unelevated source (Explorer) into an
// elevated window -- the "no" cursor everywhere. The older WM_DROPFILES route
// can be let through: accept files on the window, remove the OLE drop target
// so OLE falls back to it, and let past the three messages a drop is made of
// (WM_COPYGLOBALDATA, 0x0049, is undeclared). Nothing may call setAcceptDrops:
// Qt would register an OLE target again and drops would go back to failing.
void MainWindow::acceptDroppedFiles()
{
    leFile->setAcceptDrops(false);   // QLineEdit takes text drops by default
    HWND hwnd = (HWND)winId();
    RevokeDragDrop(hwnd);
    DragAcceptFiles(hwnd, TRUE);
    ChangeWindowMessageFilterEx(hwnd, WM_DROPFILES, MSGFLT_ALLOW, NULL);
    ChangeWindowMessageFilterEx(hwnd, WM_COPYDATA, MSGFLT_ALLOW, NULL);
    ChangeWindowMessageFilterEx(hwnd, 0x0049, MSGFLT_ALLOW, NULL);
}

bool MainWindow::nativeEvent(const QByteArray &type, void *vMsg, qintptr *result)
{
    Q_UNUSED(type);
    MSG *msg = (MSG*)vMsg;
    if (msg->message == WM_DROPFILES)
    {
        HDROP drop = (HDROP)msg->wParam;
        // Not mid-run: the image field names the file a run's results refer to.
        if (status == STATUS_IDLE && DragQueryFileW(drop, 0xFFFFFFFF, NULL, 0) > 0)
        {
            const UINT len = DragQueryFileW(drop, 0, NULL, 0);
            std::wstring name(len + 1, L'\0');
            DragQueryFileW(drop, 0, &name[0], len + 1);
            const QString file = QString::fromWCharArray(name.c_str(), (int)len);
            if (QFileInfo(file).isFile())
            {
                leFile->setText(file);
                // What typing a name and leaving the field does.
                on_leFile_editingFinished();
                activateWindow();
            }
        }
        DragFinish(drop);
        *result = 0;
        return true;
    }
    if(msg->message == WM_DEVICECHANGE)
    {
        switch(msg->wParam)
        {
        case DBT_DEVICEARRIVAL:
        case DBT_DEVICEREMOVECOMPLETE:
            // The list is rebuilt wholesale rather than patched by drive
            // letter: a device worth listing need not carry a letter at all,
            // and the broadcast only names one when it does.
            if (status == STATUS_IDLE)
            {
                getLogicalDrives();
            }
            break;
        } // skip the rest
    } // end of if msg->message
    *result = 0; //get rid of obnoxious compiler warning
    return false; // let qt handle the rest
}

void MainWindow::updateHashControls()
{
    bHashCopy->setEnabled(false);
    hashLabel->clear();
    // Hidden while empty and, unlike the progress bar, not keeping its
    // space, so the group closes up.
    hashLabel->setVisible(false);

    bHashGen->setEnabled(status == STATUS_IDLE
                         && hashableFile(leFile->text(), cboxHashType->currentIndex()));

    // generateHash() enables Copy once a digest exists.
}

// Everything that depends on which image file is named, for every way of
// naming one (typed, browsed to, dropped, command line).
void MainWindow::imageFileChanged()
{
    defaultHashTypeForFile();
    setReadWriteButtonState();
    updateHashControls();
}

void MainWindow::on_cboxHashType_IdxChg()
{
    updateHashControls();
}

void MainWindow::on_bHashGen_clicked()
{
    // See setReadWriteButtonState().
    if (status != STATUS_IDLE)
    {
        return;
    }
    generateHash(leFile->text(), cboxHashType->currentData().toInt());

}
