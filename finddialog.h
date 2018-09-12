/******************************************************************************
** QE - finddialog.h
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

#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include "ui_finddialog.h"

class FindDialog : public QDialog, public Ui::FindDialog
{
    Q_OBJECT

public:
    FindDialog( QWidget *parent = 0 );
    void populateText( const QString &findString, const QStringList &findHistory );
    void doFind();

signals:
    void findNext( const QString &str, bool cs, bool words, bool absolute );
    void findNextRegExp( const QString &str, bool cs, bool absolute );
    void findPrevious( const QString &str, bool cs, bool words, bool absolute );
    void findPreviousRegExp( const QString &str, bool cs, bool absolute );

private slots:
    void on_findEdit_textChanged( const QString &text );
    void on_reCheckBox_toggled( bool checked );
    void on_backCheckBox_toggled( bool checked );
    void on_findButton_clicked();

private:
    bool eventFilter( QObject *target, QEvent *event );

};

#endif
