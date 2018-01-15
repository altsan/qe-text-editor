/******************************************************************************
** QE - replacedialog.h
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

#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QDialog>
#include "ui_replacedialog.h"

class ReplaceDialog : public QDialog, public Ui::ReplaceDialog
{
    Q_OBJECT

public:
    ReplaceDialog( QWidget *parent = 0 );
    void doReplace();

signals:
    void replaceNext( const QString &str, const QString &repl, bool cs, bool words, bool absolute, bool confirm );
    void replaceNextRegExp( const QString &str, const QString &repl, bool cs, bool absolute, bool confirm );
    void replacePrevious( const QString &str, const QString &repl, bool cs, bool words, bool absolute, bool confirm );
    void replacePreviousRegExp( const QString &str, const QString &repl, bool cs, bool absolute, bool confirm );
    void replaceAll( const QString &str, const QString &repl, bool cs, bool words, bool fromStart, bool confirm, bool backwards );
    void replaceAllRegExp( const QString &str, const QString &repl, bool cs, bool fromStart, bool confirm, bool backwards );

private slots:
    void on_findEdit_textChanged( const QString &text );
    void on_reCheckBox_toggled( bool checked );
    void on_backCheckBox_toggled( bool checked );
    void on_replaceButton_clicked();
    void on_replaceAllButton_clicked();

};

#endif
