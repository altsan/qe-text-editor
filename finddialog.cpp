/******************************************************************************
** QE - finddialog.cpp
**
**  Copyright (C) 2018 Alexander Taylor
**  Some parts based on sample code from Blanchette & Summerfield, "C++ GUI
**  Programming with Qt4" (Second Edition), Pearson 2007.
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
#include "finddialog.h"
#include "ctlutils.h"


FindDialog::FindDialog( QWidget *parent )
    : QDialog( parent )
{
    setupUi( this );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( close() ));
    findEdit->lineEdit()->installEventFilter( this );
    findEdit->installEventFilter( this );
}


void FindDialog::show()
{
    findEdit->setFocus();
    QDialog::show();
}


void FindDialog::doFind()
{
    if ( !findEdit->currentText().isEmpty() )
        on_findButton_clicked();
}


void FindDialog::on_findEdit_editTextChanged( const QString &text )
{
    findButton->setEnabled( !text.isEmpty() );
}


void FindDialog::on_findEdit_currentIndexChanged( int index )
{
    findEdit->lineEdit()->selectAll();
}


void FindDialog::on_reCheckBox_toggled( bool checked )
{
    wordCheckBox->setEnabled( !checked );
}


void FindDialog::on_backCheckBox_toggled( bool checked )
{
    startCheckBox->setText( checked? tr("Fro&m end of file"):
                                     tr("Fro&m start of file") );
}


void FindDialog::on_findButton_clicked()
{
    QString text  = findEdit->currentText();
    bool cs       = caseCheckBox->isChecked();
    bool words    = wordCheckBox->isChecked();
    bool absolute = startCheckBox->isChecked();
    if ( backCheckBox->isChecked() ) {
        emit reCheckBox->isChecked() ? findPreviousRegExp( text, cs, absolute ) :
                                       findPrevious( text, cs, words, absolute );
    } else {
        emit reCheckBox->isChecked() ? findNextRegExp( text, cs, absolute ) :
                                       findNext( text, cs, words, absolute );
    }
    startCheckBox->setChecked( false );
    if ( !keepCheckBox->isChecked() ) close();
}


bool FindDialog::eventFilter( QObject *target, QEvent *event )
{
    bool ok = QDialog::eventFilter( target, event );

    if ( target == findEdit->lineEdit() ) {
        // Support clipboard mouse actions in the entryfield
        if ( event->type() == QEvent::MouseButtonPress ) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            mouseAction( mouseEvent, findEdit->lineEdit() );
        }
    }
    else if ( target == findEdit ) {
        // Prevent QComboBox from intercepting Enter, let the dialog handle it
        if ( event->type() == QEvent::KeyPress ) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            int pressedKey = keyEvent->key();
            if (( pressedKey == Qt::Key_Return ) || ( pressedKey == Qt::Key_Enter )) {
                keyPressEvent( keyEvent );
                ok = true;
            }
        }
    }
    return ok;
}


void FindDialog::setFindText( const QString &text )
{
    findEdit->lineEdit()->setText( text );
    findEdit->lineEdit()->selectAll();
}


void FindDialog::populateHistory( const QStringList &findHistory )
{
    findEdit->clear();
    findEdit->addItem("");
    findEdit->addItems( findHistory );
    findEdit->clearEditText();
}


