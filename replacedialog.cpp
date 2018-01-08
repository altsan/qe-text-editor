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
