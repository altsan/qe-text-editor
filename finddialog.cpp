#include <QtGui>
#include "finddialog.h"


FindDialog::FindDialog( QWidget *parent )
    : QDialog( parent )
{
    setupUi( this );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( close() ));
}


void FindDialog::doFind()
{
    on_findButton_clicked();
}


void FindDialog::on_findEdit_textChanged( const QString &text )
{
    findButton->setEnabled( !text.isEmpty() );
}


void FindDialog::on_reCheckBox_toggled( bool checked )
{
    wordCheckBox->setEnabled( !checked );
}


void FindDialog::on_backCheckBox_toggled( bool checked )
{
    startCheckBox->setText( checked? tr("&From end of file"):
                                     tr("&From start of file") );
}


void FindDialog::on_findButton_clicked()
{
    QString text  = findEdit->text();
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



