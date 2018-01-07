#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include "ui_replacedialog.h"

class ReplaceDialog : public QDialog, public Ui::ReplaceDialog
{
    Q_OBJECT

public:
    ReplaceDialog( QWidget *parent = 0 );
    void doReplace();

signals:
    void replaceNext( const QString &str, bool cs, bool words, bool absolute );
    void replaceNextRegExp( const QString &str, bool cs, bool absolute );
    void replacePrevious( const QString &str, bool cs, bool words, bool absolute );
    void replacePreviousRegExp( const QString &str, bool cs, bool absolute );

private slots:
    void on_findEdit_textChanged( const QString &text );
    void on_reCheckBox_toggled( bool checked );
    void on_backCheckBox_toggled( bool checked );
    void on_replaceButton_clicked();

};

#endif
