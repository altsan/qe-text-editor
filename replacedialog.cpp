#include <QtGui>
#include "replacedialog.h"


ReplaceDialog::ReplaceDialog( QWidget *parent )
    : QDialog( parent )
{
    setupUi( this );
    connect( cancelButton, SIGNAL( clicked() ), this, SLOT( close() ));
}


void ReplaceDialog::doReplace()
{
    on_replaceButton_clicked();
}


void ReplaceDialog::on_findEdit_textChanged( const QString &text )
{
    replaceButton->setEnabled( !text.isEmpty() );
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
    QString text  = findEdit->text();
    bool cs       = caseCheckBox->isChecked();
    bool words    = wordCheckBox->isChecked();
    bool absolute = startCheckBox->isChecked();
    if ( backCheckBox->isChecked() ) {
        emit reCheckBox->isChecked() ? replacePreviousRegExp( text, cs, absolute ) :
                                       replacePrevious( text, cs, words, absolute );
    } else {
        emit reCheckBox->isChecked() ? replaceNextRegExp( text, cs, absolute ) :
                                       replaceNext( text, cs, words, absolute );
    }
    startCheckBox->setChecked( false );
    if ( !keepCheckBox->isChecked() ) close();
}



