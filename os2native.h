/******************************************************************************
** os2native.h
**
** (C) 2018 Alexander Taylor
** Portions of this code are based on work from the Mozilla for OS/2 Project.
**
** This Source Code Form is subject to the terms of the Mozilla Public
** License, v. 2.0. If a copy of the MPL was not distributed with this
** file, You can obtain one at http://mozilla.org/MPL/2.0/.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
*******************************************************************************/

#ifndef QE_OS2NATIVE_H
#define QE_OS2NATIVE_H

#include <QFileDialog>


#define QSTRING_TO_PSZ( qs )      ( (PSZ) (qs.toLocal8Bit().data()) )
#define QSTRING_TO_PCSZ( qs )     ( (PCSZ)(qs.toLocal8Bit().constData()) )


namespace OS2Native {
    QString    	getOpenFileName( QWidget *parent = 0, const QString & caption = QString(), const QString & dir = QString(), const QString & filter = QString(), QString * selectedFilter = 0, QFileDialog::Options options = 0 );
    QStringList	getOpenFileNames( QWidget *parent = 0, const QString & caption = QString(), const QString & dir = QString(), const QString & filter = QString(), QString * selectedFilter = 0, QFileDialog::Options options = 0 );
    QString	    getSaveFileName( QWidget *parent = 0, const QString & caption = QString(), const QString & dir = QString(), const QString & filter = QString(), QString * selectedFilter = 0, QFileDialog::Options options = 0 );
    void        buildFilterString( QString filter, char *pszBuffer, ULONG ulBufSize );

    bool           createDesktopObject( const char *pcszClass, const char *pcszTitle, const char *pcszSetup, const char *pcszLocation, bool bReplace=false );
    unsigned long  deleteEA( char *pszPathName, const char *pszEAName );
    bool           destroyDesktopObject( const char *pcszObject );
    QString        getFontForLocale( const QString &locale = QString() );
    int            getSystemFontSize( void );
    unsigned short getWindowId( QWidget *window );
    void           setFrameIcon( QWidget *window, void *module, unsigned short usID );
    void         * setNativeHelp( QWidget *parent, const QString &help_library, const QString &help_title );
    void           destroyNativeHelp( void *help_instance );
    void           showHelpPanel( void *help_instance, unsigned short id );

};

#endif

