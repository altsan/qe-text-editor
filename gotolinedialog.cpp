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



