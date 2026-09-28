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

#include "tooltips.h"

#include <QtWidgets>

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

void wrapLongToolTips(QWidget *root)
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
