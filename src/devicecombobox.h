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

#ifndef DEVICECOMBOBOX_H
#define DEVICECOMBOBOX_H

#include <QtWidgets>
#include <QComboBox>

// Device list that rescans as it opens rather than on a timer, since scanning
// spins up sleeping disks (see MainWindow's connect to aboutToShowPopup).
// Hooked on showPopup() rather than a click so F4 and Alt+Down rescan too.
class DeviceComboBox : public QComboBox
{
    Q_OBJECT

public:
    DeviceComboBox(QWidget *parent = 0);

    void showPopup() override;

signals:
    // Emitted before the list drops down; same-thread (direct) slots finish
    // before the popup is built.
    void aboutToShowPopup();
};

#endif // DEVICECOMBOBOX_H
