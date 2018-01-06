#include <QtGui>
#include "finddialog.h"


FindDialog::FindDialog( QWidget *parent )
    : QDialog( parent )
{
    setupUi( this );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( close() ));
}


void FindDialog::on_findEdit_textChanged( const QString &text )
{
    findButton->setEnabled( !text.isEmpty() );
}


void FindDialog::on_reCheckBox_toggled( bool checked )
{
    caseCheckBox->setEnabled( !checked );
}


void FindDialog::on_backCheckBox_toggled( bool checked )
{
    startCheckBox->setText( checked? tr("&From end of file"):
                                     tr("&From start of file") );
}


void FindDialog::on_findButton_clicked()
{
    QString text = findEdit->text();
    Qt::CaseSensitivity cs = caseCheckBox->isChecked()? Qt::CaseSensitive :
                                                        Qt::CaseInsensitive;
    bool absolute = startCheckBox->isChecked();
    if ( backCheckBox->isChecked() ) {
        emit reCheckBox->isChecked() ? findPreviousRegExp( text, absolute ) :
                                       findPrevious( text, cs, absolute );
    } else {
        emit reCheckBox->isChecked() ? findNextRegExp( text, absolute ) :
                                       findNext( text, cs, absolute );
    }
    startCheckBox->setChecked( false );
}



