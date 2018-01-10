#ifndef GOTOLINEDIALOG_H
#define GOTOLINEDIALOG_H

#include <QDialog>
#include "ui_gotolinedialog.h"


class GoToLineDialog : public QDialog, public Ui::GoToLineDialog
{
    Q_OBJECT

public:
    GoToLineDialog( QWidget *parent = 0 );
    GoToLineDialog( QWidget *parent, const int min, const int max, const int initial );

private slots:
    void on_lineEdit_textChanged();

};

#endif
