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
 *  GPT repair, device enumeration, volume locking                    *
 *  https://github.com/peacepenguin/windiskimager                   *
 **********************************************************************/

#ifndef WINVER
#define WINVER 0x0601
#endif

#include <QtWidgets>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#include <string>
#include <windows.h>
#include <winioctl.h>
#include "disk.h"
#include "mainwindow.h"

// Report a Win32 failure with the system's description of it. `message`
// carries %1 for the error code and %2 for the description (or the next two
// free placeholders). GetLastError() is read first because FormatMessageW and
// the dialog can overwrite it.
static void reportWin32Error(const QString &title, const QString &message)
{
    DWORD code = GetLastError();
    wchar_t *text = NULL;
    FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
                   NULL, code, 0, (LPWSTR)&text, 0, NULL);
    QMessageBox::critical(MainWindow::getInstanceIfAvailable(), title,
                          message.arg(code)
                                 .arg(text ? QString::fromUtf16((const char16_t *)text)
                                           : QString()));
    LocalFree(text);
}

HANDLE getHandleOnFile(LPCWSTR filelocation, DWORD access)
{
    HANDLE hFile;
    hFile = CreateFileW(filelocation, access, (access == GENERIC_READ) ? FILE_SHARE_READ : 0, NULL, (access == GENERIC_READ) ? OPEN_EXISTING:CREATE_ALWAYS, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        reportWin32Error(QObject::tr("File Error"),
                         QObject::tr("An error occurred when attempting to get a handle on the file.\n"
                         "Error %1: %2"));
    }
    return hFile;
}

HANDLE getHandleOnDevice(int device, DWORD access)
{
    HANDLE hDevice;
    QString devicename = QString("\\\\.\\PhysicalDrive%1").arg(device);
    // Prefer sharing reads only, so Windows cannot modify the partition table
    // while the image is written. That open is refused while another handle
    // has the disk open for writing, so fall back to sharing writes too.
    hDevice = CreateFile(devicename.toLatin1().data(), access, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (hDevice == INVALID_HANDLE_VALUE)
    {
        hDevice = CreateFile(devicename.toLatin1().data(), access,
                             FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    }
    if (hDevice == INVALID_HANDLE_VALUE)
    {
        reportWin32Error(QObject::tr("Device Error"),
                         QObject::tr("An error occurred when attempting to get a handle on the device.\n"
                         "Error %1: %2"));
    }
    return hDevice;
}

bool removeLockOnVolume(HANDLE handle)
{
    DWORD junk;
    BOOL bResult;
    bResult = DeviceIoControl(handle, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, &junk, NULL);
    if (!bResult)
    {
        reportWin32Error(QObject::tr("Unlock Error"),
                         QObject::tr("An error occurred when attempting to unlock the volume.\n"
                         "Error %1: %2"));
    }
    return (bResult);
}

bool unmountVolume(HANDLE handle)
{
    DWORD junk;
    BOOL bResult;
    bResult = DeviceIoControl(handle, FSCTL_DISMOUNT_VOLUME, NULL, 0, NULL, 0, &junk, NULL);
    if (!bResult)
    {
        reportWin32Error(QObject::tr("Dismount Error"),
                         QObject::tr("An error occurred when attempting to dismount the volume.\n"
                         "Error %1: %2"));
    }
    return (bResult);
}

char *readSectorDataFromHandle(HANDLE handle, unsigned long long startsector, unsigned long long numsectors, unsigned long long sectorsize)
{
    // Not Win32 failures, so no error code to report: reportWin32Error()
    // expects %1/%2 placeholders for one.
    if (sectorsize == 0 || numsectors > ULLONG_MAX / sectorsize) {
        QMessageBox::critical(MainWindow::getInstanceIfAvailable(), QObject::tr("Read Error"),
                              QObject::tr("Sector count too large."));
        return NULL;
    }

    unsigned long bytesread;
    char *data = new(std::nothrow) char[sectorsize * numsectors];
    if (!data)
    {
        QMessageBox::critical(MainWindow::getInstanceIfAvailable(), QObject::tr("Read Error"),
                              QObject::tr("Unable to allocate memory for read buffer."));
        return NULL;
    }
    LARGE_INTEGER li;
    li.QuadPart = startsector * sectorsize;
    // An unchecked failed seek would read from wherever the pointer was.
    if (SetFilePointer(handle, li.LowPart, &li.HighPart, FILE_BEGIN) == INVALID_SET_FILE_POINTER
        && GetLastError() != NO_ERROR)
    {
        reportWin32Error(QObject::tr("Read Error"),
                         QObject::tr("An error occurred when attempting to read data from handle.\n"
                         "Error %1: %2"));
        delete[] data;
        return NULL;
    }
    if (!ReadFile(handle, data, sectorsize * numsectors, &bytesread, NULL))
    {
        reportWin32Error(QObject::tr("Read Error"),
                         QObject::tr("An error occurred when attempting to read data from handle.\n"
                         "Error %1: %2"));
        delete[] data;
        data = NULL;
    }
    if (data && bytesread < (sectorsize * numsectors))
    {
            memset(data + bytesread,0,(sectorsize * numsectors) - bytesread);
    }
    return data;
}

bool writeSectorDataToHandle(HANDLE handle, char *data, unsigned long long startsector, unsigned long long numsectors, unsigned long long sectorsize)
{
    unsigned long byteswritten;
    BOOL bResult;
    LARGE_INTEGER li;
    li.QuadPart = startsector * sectorsize;
    // An unchecked failed seek would put this chunk elsewhere on the device.
    if (SetFilePointer(handle, li.LowPart, &li.HighPart, FILE_BEGIN) == INVALID_SET_FILE_POINTER
        && GetLastError() != NO_ERROR)
    {
        reportWin32Error(QObject::tr("Write Error"),
                         QObject::tr("An error occurred when attempting to write data to handle.\n"
                         "Error %1: %2"));
        return false;
    }
    bResult = WriteFile(handle, data, sectorsize * numsectors, &byteswritten, NULL);
    if (!bResult)
    {
        reportWin32Error(QObject::tr("Write Error"),
                         QObject::tr("An error occurred when attempting to write data to handle.\n"
                         "Error %1: %2"));
        return false;
    }
    if (byteswritten != sectorsize * numsectors)
    {
        // WriteFile can succeed with a short write, leaving a silent hole.
        QMessageBox::critical(MainWindow::getInstanceIfAvailable(), QObject::tr("Write Error"),
            QObject::tr("The device took only %1 of %2 bytes. The image on the device "
                        "is incomplete.")
                .arg((qulonglong)byteswritten)
                .arg((qulonglong)(sectorsize * numsectors)));
        return false;
    }
    return true;
}

unsigned long long getNumberOfSectors(HANDLE handle, unsigned long long *sectorsize,
                                      bool *reported)
{
    DWORD junk;
    DISK_GEOMETRY_EX diskgeometry;
    BOOL bResult;
    bResult = DeviceIoControl(handle, IOCTL_DISK_GET_DRIVE_GEOMETRY_EX, NULL, 0, &diskgeometry, sizeof(diskgeometry), &junk, NULL);
    if (!bResult)
    {
        reportWin32Error(QObject::tr("Device Error"),
                         QObject::tr("An error occurred when attempting to get the device's geometry.\n"
                         "Error %1: %2"));
        if (reported != NULL) *reported = true;
        return 0;
    }
    if (sectorsize != NULL)
    {
        *sectorsize = (unsigned long long)diskgeometry.Geometry.BytesPerSector;
    }
    if (diskgeometry.Geometry.BytesPerSector == 0)
    {
        // Avoid dividing by zero; callers already treat 0 sectors as failure.
        return 0;
    }
    return (unsigned long long)diskgeometry.DiskSize.QuadPart / (unsigned long long)diskgeometry.Geometry.BytesPerSector;
}

unsigned long long getFileSizeInSectors(HANDLE handle, unsigned long long sectorsize)
{
    unsigned long long retVal = 0;
    if (sectorsize) // avoid divide by 0
    {
        LARGE_INTEGER filesize;
        if(GetFileSizeEx(handle, &filesize) == 0)
        {
            reportWin32Error(QObject::tr("File Error"),
                             QObject::tr("An error occurred while getting the file size.\n"
                             "Error %1: %2"));
            retVal = 0;
        }
        else
        {
            retVal = ((unsigned long long)filesize.QuadPart / sectorsize ) + (((unsigned long long)filesize.QuadPart % sectorsize )?1:0);
        }
    }
    return(retVal);
}

bool spaceAvailable(const QString &location, unsigned long long spaceneeded)
{
    ULARGE_INTEGER freespace;
    BOOL bResult;
    bResult = GetDiskFreeSpaceExW((LPCWSTR)location.utf16(), NULL, NULL, &freespace);
    if (!bResult)
    {
        reportWin32Error(QObject::tr("Free Space Error"),
                         QObject::tr("Failed to get the free space on the volume holding %1.\n"
                                     "Error %2: %3\n"
                                     "Checking of free space will be skipped.").arg(location));
        return true;
    }
    return (spaceneeded <= freespace.QuadPart);
}




// Open a volume by drive letter and set *disk to the physical disk of its
// first extent. access 0 queries without needing read rights and without
// disturbing other openers. Returns INVALID_HANDLE_VALUE unless *disk was set.
static HANDLE openVolumeOnDisk(char letter, DWORD access, int *disk)
{
    char volumename[] = "\\\\.\\A:";
    volumename[4] = letter;
    HANDLE h = CreateFile(volumename, access, FILE_SHARE_READ | FILE_SHARE_WRITE,
                          NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE)
    {
        return INVALID_HANDLE_VALUE;
    }
    VOLUME_DISK_EXTENTS sd;
    DWORD bytesreturned;
    if (!DeviceIoControl(h, IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, NULL, 0,
                         &sd, sizeof(sd), &bytesreturned, NULL)
        || sd.NumberOfDiskExtents == 0)
    {
        CloseHandle(h);
        return INVALID_HANDLE_VALUE;
    }
    *disk = (int)sd.Extents[0].DiskNumber;
    return h;
}

// Whether an open volume has any extent on physical disk deviceID. The buffer
// is sized for many extents: a bare VOLUME_DISK_EXTENTS holds one, and a
// spanned or mirrored volume would fail with ERROR_MORE_DATA.
static bool volumeIsOnDisk(HANDLE h, DWORD deviceID)
{
    const DWORD MAX_EXTENTS = 64;
    QByteArray buf((int)(sizeof(VOLUME_DISK_EXTENTS) + MAX_EXTENTS * sizeof(DISK_EXTENT)), 0);
    DWORD bytesreturned = 0;
    if (!DeviceIoControl(h, IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, NULL, 0,
                         buf.data(), (DWORD)buf.size(), &bytesreturned, NULL))
    {
        return false;
    }
    const VOLUME_DISK_EXTENTS *ext = (const VOLUME_DISK_EXTENTS *)buf.constData();
    for (DWORD i = 0; i < ext->NumberOfDiskExtents && i <= MAX_EXTENTS; ++i)
    {
        if (ext->Extents[i].DiskNumber == deviceID)
        {
            return true;
        }
    }
    return false;
}

// Open a volume by its \\?\Volume{GUID}\ name, minus the trailing backslash:
// with it, CreateFile opens the root directory, which takes no volume ioctls.
static HANDLE openVolumeByName(const wchar_t *guidname, DWORD access)
{
    std::wstring device(guidname);
    if (!device.empty() && device.back() == L'\\')
    {
        device.pop_back();
    }
    return CreateFileW(device.c_str(), access, FILE_SHARE_READ | FILE_SHARE_WRITE,
                       NULL, OPEN_EXISTING, 0, NULL);
}

// Where a volume is mounted, for a message: its drive letter or folder if it
// has one, otherwise the GUID name, which is all a letterless volume has.
static QString volumeDisplayName(const wchar_t *guidname)
{
    wchar_t paths[MAX_PATH * 4] = {0};
    DWORD needed = 0;
    if (GetVolumePathNamesForVolumeNameW(guidname, paths, MAX_PATH * 4, &needed)
        && paths[0] != 0)
    {
        return QString::fromWCharArray(paths);
    }
    return QString::fromWCharArray(guidname);
}

bool pathIsOnDisk(const QString &path, ULONG deviceID)
{
    // The file may not exist yet, so ask about its directory.
    QFileInfo fi(path);
    QString probe = QDir::toNativeSeparators(fi.exists() ? fi.absoluteFilePath()
                                                         : fi.absolutePath());
    std::wstring wprobe = probe.toStdWString();

    // Via the mount point, so C:\mnt\card resolves to the card, not C:.
    wchar_t mountpoint[MAX_PATH + 1] = {0};
    wchar_t guidname[MAX_PATH + 1] = {0};
    if (GetVolumePathNameW(wprobe.c_str(), mountpoint, MAX_PATH)
        && GetVolumeNameForVolumeMountPointW(mountpoint, guidname, MAX_PATH))
    {
        HANDLE h = openVolumeByName(guidname, 0);
        if (h != INVALID_HANDLE_VALUE)
        {
            bool onDisk = volumeIsOnDisk(h, deviceID);
            CloseHandle(h);
            return onDisk;
        }
    }

    // A SUBST drive has no mount point of its own, but \\.\X: still opens the
    // volume underneath it. Anything else, a UNC share included, is on no
    // local disk.
    if (probe.length() >= 2 && probe.at(1) == QChar(':'))
    {
        wchar_t device[] = L"\\\\.\\A:";
        device[4] = (wchar_t)probe.at(0).toUpper().unicode();
        HANDLE h = CreateFileW(device, 0, FILE_SHARE_READ | FILE_SHARE_WRITE,
                               NULL, OPEN_EXISTING, 0, NULL);
        if (h != INVALID_HANDLE_VALUE)
        {
            bool onDisk = volumeIsOnDisk(h, deviceID);
            CloseHandle(h);
            return onDisk;
        }
    }
    return false;
}

// Physical disk a mounted volume lives on, or -1 if it cannot be determined.
static int diskNumberOfVolume(char letter)
{
    int disk = -1;
    HANDLE h = openVolumeOnDisk(letter, 0, &disk);
    if (h == INVALID_HANDLE_VALUE)
    {
        return -1;
    }
    CloseHandle(h);
    return disk;
}

QString driveLettersOnDevice(ULONG deviceID)
{
    QStringList found;
    unsigned long driveMask = GetLogicalDrives();
    for (int i = 0; i < 26; ++i)
    {
        if (!(driveMask & (1ul << i)))
        {
            continue;
        }
        char letter = 'A' + i;
        if (diskNumberOfVolume(letter) == (int)deviceID)
        {
            found.append(QString("%1:").arg(QChar(letter)));
        }
    }
    return found.join(", ");
}

QMap<unsigned long long, QString> driveLettersByOffset(ULONG deviceID)
{
    QMap<unsigned long long, QString> found;
    unsigned long driveMask = GetLogicalDrives();
    for (int i = 0; i < 26; ++i)
    {
        if (!(driveMask & (1ul << i)))
        {
            continue;
        }
        char letter = 'A' + i;
        char volumename[] = "\\\\.\\A:";
        volumename[4] = letter;
        HANDLE h = CreateFile(volumename, 0, FILE_SHARE_READ | FILE_SHARE_WRITE,
                              NULL, OPEN_EXISTING, 0, NULL);
        if (h == INVALID_HANDLE_VALUE)
        {
            continue;
        }
        VOLUME_DISK_EXTENTS sd;
        DWORD bytesreturned;
        if (DeviceIoControl(h, IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, NULL, 0,
                            &sd, sizeof(sd), &bytesreturned, NULL)
            && sd.NumberOfDiskExtents > 0
            && sd.Extents[0].DiskNumber == deviceID)
        {
            found.insert((unsigned long long)sd.Extents[0].StartingOffset.QuadPart,
                        QString("%1:").arg(QChar(letter)));
        }
        CloseHandle(h);
    }
    return found;
}

bool diskPartitionNumbers(HANDLE hRawDisk, unsigned long long sectorsize,
                          QMap<unsigned long long, int> *numbersBySector)
{
    if (sectorsize == 0)
    {
        return false;
    }
    // Grow the buffer until the ioctl stops asking for more room, capped at
    // about 4MiB.
    DWORD size = sizeof(DRIVE_LAYOUT_INFORMATION_EX) + 32 * sizeof(PARTITION_INFORMATION_EX);
    QByteArray buf(size, 0);
    DWORD bytesreturned = 0;
    for (;;)
    {
        if (DeviceIoControl(hRawDisk, IOCTL_DISK_GET_DRIVE_LAYOUT_EX, NULL, 0,
                            buf.data(), size, &bytesreturned, NULL))
        {
            break;
        }
        if (GetLastError() != ERROR_INSUFFICIENT_BUFFER || size > 4u * 1024u * 1024u)
        {
            return false;
        }
        size *= 2;
        buf.resize(size);
    }

    const DRIVE_LAYOUT_INFORMATION_EX *layout =
        (const DRIVE_LAYOUT_INFORMATION_EX *)buf.constData();
    numbersBySector->clear();
    for (DWORD i = 0; i < layout->PartitionCount; ++i)
    {
        const PARTITION_INFORMATION_EX &p = layout->PartitionEntry[i];
        // Unused slots come back with partition number 0 or MBR type 0.
        if (p.PartitionNumber == 0)
        {
            continue;
        }
        if (p.PartitionStyle == PARTITION_STYLE_MBR && p.Mbr.PartitionType == 0)
        {
            continue;
        }
        unsigned long long sector = (unsigned long long)p.StartingOffset.QuadPart / sectorsize;
        numbersBySector->insert(sector, (int)p.PartitionNumber);
    }
    return true;
}

// The volume Windows runs from, opened with no access so every disk can be
// tested against all of its extents: a mirrored or spanned boot volume has
// one per disk, and each of those disks must be hidden.
static HANDLE openSystemVolume()
{
    char windir[MAX_PATH + 1] = {0};
    if (GetWindowsDirectoryA(windir, MAX_PATH) == 0)
    {
        return INVALID_HANDLE_VALUE;
    }
    char device[] = "\\\\.\\A:";
    device[4] = windir[0];
    return CreateFileA(device, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                       OPEN_EXISTING, 0, NULL);
}

// A trailing run of spaces and NULs is normal in the descriptor strings.
// valid is how much of buf the driver filled: a descriptor too big for buf is
// truncated, and its string offsets can then point past the end.
static QString descriptorString(const BYTE *buf, DWORD valid, DWORD offset)
{
    if (offset == 0 || offset >= valid)
    {
        return QString();
    }
    const char *s = (const char *)buf + offset;
    return QString::fromLatin1(s, (qsizetype)strnlen(s, valid - offset)).trimmed();
}

QList<PhysicalDevice> enumeratePhysicalDevices(bool includeFixed)
{
    QList<PhysicalDevice> devices;
    HANDLE systemVolume = openSystemVolume();

    // Disk numbers are not dense, so do not stop at the first gap.
    for (ULONG n = 0; n < 128; ++n)
    {
        QString devicename = QString("\\\\.\\PhysicalDrive%1").arg(n);
        // No access rights, for the same reason as openVolumeOnDisk above.
        HANDLE hDevice = CreateFile(devicename.toLatin1().data(), 0,
                                    FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                                    OPEN_EXISTING, 0, NULL);
        if (hDevice == INVALID_HANDLE_VALUE)
        {
            continue;
        }

        PhysicalDevice dev;
        dev.deviceNumber = n;
        dev.sizeBytes = 0;
        dev.removable = false;

        int arrSz = sizeof(STORAGE_DEVICE_DESCRIPTOR) + 512 - 1;
        BYTE *buf = new BYTE[arrSz]();
        PSTORAGE_DEVICE_DESCRIPTOR pDevDesc = (PSTORAGE_DEVICE_DESCRIPTOR)buf;
        pDevDesc->Size = arrSz;
        STORAGE_PROPERTY_QUERY query;
        query.PropertyId = StorageDeviceProperty;
        query.QueryType = PropertyStandardQuery;
        DWORD dwOutBytes;
        bool described = DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY,
                                         &query, sizeof(query), pDevDesc,
                                         pDevDesc->Size, &dwOutBytes, NULL);
        if (described)
        {
            const DWORD valid = qMin(dwOutBytes, (DWORD)arrSz);
            QString vendor = descriptorString(buf, valid, pDevDesc->VendorIdOffset);
            QString product = descriptorString(buf, valid, pDevDesc->ProductIdOffset);
            dev.description = QString("%1 %2").arg(vendor).arg(product).trimmed();
            // eSATA reports removable media but is a fixed internal disk in
            // practice, so it is only offered when fixed disks are shown.
            dev.removable = (pDevDesc->RemovableMedia && pDevDesc->BusType != BusTypeSata)
                            || pDevDesc->BusType == BusTypeUsb
                            || pDevDesc->BusType == BusTypeSd
                            || pDevDesc->BusType == BusTypeMmc;
        }
        delete[] buf;

        // Filter before the geometry query: asking a disk in standby for its
        // size spins it up (seconds for an HDD), whereas the open and the
        // descriptor query do not. Disks about to be dropped are never asked.
        if (systemVolume != INVALID_HANDLE_VALUE && volumeIsOnDisk(systemVolume, n))
        {
            CloseHandle(hDevice);
            continue;
        }
        if (!dev.removable && !includeFixed)
        {
            CloseHandle(hDevice);
            continue;
        }

        // An empty card reader reports no size, so size doubles as the
        // media-present test. Not IOCTL_DISK_GET_LENGTH_INFO: it requires
        // FILE_READ_ACCESS, which this zero-access handle lacks.
        DISK_GEOMETRY_EX geometry;
        DWORD junk;
        if (DeviceIoControl(hDevice, IOCTL_DISK_GET_DRIVE_GEOMETRY_EX, NULL, 0,
                            &geometry, sizeof(geometry), &junk, NULL))
        {
            dev.sizeBytes = (unsigned long long)geometry.DiskSize.QuadPart;
        }
        CloseHandle(hDevice);

        if (dev.sizeBytes == 0)
        {
            continue;
        }
        if (dev.description.isEmpty())
        {
            dev.description = QObject::tr("Unknown device");
        }
        dev.letters = driveLettersOnDevice(n);
        devices.append(dev);
    }
    if (systemVolume != INVALID_HANDLE_VALUE)
    {
        CloseHandle(systemVolume);
    }
    return devices;
}

bool LockedVolumes::lockAll(DWORD deviceID)
{
    // Enumerate volumes, not drive letters: a folder-mounted or letterless
    // volume is just as live, and Windows refuses raw writes to a mounted
    // volume's sectors, failing the write partway through.
    wchar_t guidname[MAX_PATH + 1] = {0};
    HANDLE find = FindFirstVolumeW(guidname, MAX_PATH);
    if (find == INVALID_HANDLE_VALUE)
    {
        // There is always at least the system volume, so this is a failure.
        QMessageBox::critical(MainWindow::getInstanceIfAvailable(), QObject::tr("Lock Error"),
                              QObject::tr("Could not list the volumes on this computer.\n"
                                          "Error %1").arg(GetLastError()));
        return false;
    }
    bool ok = true;
    do
    {
        // Probe with no access; only the target disk's volumes are opened
        // for writing.
        HANDLE probe = openVolumeByName(guidname, 0);
        if (probe == INVALID_HANDLE_VALUE)
        {
            continue;
        }
        bool onDisk = volumeIsOnDisk(probe, deviceID);
        CloseHandle(probe);
        if (!onDisk)
        {
            continue;
        }
        const QString name = volumeDisplayName(guidname);
        HANDLE h = openVolumeByName(guidname, GENERIC_READ | GENERIC_WRITE);
        if (h == INVALID_HANDLE_VALUE)
        {
            // A write-protected card refuses write access, but lock and
            // dismount work on a read handle, so a read can still go ahead.
            h = openVolumeByName(guidname, GENERIC_READ);
        }
        if (h == INVALID_HANDLE_VALUE)
        {
            QMessageBox::critical(MainWindow::getInstanceIfAvailable(), QObject::tr("Lock Error"),
                                  QObject::tr("Could not lock volume %1: it is still in use.\n"
                                              "Close any program using the device and try again.\n"
                                              "Error %2").arg(name).arg(GetLastError()));
            ok = false;
            break;
        }
        // A briefly busy volume (indexer, antivirus, Explorer) fails the lock
        // with ERROR_ACCESS_DENIED, so retry for about two seconds.
        bool gotlock = false;
        DWORD junk;
        for (int attempt = 0; attempt < 20 && !gotlock; ++attempt)
        {
            gotlock = DeviceIoControl(h, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &junk, NULL);
            if (!gotlock)
            {
                Sleep(100);
            }
        }
        if (!gotlock)
        {
            QMessageBox::critical(MainWindow::getInstanceIfAvailable(), QObject::tr("Lock Error"),
                                  QObject::tr("Could not lock volume %1: it is still in use.\n"
                                              "Close any program using the device and try again.\n"
                                              "Error %2").arg(name).arg(GetLastError()));
            CloseHandle(h);
            ok = false;
            break;
        }
        if (!unmountVolume(h))
        {
            CloseHandle(h);
            ok = false;
            break;
        }
        handles.append(h);
    } while (FindNextVolumeW(find, guidname, MAX_PATH));
    FindVolumeClose(find);
    if (!ok)
    {
        release();
    }
    return ok;
}

void LockedVolumes::release()
{
    for (int i = 0; i < handles.size(); ++i)
    {
        removeLockOnVolume(handles.at(i));
        CloseHandle(handles.at(i));
    }
    handles.clear();
}

bool flushDevice(HANDLE handle)
{
    // No IOCTL_DISK_UPDATE_PROPERTIES: a partition table re-read triggers
    // Windows' GPT rewrite (see disk.h).
    return FlushFileBuffers(handle);
}

bool setDiskOffline(HANDLE handle, bool offline)
{
    SET_DISK_ATTRIBUTES sda;
    DWORD junk;
    ZeroMemory(&sda, sizeof(sda));
    sda.Version = sizeof(sda);
    sda.Persist = FALSE;
    sda.Attributes = offline ? DISK_ATTRIBUTE_OFFLINE : 0;
    sda.AttributesMask = DISK_ATTRIBUTE_OFFLINE;
    return DeviceIoControl(handle, IOCTL_DISK_SET_DISK_ATTRIBUTES, &sda, sizeof(sda),
                           NULL, 0, &junk, NULL);
}

bool ejectDevice(HANDLE handle)
{
    DWORD junk;
    PREVENT_MEDIA_REMOVAL pmr;
    pmr.PreventMediaRemoval = FALSE;
    DeviceIoControl(handle, IOCTL_STORAGE_MEDIA_REMOVAL, &pmr, sizeof(pmr), NULL, 0, &junk, NULL);
    return DeviceIoControl(handle, IOCTL_STORAGE_EJECT_MEDIA, NULL, 0, NULL, 0, &junk, NULL);
}

// ---------------------------------------------------------------------------
// GPT repair
// ---------------------------------------------------------------------------

// Offsets within the 92-byte GPT header (UEFI 2.x, section 5.3).
#define GPT_OFF_SIGNATURE      0
#define GPT_OFF_HEADERSIZE    12
#define GPT_OFF_HEADERCRC     16
#define GPT_OFF_MYLBA         24
#define GPT_OFF_ALTLBA        32
#define GPT_OFF_FIRSTUSABLE   40
#define GPT_OFF_LASTUSABLE    48
#define GPT_OFF_ENTRYLBA      72
#define GPT_OFF_NUMENTRIES    80
#define GPT_OFF_ENTRYSIZE     84
#define GPT_OFF_ENTRIESCRC    88
// Offsets within one partition entry.
#define GPT_ENT_TYPEGUID       0
#define GPT_ENT_FIRSTLBA      32
#define GPT_ENT_LASTLBA       40

static DWORD gptCrc32(const unsigned char *data, size_t len)
{
    static DWORD table[256];
    static bool built = false;
    if (!built)
    {
        for (DWORD i = 0; i < 256; ++i)
        {
            DWORD c = i;
            for (int k = 0; k < 8; ++k)
            {
                c = (c & 1) ? (0xEDB88320u ^ (c >> 1)) : (c >> 1);
            }
            table[i] = c;
        }
        built = true;
    }
    DWORD crc = 0xFFFFFFFFu;
    for (size_t i = 0; i < len; ++i)
    {
        crc = table[(crc ^ data[i]) & 0xFF] ^ (crc >> 8);
    }
    return crc ^ 0xFFFFFFFFu;
}

static DWORD rd32(const unsigned char *p, int off)
{
    return (DWORD)p[off] | ((DWORD)p[off+1] << 8) | ((DWORD)p[off+2] << 16) | ((DWORD)p[off+3] << 24);
}

static unsigned long long rd64(const unsigned char *p, int off)
{
    unsigned long long v = 0;
    for (int i = 7; i >= 0; --i)
    {
        v = (v << 8) | p[off + i];
    }
    return v;
}

static void wr32(unsigned char *p, int off, DWORD v)
{
    for (int i = 0; i < 4; ++i)
    {
        p[off + i] = (unsigned char)((v >> (8 * i)) & 0xFF);
    }
}

static void wr64(unsigned char *p, int off, unsigned long long v)
{
    for (int i = 0; i < 8; ++i)
    {
        p[off + i] = (unsigned char)((v >> (8 * i)) & 0xFF);
    }
}

static bool rawSeekRead(HANDLE h, unsigned long long offset, void *buf, DWORD len)
{
    LARGE_INTEGER li;
    DWORD got = 0;
    li.QuadPart = (LONGLONG)offset;
    if (SetFilePointer(h, li.LowPart, &li.HighPart, FILE_BEGIN) == INVALID_SET_FILE_POINTER
        && GetLastError() != NO_ERROR)
    {
        return false;
    }
    return ReadFile(h, buf, len, &got, NULL) && got == len;
}

static bool rawSeekWrite(HANDLE h, unsigned long long offset, const void *buf, DWORD len)
{
    LARGE_INTEGER li;
    DWORD put = 0;
    li.QuadPart = (LONGLONG)offset;
    if (SetFilePointer(h, li.LowPart, &li.HighPart, FILE_BEGIN) == INVALID_SET_FILE_POINTER
        && GetLastError() != NO_ERROR)
    {
        return false;
    }
    return WriteFile(h, buf, len, &put, NULL) && put == len;
}

// An entry is in use when its type GUID is non-zero.
static bool gptEntryInUse(const unsigned char *e)
{
    for (int b = 0; b < 16; ++b)
    {
        if (e[GPT_ENT_TYPEGUID + b] != 0)
        {
            return true;
        }
    }
    return false;
}

// Validate a GPT header's entry-array geometry and report the sectors it
// takes. The signature is not checked: callers distinguish "no GPT" from
// "malformed GPT" themselves.
static bool gptEntryGeometry(const unsigned char *hdr, unsigned long long sectorsize,
                             unsigned long long *entrysectors)
{
    unsigned long long numentries = rd32(hdr, GPT_OFF_NUMENTRIES);
    unsigned long long entrysize  = rd32(hdr, GPT_OFF_ENTRYSIZE);
    if (numentries == 0 || numentries > 65536 || entrysize < 128 || entrysize > 4096)
    {
        return false;
    }
    *entrysectors = (numentries * entrysize + sectorsize - 1) / sectorsize;
    return true;
}

GptFixResult relocateBackupGPT(HANDLE hRawDisk, unsigned long long sectorsize,
                               unsigned long long devicesectors, QString *detail)
{
    if (sectorsize < 512 || devicesectors < 96)
    {
        return GPT_FIX_NO_GPT;
    }

    QByteArray primary(sectorsize, 0);
    unsigned char *hdr = (unsigned char *)primary.data();
    if (!rawSeekRead(hRawDisk, sectorsize, hdr, (DWORD)sectorsize))
    {
        return GPT_FIX_FAILED;
    }
    if (memcmp(hdr + GPT_OFF_SIGNATURE, "EFI PART", 8) != 0)
    {
        return GPT_FIX_NO_GPT;
    }

    DWORD headersize = rd32(hdr, GPT_OFF_HEADERSIZE);
    if (headersize < 92 || headersize > sectorsize)
    {
        if (detail) *detail = QObject::tr("the primary GPT header size is out of range");
        return GPT_FIX_BAD_GPT;
    }

    {
        QByteArray probe = primary.left(headersize);
        wr32((unsigned char *)probe.data(), GPT_OFF_HEADERCRC, 0);
        if (gptCrc32((const unsigned char *)probe.constData(), headersize) != rd32(hdr, GPT_OFF_HEADERCRC))
        {
            if (detail) *detail = QObject::tr("the primary GPT header checksum is invalid");
            return GPT_FIX_BAD_GPT;
        }
    }

    unsigned long long entrylba   = rd64(hdr, GPT_OFF_ENTRYLBA);
    unsigned long long numentries = rd32(hdr, GPT_OFF_NUMENTRIES);
    unsigned long long entrysize  = rd32(hdr, GPT_OFF_ENTRYSIZE);
    unsigned long long lastlba    = devicesectors - 1;

    if (numentries == 0 || numentries > 65536 || entrysize < 128 || entrysize > 4096
        || entrylba < 2 || entrylba >= devicesectors)
    {
        if (detail) *detail = QObject::tr("the GPT partition entry array is not where the header says");
        return GPT_FIX_BAD_GPT;
    }

    if (rd64(hdr, GPT_OFF_ALTLBA) == lastlba)
    {
        return GPT_FIX_NOT_NEEDED;
    }

    unsigned long long entrybytes = numentries * entrysize;
    unsigned long long entrysectors = (entrybytes + sectorsize - 1) / sectorsize;
    if (entrysectors + 2 >= devicesectors)
    {
        // Guards the subtraction from the last LBA below against wrapping.
        if (detail) *detail = QObject::tr("the GPT entry array does not fit on the device");
        return GPT_FIX_BAD_GPT;
    }
    QByteArray entries(entrysectors * sectorsize, 0);
    if (!rawSeekRead(hRawDisk, entrylba * sectorsize, entries.data(), (DWORD)(entrysectors * sectorsize)))
    {
        return GPT_FIX_FAILED;
    }

    // Verify, never recompute, the entry CRC: on a table Windows has already
    // rewritten it is the only record of the real entries, and
    // repairPrimaryGpt() needs it to find them.
    if (gptCrc32((const unsigned char *)entries.constData(), (size_t)entrybytes)
        != rd32(hdr, GPT_OFF_ENTRIESCRC))
    {
        if (detail) *detail = QObject::tr("the GPT partition entry array checksum is invalid");
        return GPT_FIX_BAD_GPT;
    }

    unsigned long long backuphdr     = lastlba;
    unsigned long long backupentries = backuphdr - entrysectors;
    unsigned long long firstusable   = rd64(hdr, GPT_OFF_FIRSTUSABLE);
    unsigned long long lastusable    = backupentries - 1;
    unsigned long long oldbackuphdr  = rd64(hdr, GPT_OFF_ALTLBA);

    if (backupentries <= firstusable || lastusable <= firstusable)
    {
        return GPT_FIX_FAILED;
    }

    // Only a malformed table can have a partition past the new LastUsableLBA.
    for (unsigned long long i = 0; i < numentries; ++i)
    {
        const unsigned char *e = (const unsigned char *)entries.constData() + i * entrysize;
        if (!gptEntryInUse(e))
        {
            continue;
        }
        if (rd64(e, GPT_ENT_LASTLBA) > lastusable)
        {
            if (detail) *detail = QObject::tr("a partition extends past the end of the device");
            return GPT_FIX_FAILED;
        }
    }

    // Only the fields describing where the device ends change. Leave
    // PartitionEntryLBA and FirstUsableLBA as the image wrote them: moving the
    // entry array could overwrite what the image reserved ahead of
    // FirstUsableLBA.
    wr64(hdr, GPT_OFF_MYLBA, 1);
    wr64(hdr, GPT_OFF_ALTLBA, backuphdr);
    wr64(hdr, GPT_OFF_LASTUSABLE, lastusable);
    wr32(hdr, GPT_OFF_HEADERCRC, 0);
    wr32(hdr, GPT_OFF_HEADERCRC, gptCrc32(hdr, headersize));

    // The backup header is the primary with MyLBA/AlternateLBA swapped and its
    // own copy of the entry array.
    QByteArray backup = primary;
    unsigned char *bhdr = (unsigned char *)backup.data();
    wr64(bhdr, GPT_OFF_MYLBA, backuphdr);
    wr64(bhdr, GPT_OFF_ALTLBA, 1);
    wr64(bhdr, GPT_OFF_ENTRYLBA, backupentries);
    wr32(bhdr, GPT_OFF_HEADERCRC, 0);
    wr32(bhdr, GPT_OFF_HEADERCRC, gptCrc32(bhdr, headersize));

    // Backup copies first: if power is lost midway the primary still describes
    // a consistent, if stale, table.
    if (!rawSeekWrite(hRawDisk, backupentries * sectorsize, entries.constData(),
                      (DWORD)(entrysectors * sectorsize))
        || !rawSeekWrite(hRawDisk, backuphdr * sectorsize, bhdr, (DWORD)sectorsize))
    {
        return GPT_FIX_FAILED;
    }
    // The primary entry array is unchanged and not rewritten.
    if (!rawSeekWrite(hRawDisk, sectorsize, hdr, (DWORD)sectorsize))
    {
        return GPT_FIX_FAILED;
    }

    // The protective MBR must span the whole device too, or Windows still sees
    // a mismatch. Only a genuine 0xEE protective entry is touched.
    QByteArray mbr(sectorsize, 0);
    if (rawSeekRead(hRawDisk, 0, mbr.data(), (DWORD)sectorsize))
    {
        unsigned char *m = (unsigned char *)mbr.data();
        if (m[450] == 0xEE)
        {
            unsigned long long span = (lastlba > 0xFFFFFFFFull) ? 0xFFFFFFFFull : lastlba;
            if (rd32(m, 454) == 1 && rd32(m, 458) != (DWORD)span)
            {
                wr32(m, 458, (DWORD)span);
                if (!rawSeekWrite(hRawDisk, 0, m, (DWORD)sectorsize))
                {
                    return GPT_FIX_FAILED;
                }
            }
        }
    }

    // Clear the image's stale backup GPT mid-device: nothing follows the
    // pointers to it, but scan, clone and recovery tools may act on a stray
    // "EFI PART" signature. Only when that sector really is the stale backup
    // header and no partition or the new table overlaps it; failure here does
    // not fail the repair.
    bool stalecleared = false;
    if (oldbackuphdr >= 2 && oldbackuphdr < backupentries)
    {
        QByteArray stale(sectorsize, 0);
        unsigned char *shdr = (unsigned char *)stale.data();
        if (rawSeekRead(hRawDisk, oldbackuphdr * sectorsize, shdr, (DWORD)sectorsize)
            && memcmp(shdr + GPT_OFF_SIGNATURE, "EFI PART", 8) == 0
            && rd64(shdr, GPT_OFF_MYLBA) == oldbackuphdr)
        {
            unsigned long long staleentrylba = rd64(shdr, GPT_OFF_ENTRYLBA);
            unsigned long long stalefirst = staleentrylba;
            unsigned long long stalelast  = oldbackuphdr;
            if (staleentrylba < 2 || staleentrylba > oldbackuphdr
                || oldbackuphdr - staleentrylba != entrysectors)
            {
                // Array not directly below the header: clear the header alone.
                stalefirst = oldbackuphdr;
            }

            // Stay within [FirstUsableLBA, new backup array) and outside every
            // partition.
            bool safe = (stalefirst >= firstusable) && (stalelast < backupentries);
            for (unsigned long long i = 0; safe && i < numentries; ++i)
            {
                const unsigned char *e =
                    (const unsigned char *)entries.constData() + i * entrysize;
                if (!gptEntryInUse(e))
                {
                    continue;
                }
                unsigned long long pstart = rd64(e, GPT_ENT_FIRSTLBA);
                unsigned long long pend   = rd64(e, GPT_ENT_LASTLBA);
                if (pstart <= stalelast && stalefirst <= pend)
                {
                    safe = false;
                }
            }

            if (safe)
            {
                unsigned long long count = stalelast - stalefirst + 1;
                QByteArray zeros(count * sectorsize, 0);
                stalecleared = rawSeekWrite(hRawDisk, stalefirst * sectorsize,
                                            zeros.constData(),
                                            (DWORD)(count * sectorsize));
            }
        }
    }

    FlushFileBuffers(hRawDisk);
    if (detail)
    {
        *detail = stalecleared
            ? QObject::tr("backup GPT moved to LBA %1; last usable LBA is now %2; "
                          "the stale copy at LBA %3 was cleared")
                  .arg(backuphdr).arg(lastusable).arg(oldbackuphdr)
            : QObject::tr("backup GPT moved to LBA %1; last usable LBA is now %2")
                  .arg(backuphdr).arg(lastusable);
    }
    return GPT_FIX_OK;
}

// One in-use primary MBR entry: its table slot and current extent.
struct MbrSlot
{
    int idx;
    unsigned long long first, count;
};

// Read sector 0 into *sector0 if it holds an MBR that is the disk's partition
// table; otherwise return false with *sector0 untouched.
//
// A GPT disk's MBR is not: a 0xEE entry, or a GPT header at LBA 1, means the
// MBR is only the protective one, or a hybrid copy of some GPT partitions.
// Repacking those entries would move partitions while the GPT, and its backup
// dropped with the rest of the tail, still described them where they were --
// and Read tries the MBR plan whenever the GPT plan declines.
static bool readValidMbr(HANDLE hRawDisk, unsigned long long sectorsize,
                         unsigned long long devicesectors, QByteArray *sector0,
                         QString *detail)
{
    if (sectorsize < 512 || devicesectors < 3)
    {
        return false;
    }
    QByteArray sector(sectorsize, 0);
    unsigned char *mbr = (unsigned char *)sector.data();
    if (!rawSeekRead(hRawDisk, 0ull, mbr, (DWORD)sectorsize)
        || mbr[510] != 0x55 || mbr[511] != 0xAA)
    {
        return false;
    }
    bool gpt = false;
    for (int i = 0; i < 4 && !gpt; ++i)
    {
        gpt = (mbr[446 + i * 16 + 4] == 0xEE);
    }
    if (!gpt)
    {
        QByteArray lba1(sectorsize, 0);
        gpt = rawSeekRead(hRawDisk, sectorsize, lba1.data(), (DWORD)sectorsize)
              && memcmp(lba1.constData() + GPT_OFF_SIGNATURE, "EFI PART", 8) == 0;
    }
    if (gpt)
    {
        if (detail) *detail = QObject::tr("the device has a GPT, which its MBR only mirrors");
        return false;
    }
    *sector0 = sector;
    return true;
}

// Append every in-use primary entry to *order, in slot order (callers sort).
// Skips empty and zero-length entries, and 0xEE ones, though readValidMbr()
// already turns a disk with one away; extended/logical partitions are not
// walked. Returns false, via *detail, on an impossible range.
static bool walkMbrEntries(const unsigned char *mbr, unsigned long long devicesectors,
                           QList<MbrSlot> *order, QString *detail)
{
    for (int i = 0; i < 4; ++i)
    {
        const unsigned char *e = mbr + 446 + i * 16;
        if (e[4] == 0 || e[4] == 0xEE)
        {
            continue;
        }
        unsigned long long start = rd32(e, 8);
        unsigned long long count = rd32(e, 12);
        if (count == 0)
        {
            continue;
        }
        if (start < 1ull || start >= devicesectors || count > devicesectors - start)
        {
            if (detail) *detail = QObject::tr("a partition entry describes an impossible range");
            return false;
        }
        MbrSlot s;
        s.idx = i;
        s.first = start;
        s.count = count;
        order->append(s);
    }
    return true;
}

bool listMbrPartitions(HANDLE hRawDisk, unsigned long long sectorsize,
                       unsigned long long devicesectors,
                       QList<PartitionInfo> *partitions, QString *detail)
{
    QByteArray sector0;
    if (!readValidMbr(hRawDisk, sectorsize, devicesectors, &sector0, detail))
    {
        return false;
    }
    const unsigned char *mbr = (const unsigned char *)sector0.constData();

    QList<MbrSlot> order;
    if (!walkMbrEntries(mbr, devicesectors, &order, detail))
    {
        return false;
    }
    std::sort(order.begin(), order.end(), [](const MbrSlot &a, const MbrSlot &b)
    {
        return a.first < b.first;
    });

    partitions->clear();
    for (const MbrSlot &s : order)
    {
        PartitionInfo info;
        info.slot = s.idx;
        info.firstSector = s.first;
        info.sectors = s.count;
        partitions->append(info);
    }
    return true;
}

bool planMbrShrink(HANDLE hRawDisk, unsigned long long sectorsize,
                   unsigned long long devicesectors, unsigned long long alignsectors,
                   PartitionShrinkPlan *plan, QString *detail,
                   const QList<int> *excludeSlots)
{
    if (alignsectors == 0)
    {
        return false;
    }

    QByteArray sector0;
    if (!readValidMbr(hRawDisk, sectorsize, devicesectors, &sector0, detail))
    {
        return false;
    }
    unsigned char *mbr = (unsigned char *)sector0.data();

    QList<MbrSlot> order;
    if (!walkMbrEntries(mbr, devicesectors, &order, detail))
    {
        return false;
    }
    // Where the first partition starts, excluded or not: everything before it
    // is kept as it is; see PartitionShrinkPlan.
    unsigned long long leading = devicesectors;
    for (const MbrSlot &s : order)
    {
        leading = qMin(leading, s.first);
    }
    if (excludeSlots)
    {
        for (int i = order.size() - 1; i >= 0; --i)
        {
            if (excludeSlots->contains(order[i].idx))
            {
                memset(mbr + 446 + order[i].idx * 16, 0, 16);
                order.removeAt(i);
            }
        }
    }
    if (order.isEmpty())
    {
        if (detail) *detail = QObject::tr("the MBR holds no partitions to shrink to");
        return false;
    }
    std::sort(order.begin(), order.end(), [](const MbrSlot &a, const MbrSlot &b)
    {
        return a.first < b.first;
    });

    // The leading area, boot sector aside, is copied where it is; packing
    // starts where the first partition did.
    QList<ShrinkCopyRange> ranges;
    if (leading > 1ull)
    {
        ranges.append(ShrinkCopyRange{1ull, 1ull, leading - 1ull});
    }
    unsigned long long cursor = leading;
    unsigned long long prevend = leading;      // original end of the last kept partition
    for (const MbrSlot &s : order)
    {
        if (s.first < prevend)
        {
            // Overlapping entries: packing them apart would change what each holds.
            if (detail) *detail = QObject::tr("a partition entry describes an impossible range");
            return false;
        }
        prevend = s.first + s.count;
        // The first kept partition goes where the first partition was, even
        // off a 1MiB boundary, as it was on the device; the rest are aligned.
        // Never later than it already is: a partition already packed tighter
        // than the alignment stays put.
        unsigned long long newfirst = (cursor == leading) ? leading : qMin(s.first,
            ((cursor + alignsectors - 1) / alignsectors) * alignsectors);
        unsigned long long newlast  = newfirst + s.count - 1;
        if (newfirst > 0xFFFFFFFFull)
        {
            // The MBR start field is 32 bits.
            if (detail) *detail = QObject::tr("the repacked layout no longer fits a 32-bit MBR entry");
            return false;
        }
        ranges.append(ShrinkCopyRange{s.first, newfirst, s.count});
        wr32(mbr + 446 + s.idx * 16, 8, (DWORD)newfirst);
        cursor = newlast + 1;
    }

    if (cursor >= devicesectors)
    {
        if (detail) *detail = QObject::tr("the device is already this tight; nothing to shrink");
        return false;
    }

    plan->headerregion  = sector0;
    plan->headersectors = 1ull;
    plan->ranges        = ranges;
    plan->backupregion.clear();
    plan->backupsectors = 0ull;
    plan->totalsectors  = cursor;
    return true;
}

// Read and validate the primary GPT header and entry array. On failure the
// outputs are untouched and *detail, when given, may explain why.
static bool readValidGpt(HANDLE hRawDisk, unsigned long long sectorsize,
                         unsigned long long devicesectors, QByteArray *primaryOut,
                         QByteArray *entriesOut, unsigned long long *entrylbaOut,
                         unsigned long long *entrysectorsOut, unsigned long long *numentriesOut,
                         unsigned long long *entrysizeOut, unsigned long long *firstusableOut,
                         QString *detail)
{
    if (sectorsize < 512 || devicesectors < 96)
    {
        return false;
    }

    QByteArray primary(sectorsize, 0);
    unsigned char *hdr = (unsigned char *)primary.data();
    if (!rawSeekRead(hRawDisk, sectorsize, hdr, (DWORD)sectorsize)
        || memcmp(hdr + GPT_OFF_SIGNATURE, "EFI PART", 8) != 0)
    {
        return false;
    }

    DWORD headersize = rd32(hdr, GPT_OFF_HEADERSIZE);
    if (headersize < 92 || headersize > sectorsize)
    {
        if (detail) *detail = QObject::tr("the primary GPT header size is out of range");
        return false;
    }
    {
        QByteArray probe = primary.left(headersize);
        wr32((unsigned char *)probe.data(), GPT_OFF_HEADERCRC, 0);
        if (gptCrc32((const unsigned char *)probe.constData(), headersize) != rd32(hdr, GPT_OFF_HEADERCRC))
        {
            if (detail) *detail = QObject::tr("the primary GPT header checksum is invalid");
            return false;
        }
    }

    unsigned long long entrysectors;
    if (!gptEntryGeometry(hdr, sectorsize, &entrysectors))
    {
        if (detail) *detail = QObject::tr("the GPT entry array geometry is not usable");
        return false;
    }
    unsigned long long entrylba    = rd64(hdr, GPT_OFF_ENTRYLBA);
    unsigned long long numentries  = rd32(hdr, GPT_OFF_NUMENTRIES);
    unsigned long long entrysize   = rd32(hdr, GPT_OFF_ENTRYSIZE);
    unsigned long long firstusable = rd64(hdr, GPT_OFF_FIRSTUSABLE);
    if (entrylba < 2 || entrylba >= devicesectors || entrysectors > devicesectors - entrylba)
    {
        if (detail) *detail = QObject::tr("the GPT partition entry array is not where the header says");
        return false;
    }
    if (firstusable < entrylba + entrysectors || firstusable >= devicesectors
        || firstusable > devicesectors / 2)
    {
        // The devicesectors / 2 bound is a sanity check, not a spec rule:
        // planGptShrink() reads everything ahead of FirstUsableLBA in one
        // piece.
        if (detail) *detail = QObject::tr("FirstUsableLBA is not usable for repacking");
        return false;
    }

    unsigned long long entrybytes = numentries * entrysize;
    QByteArray entries(entrysectors * sectorsize, 0);
    if (!rawSeekRead(hRawDisk, entrylba * sectorsize, entries.data(), (DWORD)(entrysectors * sectorsize)))
    {
        return false;
    }
    if (gptCrc32((const unsigned char *)entries.constData(), (size_t)entrybytes)
        != rd32(hdr, GPT_OFF_ENTRIESCRC))
    {
        if (detail) *detail = QObject::tr("the GPT partition entry array checksum is invalid");
        return false;
    }

    *primaryOut = primary;
    *entriesOut = entries;
    *entrylbaOut = entrylba;
    *entrysectorsOut = entrysectors;
    *numentriesOut = numentries;
    *entrysizeOut = entrysize;
    *firstusableOut = firstusable;
    return true;
}

// The partition name from a GPT entry (UTF-16LE, 36 code units, offset 56),
// truncated at the first NUL.
static QString gptEntryName(const unsigned char *e)
{
    QString name = QString::fromUtf16(reinterpret_cast<const char16_t *>(e + 56), 36);
    int nul = name.indexOf(QChar(0));
    if (nul >= 0)
    {
        name.truncate(nul);
    }
    return name;
}

bool listGptPartitions(HANDLE hRawDisk, unsigned long long sectorsize,
                       unsigned long long devicesectors,
                       QList<PartitionInfo> *partitions, QString *detail)
{
    QByteArray primary, entries;
    unsigned long long entrylba, entrysectors, numentries, entrysize, firstusable;
    if (!readValidGpt(hRawDisk, sectorsize, devicesectors, &primary, &entries,
                      &entrylba, &entrysectors, &numentries, &entrysize, &firstusable, detail))
    {
        return false;
    }

    QList<QPair<unsigned long long, PartitionInfo>> found;
    for (unsigned long long i = 0; i < numentries; ++i)
    {
        const unsigned char *e = (const unsigned char *)entries.constData() + i * entrysize;
        if (!gptEntryInUse(e))
        {
            continue;
        }
        unsigned long long first = rd64(e, GPT_ENT_FIRSTLBA);
        unsigned long long last  = rd64(e, GPT_ENT_LASTLBA);
        if (last < first)
        {
            continue;
        }
        PartitionInfo info;
        info.slot = (int)i;
        info.firstSector = first;
        info.sectors = last - first + 1;
        info.name = gptEntryName(e);
        found.append(qMakePair(first, info));
    }
    std::sort(found.begin(), found.end(), [](const QPair<unsigned long long, PartitionInfo> &a,
                                              const QPair<unsigned long long, PartitionInfo> &b)
    {
        return a.first < b.first;
    });

    partitions->clear();
    for (const auto &pair : found)
    {
        partitions->append(pair.second);
    }
    return true;
}

bool planGptShrink(HANDLE hRawDisk, unsigned long long sectorsize,
                   unsigned long long devicesectors, unsigned long long alignsectors,
                   PartitionShrinkPlan *plan, QString *detail,
                   const QList<int> *excludeSlots)
{
    if (alignsectors == 0)
    {
        return false;
    }

    QByteArray primary, entries;
    unsigned long long entrylba, entrysectors, numentries, entrysize, firstusable;
    if (!readValidGpt(hRawDisk, sectorsize, devicesectors, &primary, &entries,
                      &entrylba, &entrysectors, &numentries, &entrysize, &firstusable, detail))
    {
        return false;
    }
    unsigned char *hdr = (unsigned char *)primary.data();
    DWORD headersize = rd32(hdr, GPT_OFF_HEADERSIZE);
    unsigned long long entrybytes = numentries * entrysize;

    // The tables themselves; the area after them, up to the first partition,
    // is copied as a range (see PartitionShrinkPlan) rather than held in
    // memory.
    const unsigned long long headerend = entrylba + entrysectors;
    if (headerend * sectorsize > 64ull * 1024ull * 1024ull)
    {
        if (detail) *detail = QObject::tr("the GPT partition entry array is not where the header says");
        return false;
    }

    // Kept slots, sorted by start below so packing preserves on-disk order,
    // and where the first partition starts, excluded or not.
    QList<int> order;
    unsigned long long leading = devicesectors;
    for (unsigned long long i = 0; i < numentries; ++i)
    {
        unsigned char *e = (unsigned char *)entries.data() + i * entrysize;
        if (!gptEntryInUse(e))
        {
            continue;
        }
        unsigned long long first = rd64(e, GPT_ENT_FIRSTLBA);
        unsigned long long last  = rd64(e, GPT_ENT_LASTLBA);
        if (last < first || last >= devicesectors || first < firstusable)
        {
            if (detail) *detail = QObject::tr("a partition entry describes an impossible range");
            return false;
        }
        leading = qMin(leading, first);
        if (excludeSlots && excludeSlots->contains((int)i))
        {
            memset(e, 0, (size_t)entrysize);
            continue;
        }
        order.append((int)i);
    }
    if (order.isEmpty())
    {
        if (detail) *detail = QObject::tr("the GPT holds no partitions to shrink to");
        return false;
    }
    std::sort(order.begin(), order.end(), [&](int a, int b)
    {
        const unsigned char *ea = (const unsigned char *)entries.constData() + (size_t)a * entrysize;
        const unsigned char *eb = (const unsigned char *)entries.constData() + (size_t)b * entrysize;
        return rd64(ea, GPT_ENT_FIRSTLBA) < rd64(eb, GPT_ENT_FIRSTLBA);
    });

    // Everything from the end of the table to the first partition is copied
    // where it is; packing starts where the first partition did.
    QList<ShrinkCopyRange> ranges;
    if (leading > headerend)
    {
        ranges.append(ShrinkCopyRange{headerend, headerend, leading - headerend});
    }
    unsigned long long cursor = leading;
    unsigned long long prevend = leading;      // original end of the last kept partition
    for (int idx : order)
    {
        unsigned char *e = (unsigned char *)entries.data() + (size_t)idx * entrysize;
        unsigned long long origfirst = rd64(e, GPT_ENT_FIRSTLBA);
        unsigned long long origlast  = rd64(e, GPT_ENT_LASTLBA);
        if (origfirst < prevend)
        {
            // Overlapping entries: packing them apart would change what each holds.
            if (detail) *detail = QObject::tr("a partition entry describes an impossible range");
            return false;
        }
        prevend = origlast + 1;
        unsigned long long length  = origlast - origfirst + 1;
        // As in planMbrShrink(): the first kept partition goes where the first
        // partition was, the rest aligned and never later than they were.
        unsigned long long newfirst = (cursor == leading) ? leading : qMin(origfirst,
            ((cursor + alignsectors - 1) / alignsectors) * alignsectors);
        unsigned long long newlast  = newfirst + length - 1;
        ranges.append(ShrinkCopyRange{origfirst, newfirst, length});
        wr64(e, GPT_ENT_FIRSTLBA, newfirst);
        wr64(e, GPT_ENT_LASTLBA, newlast);
        cursor = newlast + 1;
    }

    if (cursor + entrysectors + 1 >= devicesectors)
    {
        if (detail) *detail = QObject::tr("the device is already this tight; nothing to shrink");
        return false;
    }

    // From here on, the repacked image: backup array and header right after
    // the data, as relocateBackupGPT() would place them.
    unsigned long long lastlba      = cursor + entrysectors;
    unsigned long long backuphdr    = lastlba;
    unsigned long long backupentries = cursor;
    unsigned long long lastusable   = cursor - 1;

    wr64(hdr, GPT_OFF_ALTLBA, backuphdr);
    wr64(hdr, GPT_OFF_LASTUSABLE, lastusable);
    // The entries changed, so both CRCs are redone.
    wr32(hdr, GPT_OFF_ENTRIESCRC, gptCrc32((const unsigned char *)entries.constData(), (size_t)entrybytes));
    wr32(hdr, GPT_OFF_HEADERCRC, 0);
    wr32(hdr, GPT_OFF_HEADERCRC, gptCrc32(hdr, headersize));

    QByteArray region((size_t)(headerend * sectorsize), 0);
    if (!rawSeekRead(hRawDisk, 0, region.data(), (DWORD)region.size()))
    {
        return false;
    }
    memcpy(region.data() + sectorsize, hdr, headersize);
    memcpy(region.data() + entrylba * sectorsize, entries.constData(), (size_t)entries.size());

    // Protective MBR span, as in relocateBackupGPT().
    unsigned char *mbr = (unsigned char *)region.data();
    if (mbr[450] == 0xEE)
    {
        unsigned long long span = (lastlba > 0xFFFFFFFFull) ? 0xFFFFFFFFull : lastlba;
        if (rd32(mbr, 454) == 1)
        {
            wr32(mbr, 458, (DWORD)span);
        }
    }

    // Backup header, built as in relocateBackupGPT().
    QByteArray backup(sectorsize, 0);
    unsigned char *bhdr = (unsigned char *)backup.data();
    memcpy(bhdr, hdr, headersize);
    wr64(bhdr, GPT_OFF_MYLBA, backuphdr);
    wr64(bhdr, GPT_OFF_ALTLBA, 1);
    wr64(bhdr, GPT_OFF_ENTRYLBA, backupentries);
    wr32(bhdr, GPT_OFF_HEADERCRC, 0);
    wr32(bhdr, GPT_OFF_HEADERCRC, gptCrc32(bhdr, headersize));

    QByteArray backupregion;
    backupregion.append(entries);
    backupregion.append(backup);

    plan->headerregion  = region;
    plan->headersectors = headerend;
    plan->ranges        = ranges;
    plan->backupregion  = backupregion;
    plan->backupsectors = entrysectors + 1;
    plan->totalsectors  = lastlba + 1;
    return true;
}

bool deviceHasMbrTable(HANDLE hRawDisk, unsigned long long sectorsize)
{
    if (sectorsize < 512)
    {
        return false;
    }

    QByteArray sector(sectorsize, 0);
    unsigned char *mbr = (unsigned char *)sector.data();
    if (!rawSeekRead(hRawDisk, 0ull, mbr, (DWORD)sectorsize))
    {
        return false;
    }
    if (mbr[510] != 0x55 || mbr[511] != 0xAA)
    {
        return false;
    }
    // 0xEE is skipped so a device with a damaged GPT header is not reported
    // as an MBR image.
    for (int i = 0; i < 4; ++i)
    {
        unsigned char type = mbr[446 + i * 16 + 4];
        if (type != 0x00 && type != 0xEE)
        {
            return true;
        }
    }
    return false;
}

GptRewriteRisk gptRewriteRisk(HANDLE hRawDisk, unsigned long long sectorsize)
{
    if (sectorsize < 512)
    {
        return GPT_RISK_UNKNOWN;
    }

    QByteArray primary(sectorsize, 0);
    unsigned char *hdr = (unsigned char *)primary.data();
    if (!rawSeekRead(hRawDisk, sectorsize, hdr, (DWORD)sectorsize))
    {
        return GPT_RISK_UNKNOWN;
    }
    if (memcmp(hdr + GPT_OFF_SIGNATURE, "EFI PART", 8) != 0)
    {
        return GPT_RISK_NO_GPT;
    }

    unsigned long long entrylba    = rd64(hdr, GPT_OFF_ENTRYLBA);
    unsigned long long firstusable = rd64(hdr, GPT_OFF_FIRSTUSABLE);
    unsigned long long entrysectors = 0ull;
    if (!gptEntryGeometry(hdr, sectorsize, &entrysectors))
    {
        return GPT_RISK_UNKNOWN;
    }
    if (firstusable < entrysectors)
    {
        return GPT_RISK_UNKNOWN;
    }

    // The value Windows would write, against the one that is there.
    return (firstusable - entrysectors == entrylba) ? GPT_RISK_SAFE : GPT_RISK_AFFECTED;
}

// Read the primary header into *header and check its signature, size and CRC.
static GptPrimaryState readPrimaryForCheck(HANDLE hRawDisk, unsigned long long sectorsize,
                                           QByteArray *header)
{
    header->fill(0, sectorsize);
    unsigned char *hdr = (unsigned char *)header->data();
    if (!rawSeekRead(hRawDisk, sectorsize, hdr, (DWORD)sectorsize))
    {
        return GPT_PRIMARY_UNKNOWN;
    }
    if (memcmp(hdr + GPT_OFF_SIGNATURE, "EFI PART", 8) != 0)
    {
        return GPT_PRIMARY_NO_GPT;
    }
    DWORD headersize = rd32(hdr, GPT_OFF_HEADERSIZE);
    if (headersize < 92 || headersize > sectorsize)
    {
        return GPT_PRIMARY_UNKNOWN;
    }
    QByteArray probe = header->left(headersize);
    wr32((unsigned char *)probe.data(), GPT_OFF_HEADERCRC, 0);
    if (gptCrc32((const unsigned char *)probe.constData(), headersize)
        != rd32(hdr, GPT_OFF_HEADERCRC))
    {
        // Windows' rewrite leaves a header that passes; this is other damage.
        return GPT_PRIMARY_UNKNOWN;
    }
    return GPT_PRIMARY_OK;
}

GptPrimaryState gptPrimaryState(HANDLE hRawDisk, unsigned long long sectorsize,
                                unsigned long long devicesectors)
{
    if (sectorsize < 512 || devicesectors < 96)
    {
        return GPT_PRIMARY_UNKNOWN;
    }

    QByteArray primary;
    GptPrimaryState st = readPrimaryForCheck(hRawDisk, sectorsize, &primary);
    if (st != GPT_PRIMARY_OK)
    {
        return st;
    }
    const unsigned char *hdr = (const unsigned char *)primary.constData();

    unsigned long long entrysectors = 0ull;
    if (!gptEntryGeometry(hdr, sectorsize, &entrysectors))
    {
        return GPT_PRIMARY_UNKNOWN;
    }
    unsigned long long entrylba = rd64(hdr, GPT_OFF_ENTRYLBA);
    if (entrylba < 2 || entrylba + entrysectors > devicesectors)
    {
        // Pointing off the device is the same fault, just further out.
        return GPT_PRIMARY_BROKEN;
    }

    QByteArray entries(entrysectors * sectorsize, 0);
    if (!rawSeekRead(hRawDisk, entrylba * sectorsize, entries.data(),
                     (DWORD)(entrysectors * sectorsize)))
    {
        return GPT_PRIMARY_UNKNOWN;
    }
    size_t crclen = (size_t)(rd32(hdr, GPT_OFF_NUMENTRIES) * rd32(hdr, GPT_OFF_ENTRYSIZE));
    DWORD crc = gptCrc32((const unsigned char *)entries.constData(), crclen);
    return (crc == rd32(hdr, GPT_OFF_ENTRIESCRC)) ? GPT_PRIMARY_OK : GPT_PRIMARY_BROKEN;
}

bool repairPrimaryGpt(HANDLE hRawDisk, unsigned long long sectorsize,
                      unsigned long long devicesectors, QString *detail)
{
    if (sectorsize < 512 || devicesectors < 96)
    {
        if (detail) *detail = QObject::tr("the device geometry is not usable");
        return false;
    }

    QByteArray primary;
    if (readPrimaryForCheck(hRawDisk, sectorsize, &primary) != GPT_PRIMARY_OK)
    {
        if (detail) *detail = QObject::tr("the primary GPT header is not readable");
        return false;
    }
    unsigned char *hdr = (unsigned char *)primary.data();

    unsigned long long entrysectors = 0ull;
    if (!gptEntryGeometry(hdr, sectorsize, &entrysectors))
    {
        if (detail) *detail = QObject::tr("the GPT entry array geometry is not usable");
        return false;
    }

    DWORD wantcrc = rd32(hdr, GPT_OFF_ENTRIESCRC);
    size_t crclen = (size_t)(rd32(hdr, GPT_OFF_NUMENTRIES) * rd32(hdr, GPT_OFF_ENTRYSIZE));
    if (2ull + entrysectors > devicesectors)
    {
        if (detail) *detail = QObject::tr("the device is too small to hold an entry array");
        return false;
    }

    QByteArray entries(entrysectors * sectorsize, 0);
    if (!rawSeekRead(hRawDisk, 2ull * sectorsize, entries.data(),
                     (DWORD)(entrysectors * sectorsize)))
    {
        if (detail) *detail = QObject::tr("the partition entries could not be read");
        return false;
    }
    if (gptCrc32((const unsigned char *)entries.constData(), crclen) != wantcrc)
    {
        if (detail)
        {
            *detail = QObject::tr("the partition entries are not at LBA 2, so this is not "
                                  "the damage this can repair");
        }
        return false;
    }

    wr64(hdr, GPT_OFF_ENTRYLBA, 2ull);
    wr32(hdr, GPT_OFF_HEADERCRC, 0);
    wr32(hdr, GPT_OFF_HEADERCRC, gptCrc32(hdr, rd32(hdr, GPT_OFF_HEADERSIZE)));
    if (!rawSeekWrite(hRawDisk, sectorsize, hdr, (DWORD)sectorsize))
    {
        if (detail) *detail = QObject::tr("the repaired header could not be written");
        return false;
    }
    FlushFileBuffers(hRawDisk);
    if (detail)
    {
        *detail = QObject::tr("PartitionEntryLBA pointed back at LBA 2 and the header "
                              "checksum rebuilt");
    }
    return true;
}

bool gptImageBackupRange(const unsigned char *lba1, unsigned long long sectorsize,
                         unsigned long long *first, unsigned long long *last)
{
    if (lba1 == NULL || sectorsize < 512)
    {
        return false;
    }
    if (memcmp(lba1 + GPT_OFF_SIGNATURE, "EFI PART", 8) != 0)
    {
        return false;
    }

    unsigned long long backuphdr = rd64(lba1, GPT_OFF_ALTLBA);
    unsigned long long entrysectors = 0ull;
    if (backuphdr < 2 || !gptEntryGeometry(lba1, sectorsize, &entrysectors))
    {
        return false;
    }

    // Assume the entry array sits directly below the header, as the clearing
    // in relocateBackupGPT() normally finds it; if that would reach LBA 1,
    // report the header sector alone.
    unsigned long long lo = backuphdr;
    if (entrysectors < backuphdr - 1)
    {
        lo = backuphdr - entrysectors;
    }

    if (first) *first = lo;
    if (last)  *last  = backuphdr;
    return true;
}

bool gptOwnedSectors(HANDLE hRawDisk, unsigned long long sectorsize,
                     unsigned long long devicesectors,
                     unsigned long long *frontend, unsigned long long *tailstart)
{
    if (sectorsize < 512 || devicesectors < 96)
    {
        return false;
    }

    QByteArray primary(sectorsize, 0);
    unsigned char *hdr = (unsigned char *)primary.data();
    if (!rawSeekRead(hRawDisk, sectorsize, hdr, (DWORD)sectorsize))
    {
        return false;
    }
    if (memcmp(hdr + GPT_OFF_SIGNATURE, "EFI PART", 8) != 0)
    {
        return false;
    }

    unsigned long long entrysectors = 0ull;
    if (!gptEntryGeometry(hdr, sectorsize, &entrysectors))
    {
        return false;
    }
    if (entrysectors + 2 >= devicesectors)
    {
        return false;
    }

    // Front: protective MBR and primary header -- the only front sectors
    // relocateBackupGPT() and repairPrimaryGpt() write.
    if (frontend)  *frontend  = 2;
    // Tail: relocated backup entry array plus its header at the last LBA.
    if (tailstart) *tailstart = devicesectors - 1 - entrysectors;
    return true;
}

// Protective MBR + header + 32 entry sectors (a standard 512-byte-sector GPT).
// Used at both ends; the tail needs one fewer, but the spare sector is harmless.
#define GPT_RESERVED_SECTORS 34

bool wipePartitionTables(HANDLE hRawDisk, unsigned long long sectorsize,
                         unsigned long long devicesectors)
{
    if (sectorsize < 512 || devicesectors < (GPT_RESERVED_SECTORS * 2))
    {
        return false;
    }

    QByteArray zeros(GPT_RESERVED_SECTORS * sectorsize, 0);

    // Front: protective MBR and primary GPT, so a partial write cannot leave
    // a hybrid of old and new tables.
    if (!rawSeekWrite(hRawDisk, 0, zeros.constData(), (DWORD)zeros.size()))
    {
        return false;
    }

    // Tail: wherever a backup GPT from any previous image would sit.
    unsigned long long tail = (devicesectors - GPT_RESERVED_SECTORS) * sectorsize;
    if (!rawSeekWrite(hRawDisk, tail, zeros.constData(), (DWORD)zeros.size()))
    {
        return false;
    }

    return FlushFileBuffers(hRawDisk);
}
