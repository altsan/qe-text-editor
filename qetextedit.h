/******************************************************************************
** QE - qetextedit.h
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

#ifndef QETEXTEDIT_H
#define QETEXTEDIT_H

#include <QWidget>
#include <QPlainTextEdit>

class QeTextEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    QeTextEdit( QWidget *parent = 0 );
    void mousePressEvent( QMouseEvent *event );
    void contextMenuEvent( QContextMenuEvent *event );

protected:
    void dropEvent( QDropEvent *event );

private slots:
    void copy();
    void cut();
    void paste();

private:
    bool isChording;

};

#endif
