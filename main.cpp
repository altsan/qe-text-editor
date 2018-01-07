#include <QApplication>
#include <QFileInfo>

#include "mainwindow.h"

int main( int argc, char *argv[] )
{
    QApplication app( argc, argv );
    MainWindow *qe = new MainWindow;
    bool openReadOnly = false;
    bool showUsage     = false;
    QString fileName;

    for ( int a = 1; a < argc; a++ ) {
        char *psz = argv[ a ];
        if ( *psz == '/' || *psz == '-') {
            psz++;
            if ( !(*psz) ) continue;
            if (( *psz == 'R') || ( *psz == 'r')) {
                openReadOnly = true;
            }
            if (( *psz == 'H') || ( *psz == 'h') || ( *psz == '?')) {
                showUsage = true;
            }
        }
        else if ( fileName.isNull() ) {
            QFileInfo fileinfo( psz );
            fileName = fileinfo.canonicalFilePath();
        }
    }

    if ( showUsage ) {
        qe->showUsage();
        return 0;
    }
    if ( !fileName.isNull() )
        qe->loadFile( fileName );

    qe->setReadOnly( openReadOnly );
    qe->show();
    return app.exec();
}
