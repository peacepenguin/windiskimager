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
 **********************************************************************/

#ifndef WINVER
#define WINVER 0x0601
#endif

#include <QApplication>
#include <windows.h>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationDisplayName(VER);

    // Qt's own strings (QMessageBox buttons) come from translations/ next to
    // the exe. tools/deploy-cross.sh copies Qt's qtbase_<lang>.qm;
    // windeployqt, used by tools/deploy.sh, writes only a merged qt_<lang>.qm.
    // Try qtbase first, then the merged file, or a native build shows Qt's
    // strings in English.
    QTranslator qttranslator;
    const QString qttrdir = QCoreApplication::applicationDirPath() + "/translations";
    if (qttranslator.load(QLocale::system(), "qtbase", "_", qttrdir)
        || qttranslator.load(QLocale::system(), "qt", "_", qttrdir))
        app.installTranslator(&qttranslator);

    // The app's own catalogues are embedded by translations.qrc, under :/lang.
    QTranslator translator;
    if (translator.load(QLocale::system(), "diskimager", "_", ":/lang"))
        app.installTranslator(&translator);

    MainWindow* mainwindow = MainWindow::getInstance();
    mainwindow->show();
    return app.exec();
}
