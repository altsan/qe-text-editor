#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include "ui_finddialog.h"

class FindDialog : public QDialog, public Ui::FindDialog
{
    Q_OBJECT

public:
    FindDialog( QWidget *parent = 0 );

signals:
    void findNext( const QString &str, Qt::CaseSensitivity cs, bool absolute );
    void findNextRegExp( const QString &str, bool absolute );
    void findPrevious( const QString &str, Qt::CaseSensitivity cs, bool absolute );
    void findPreviousRegExp( const QString &str, bool absolute );

private slots:
    void on_findEdit_textChanged( const QString &text );
    void on_reCheckBox_toggled( bool checked );
    void on_backCheckBox_toggled( bool checked );
    void on_findButton_clicked();

/*
private:
    QLabel      *findLabel;
    QLineEdit   *findEdit;
    QCheckBox   *reCheckBox;
    QCheckBox   *caseCheckBox;
    QCheckBox   *backCheckBox;
    QCheckBox   *startCheckBox;
    QPushButton *findButton;
    QPushButton *closeButton;
*/

};

#endif
