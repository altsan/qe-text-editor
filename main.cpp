#include <QApplication>
#include <QFileInfo>

#include "mainwindow.h"

int main( int argc, char *argv[] )
{
    QApplication app( argc, argv );
    MainWindow *qe = new MainWindow;
    bool openReadOnly = false;
    QString fileName;

    for ( int a = 1; ( a < argc ) && ( fileName.isNull() ); a++ ) {
        char *psz = argv[ a ];
        if ( *psz == '/' || *psz == '-') {
            psz++;
            if ( !(*psz) ) continue;
            if (( *psz == 'R') || ( *psz == 'r')) {
                openReadOnly = true;
            }
        }
        else {
            QFileInfo fileinfo( psz );
            fileName = fileinfo.canonicalFilePath();
        }
    }

    if ( !fileName.isNull() )
        qe->loadFile( fileName );

    qe->show();
    return app.exec();
}
