/******************************************************************************
** QE - ctlutils.cpp
**
**  Copyright (C) 2018 Alexander Taylor
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
******************************************************************************/

#include "ctlutils.h"


void doCopy( QString text, QLineEdit *field )
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText( text, clipboard->supportsSelection() ?
                            QClipboard::Selection :
                            QClipboard::Clipboard );
    field->deselect();
}


void doPaste( QLineEdit *field )
{
    QClipboard *clipboard = QApplication::clipboard();
    QString text = clipboard->supportsSelection() ?
                       clipboard->text( QClipboard::Selection ) :
                       clipboard->text( QClipboard::Clipboard );
    field->insert( text );
}


void mouseAction( QMouseEvent *event, QLineEdit *field )
{
    Qt::MouseButtons buttons = event->buttons();
    Qt::MouseButtons clicked = event->button();
    QString selected = field->selectedText();

    // Mouse chord, MB1 first - copy if text is selected, otherwise paste
    if (( clicked == Qt::RightButton ) && ( buttons & Qt::LeftButton ))
    {
        if ( selected.isEmpty() )
            doPaste( field );
        else
            doCopy( selected, field );
    }

    // Mouse chord, MB2 first - paste only
    else if (( clicked == Qt::LeftButton ) && ( buttons & Qt::RightButton ))
    {
        doPaste( field );
    }

    // MB3 clicked - paste only if platform supports selection clipboard (X11)
    else if (( clicked == Qt::MidButton ) && QApplication::clipboard()->supportsSelection() )
    {
        doPaste( field  );
    }
}


