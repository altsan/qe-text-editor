#include <QApplication>

#include "mainwindow.h"

int main( int argc, char *argv[] )
{
    QApplication app( argc, argv );
    MainWindow *qe = new MainWindow;
    qe->show();
    return app.exec();
}
