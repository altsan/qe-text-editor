/******************************************************************************
** QE - iodialog.cpp
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
#include "iodialog.h"

IoDialog::IoDialog( QString text, QWidget *parent )
{
    label = new QLabel( text );
    cancelButton = new QPushButton( tr("Cancel"));
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( cancelClicked() ));

    QVBoxLayout *vblayout = new QVBoxLayout;
    QHBoxLayout *hblayout = new QHBoxLayout;
    hblayout->addWidget( cancelButton );
    hblayout->addStretch();
    vblayout->addWidget( label );
    vblayout->setSpacing( 8 );
    vblayout->addLayout( hblayout );

    setWindowFlags( Qt::SubWindow );
    setLayout( vblayout );
}


void IoDialog::cancelClicked()
{
    emit abortOpen();
    close();
}
