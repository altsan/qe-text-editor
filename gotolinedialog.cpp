/******************************************************************************
** QE - gotolinedialog.cpp
** The "go to line" dialog.
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
#include "gotolinedialog.h"


GoToLineDialog::GoToLineDialog( QWidget *parent )
    : QDialog( parent )
{
    setupUi( this );
    QIntValidator *validator = new QIntValidator( this );
    lineEdit->setValidator( validator );
    lineEdit->setText("1");
    lineEdit->selectAll();
    connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ));
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ));
}

GoToLineDialog::GoToLineDialog( QWidget *parent, int min, int max, int initial )
    : QDialog( parent )
{
    setupUi( this );
    QIntValidator *validator = new QIntValidator( this );
    validator->setRange( min, max );
    lineEdit->setValidator( validator );
    lineEdit->setText( tr("%1").arg( initial ));
    lineEdit->selectAll();
    connect( okButton, SIGNAL( clicked() ), this, SLOT( accept() ));
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( reject() ));
}


void GoToLineDialog::on_lineEdit_textChanged()
{
    okButton->setEnabled( lineEdit->hasAcceptableInput() );
}



