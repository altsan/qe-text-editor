/******************************************************************************
** QE - replacedialog.cpp
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
#include "replacedialog.h"
#include "ctlutils.h"


ReplaceDialog::ReplaceDialog( QWidget *parent )
    : QDialog( parent )
{
    setupUi( this );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( close() ));
    findEdit->installEventFilter( this );
    replaceEdit->installEventFilter( this );
}


void ReplaceDialog::doReplace()
{
    on_replaceButton_clicked();
}


void ReplaceDialog::on_findEdit_textChanged( const QString &text )
{
    replaceButton->setEnabled( !text.isEmpty() );
    replaceAllButton->setEnabled( !text.isEmpty() );
}


void ReplaceDialog::on_reCheckBox_toggled( bool checked )
{
    wordCheckBox->setEnabled( !checked );
}


void ReplaceDialog::on_backCheckBox_toggled( bool checked )
{
    startCheckBox->setText( checked? tr("Fro&m end of file"):
                                     tr("Fro&m start of file") );
}


void ReplaceDialog::on_replaceButton_clicked()
{
    QString text = findEdit->text();
    QString replacement = replaceEdit->text();
    bool cs = caseCheckBox->isChecked();
    bool words = wordCheckBox->isChecked();
    bool absolute = startCheckBox->isChecked();
    bool confirm = verifyCheckBox->isChecked();

    if ( text.isNull() ) return;

    if ( backCheckBox->isChecked() ) {
        emit reCheckBox->isChecked() ? replacePreviousRegExp( text, replacement, cs, absolute, confirm ) :
                                       replacePrevious( text, replacement, cs, words, absolute, confirm );
    } else {
        emit reCheckBox->isChecked() ? replaceNextRegExp( text, replacement, cs, absolute, confirm ) :
                                       replaceNext( text, replacement, cs, words, absolute, confirm );
    }
    startCheckBox->setChecked( false );
}


void ReplaceDialog::on_replaceAllButton_clicked()
{
    QString text = findEdit->text();
    QString replacement = replaceEdit->text();
    bool cs = caseCheckBox->isChecked();
    bool words = wordCheckBox->isChecked();
    bool absolute = startCheckBox->isChecked();
    bool confirm = verifyCheckBox->isChecked();
    bool backwards = backCheckBox->isChecked();

    if ( text.isNull() ) return;
    emit reCheckBox->isChecked() ? replaceAllRegExp( text, replacement, cs, absolute, confirm, backwards ) :
                                   replaceAll( text, replacement, cs, words, absolute, confirm, backwards );
}


bool ReplaceDialog::eventFilter( QObject *target, QEvent *event )
{
    bool ok = QDialog::eventFilter( target, event );

    if ((( target == findEdit ) || ( target == replaceEdit )) &&
        ( event->type() == QEvent::MouseButtonPress ))
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        QLineEdit *lineEdit = static_cast<QLineEdit *>(target);
        mouseAction( mouseEvent, lineEdit );
    }
    return ok;
}


