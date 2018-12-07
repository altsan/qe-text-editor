/******************************************************************************
** QE - replacedialog.cpp
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
#include "replacedialog.h"
#include "ctlutils.h"


ReplaceDialog::ReplaceDialog( QWidget *parent )
    : QDialog( parent )
{
    setupUi( this );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( close() ));
    findEdit->lineEdit()->installEventFilter( this );
    findEdit->installEventFilter( this );
    replaceEdit->lineEdit()->installEventFilter( this );
    replaceEdit->installEventFilter( this );
}


void ReplaceDialog::show()
{
    findEdit->setFocus();
    QDialog::show();
}


void ReplaceDialog::doReplace()
{
    on_replaceButton_clicked();
}


void ReplaceDialog::on_findEdit_editTextChanged( const QString &text )
{
    findButton->setEnabled( !text.isEmpty() );
    replaceButton->setEnabled( !text.isEmpty() );
    replaceAllButton->setEnabled( !text.isEmpty() );
}


void ReplaceDialog::on_findEdit_currentIndexChanged( int index )
{
    findEdit->lineEdit()->selectAll();
}


void ReplaceDialog::on_replaceEdit_currentIndexChanged( int index )
{
    replaceEdit->lineEdit()->selectAll();
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


void ReplaceDialog::on_findButton_clicked()
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
}


void ReplaceDialog::on_replaceButton_clicked()
{
    QString text = findEdit->currentText();
    QString replacement = replaceEdit->currentText();
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
    QString text = findEdit->currentText();
    QString replacement = replaceEdit->currentText();
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

    if (( target == findEdit->lineEdit() ) || ( target == replaceEdit->lineEdit() )) {
        // Support clipboard mouse actions in the entryfield
        if ( event->type() == QEvent::MouseButtonPress ) {
            QLineEdit *lineEdit = static_cast<QLineEdit *>(target);
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            mouseAction( mouseEvent, lineEdit );
        }
    }
    else if (( target == findEdit ) || ( target == replaceEdit )) {
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


void ReplaceDialog::setFindText( const QString &text )
{
    findEdit->lineEdit()->setText( text );
    findEdit->lineEdit()->selectAll();
}


void ReplaceDialog::populateHistory( const QStringList &findHistory, const QStringList &replaceHistory )
{
    findEdit->clear();
    findEdit->addItem("");
    findEdit->addItems( findHistory );
    findEdit->clearEditText();
    replaceEdit->clear();
    replaceEdit->addItem("");
    if ( replaceHistory.count() > 0 )
        replaceEdit->addItems( replaceHistory );
    replaceEdit->clearEditText();
}

