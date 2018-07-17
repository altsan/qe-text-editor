/******************************************************************************
** QE - iodialog.h
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

#ifndef QE_IODIALOG_H
#define QE_IODIALOG_H

#include <QDialog>
#include <QLabel>

class IoDialog : public QDialog
{
    Q_OBJECT

public:
    IoDialog( QString text, QWidget *parent = 0 );

signals:
    void abortOpen();

private slots:
    void cancelClicked();

private:
    QLabel      *label;
    QPushButton *cancelButton;

};

#endif
