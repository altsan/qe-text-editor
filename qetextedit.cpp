/******************************************************************************
** QE - qetextedit.cpp
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

#include <QtGui>
#include "qetextedit.h"

// This subclass of QPlainTextEdit reimplements the mousePressEvent in order
// to handle OS/2-style clipboard mouse actions:
//   - Click RMB while holding down MB1 to highlight text: copy
//   - Click LMB and RMB simultaneously when there is text in the clipboard: paste
//
// It should also handle MB3 paste on X11 (only).
//
// Finally, it doesn't paste the names of dropped files but lets the parent
// handle them (e.g. by opening them).
//

QeTextEdit::QeTextEdit( QWidget *parent )
    : QPlainTextEdit( parent )
{
}


void QeTextEdit::mousePressEvent( QMouseEvent *event )
{
    Qt::MouseButtons buttons = event->buttons();
    Qt::MouseButtons clicked = event->button();
    QTextCursor cursor = textCursor();
    QString selected = cursor.selectedText();

    // Mouse chord, MB1 first - copy if text is selected, otherwise paste
    if (( clicked == Qt::RightButton ) && ( buttons & Qt::LeftButton ))
    {
        if ( selected.isEmpty() )
            doPaste( cursor );
        else
            doCopy( selected, cursor );
        setTextCursor( cursor );
    }

    // Mouse chord, MB2 first - paste only
    else if (( clicked == Qt::LeftButton ) && ( buttons & Qt::RightButton ))
    {
        doPaste( cursor );
    }

    // MB3 clicked - paste only if platform supports selection clipboard (X11)
    else if (( clicked == Qt::MidButton ) && QApplication::clipboard()->supportsSelection() )
    {
        doPaste( cursor );
    }

    QPlainTextEdit::mousePressEvent( event );
}


void QeTextEdit::doCopy( QString text, QTextCursor cursor )
{
#if 0
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText( text, clipboard->supportsSelection() ?
                                QClipboard::Selection :
                                QClipboard::Clipboard );
    cursor.clearSelection();
    setTextCursor( cursor );
#else
    QPlainTextEdit::copy();
#endif
}


void QeTextEdit::doPaste( QTextCursor cursor )
{
#if 0
    QClipboard *clipboard = QApplication::clipboard();
    QString text = clipboard->supportsSelection() ?
                       clipboard->text( QClipboard::Selection ) :
                       clipboard->text( QClipboard::Clipboard );
    cursor.insertText( text );
    setTextCursor( cursor );
#else
    QPlainTextEdit::paste();
#endif
}



void QeTextEdit::dropEvent( QDropEvent *event )
{
    // Don't handle dropped filenames (the owner will take care of them)
    if ( event->mimeData()->hasFormat("text/uri-list")) {
        QList<QUrl> urls = event->mimeData()->urls();
        if ( !urls.isEmpty() ) {
            QString fileName = urls.first().toLocalFile();
            if ( !fileName.isEmpty() ) return;
        }
    }
    // Let the default event handler take care of everything else
    QPlainTextEdit::dropEvent( event );
}


