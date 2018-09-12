/******************************************************************************
** QE - main.cpp
**
**  Copyright (C) 2018 Alexander Taylor
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
******************************************************************************/

#include <QApplication>
#include <QDir>
#include <QFileInfo>

#include "mainwindow.h"

#ifdef Q_OS_WIN32
#include <windows.h>
#endif

int main( int argc, char *argv[] )
{
    QApplication app( argc, argv );
    MainWindow *qe = new MainWindow;
    bool openReadOnly = false;
    bool showUsage    = false;
    bool openEncoding = false;
    QString encoding;
    QString fileName;

    for ( int a = 1; a < argc; a++ ) {
        char *psz = argv[ a ];
        if ( *psz == '/' || *psz == '-') {
            QString argStr( ++psz );
            if ( argStr.isEmpty() || argStr.isNull() )
                continue;
            else if ( argStr.compare( QString("read"), Qt::CaseInsensitive ) == 0 )
                openReadOnly = true;
            else if ( argStr.compare( QString("xxxx"), Qt::CaseInsensitive ) == 0 )
                openReadOnly = true;
            else if ( argStr.startsWith( QString("cp:"), Qt::CaseInsensitive ) == 1 ) {
                openEncoding = true;
                encoding = argStr;
                encoding.remove( 0, 3 );
            }
            else if ( argStr.startsWith( QString("enc:"), Qt::CaseInsensitive ) == 1 ) {
                openEncoding = true;
                encoding = argStr;
                encoding.remove( 0, 4 );
            }
            else if (( argStr.compare( QString("?")) == 0 ) ||
                     ( argStr.compare( QString("h"), Qt::CaseInsensitive ) == 0 ))
                showUsage = true;
        }
        // Not a / switch, treat as a filename
        else if ( fileName.isNull() ) {
#ifdef Q_OS_WIN32
            int t;
            LPWSTR *wstr = CommandLineToArgvW( GetCommandLine(), &t );
            QFileInfo fileinfo( QString::fromWCharArray( wstr[a] ));
#else
            QFileInfo fileinfo( QString::fromLocal8Bit( psz ));
#endif
            if ( fileinfo.canonicalFilePath().isEmpty() ) {
                QDir dir( QDir::currentPath() );
                fileName = QDir::cleanPath( dir.filePath( psz ));
            }
            else
                fileName = fileinfo.canonicalFilePath();
        }
    }

    if ( showUsage ) {
        qe->showUsage();
        return 0;
    }

    if ( !fileName.isNull() ) {
        if ( openEncoding )
            qe->openAsEncoding( fileName, true, encoding );
        else
            qe->loadFile( fileName, true );
    }
    else if ( openEncoding && qe->mapNameToEncoding( encoding )) {
        qe->setTextEncoding( encoding );
    }

    qe->setReadOnly( openReadOnly );
    qe->show();

    return app.exec();
}
