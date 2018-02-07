/******************************************************************************
** os2native.cpp
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

#define INCL_WIN
#include <os2.h>

#include "os2native.h"

#define DID_NEWTYPE_CB  290

//
// [BEGIN]
// This code (with some modifications) taken from the OS/2 Firefox sources:
// https://github.com/bitwiseworks/mozilla-os2/blob/master/widget/os2/nsFilePicker.cpp

typedef struct _FdData
{
   PAPSZ papszIFilterList;
   ULONG ulCurExt;
   ULONG ulNumFilters;
} FDDATA, *PFDDATA;


MRESULT EXPENTRY EnhancedFileDlgProc( HWND hwndDlg, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
    MRESULT  mr;
    PFILEDLG pfiledlg;
    HWND     hwndTypeCombo;
    INT      i;
    SWP      swp;
    PFDDATA  pfdextra;

    switch ( msg ) {
        case WM_INITDLG:
            mr = WinDefFileDlgProc( hwndDlg, msg, mp1, mp2 );

            // Hide the normal (EA-based) Type combobox
            hwndTypeCombo = WinWindowFromID( hwndDlg, DID_FILTER_CB );
            WinQueryWindowPos( hwndTypeCombo, &swp );
            WinSetWindowPos( hwndTypeCombo, NULLHANDLE, 0, 0, 0, 0, SWP_HIDE );

            // Now create our replacement Type combobox in the same position
            hwndTypeCombo = WinCreateWindow( hwndDlg, (PCSZ) WC_COMBOBOX, (PCSZ) "",
                                             WS_VISIBLE | WS_PARENTCLIP | WS_SYNCPAINT | WS_TABSTOP | CBS_DROPDOWNLIST,
                                             swp.x, swp.y, swp.cx, swp.cy,
                                             hwndDlg, swp.hwndInsertBehind, DID_NEWTYPE_CB,
                                             NULL, NULL );
            WinSendMsg( hwndTypeCombo, LM_DELETEALL, (MPARAM)0, (MPARAM)0 );
            pfiledlg = (PFILEDLG) WinQueryWindowULong( hwndDlg, QWL_USER );
            pfdextra = (PFDDATA)pfiledlg->ulUser;
            i = 0;
            while ( *(pfiledlg->papszITypeList[i]) != NULL ) {
                WinSendMsg( hwndTypeCombo, LM_INSERTITEM,
                            (MPARAM)LIT_END, (MPARAM)*( pfiledlg->papszITypeList[i] ));
                i++;
            }
            WinSendMsg( hwndTypeCombo, LM_SELECTITEM, (MPARAM)pfdextra->ulCurExt, (MPARAM)TRUE );
            return mr;

        case WM_CONTROL:
            if (( SHORT1FROMMP( mp1 ) == DID_NEWTYPE_CB ) && ( SHORT2FROMMP( mp1 ) == CBN_LBSELECT ))
            {
                hwndTypeCombo = WinWindowFromID(hwndDlg, DID_NEWTYPE_CB );
                pfiledlg = (PFILEDLG) WinQueryWindowULong( hwndDlg, QWL_USER );
                pfdextra = (PFDDATA)pfiledlg->ulUser;
                pfdextra->ulCurExt = (ULONG) WinSendMsg( hwndTypeCombo, LM_QUERYSELECTION,
                                                         (MPARAM)LIT_FIRST, (MPARAM)0 );
                if (pfiledlg->fl & FDS_OPEN_DIALOG) {
                    WinSetWindowText(WinWindowFromID(hwndDlg,DID_FILENAME_ED), *(pfdextra->papszIFilterList[pfdextra->ulCurExt]));
                    WinSendMsg(WinWindowFromID(hwndDlg,DID_FILENAME_ED), EM_SETSEL, MPFROM2SHORT(0, 32000), (MPARAM)0 );
                    WinSendMsg(hwndDlg, WM_CONTROL, MPFROM2SHORT(DID_FILTER_CB, CBN_LBSELECT), (MPARAM)0 );
                }
                return (MRESULT)TRUE;
            }
            break;

        default: break;
    }
    return WinDefFileDlgProc( hwndDlg, msg, mp1, mp2 );
}

// This code (with some modifications) taken from the OS/2 Firefox sources.
// [END]
//


void EnhanceFileDialog( PFILEDLG pfd, QString filter, QString *selectedFilter )
{
    PFDDATA pfdextra;
    LONG    i;
    PSZ     *apszTypeList,
            *apszFilterList;
    QStringList typeList,
                filterList;

    pfdextra = (PFDDATA) calloc( 1, sizeof( FDDATA ));

    pfd->pfnDlgProc = EnhancedFileDlgProc;
    pfd->ulUser     = (ULONG) pfdextra;

    if ( !( filter.isNull() || filter.isEmpty() )) {
        QStringList filters = filter.split( QRegExp(";;"), QString::SkipEmptyParts );
        QRegExp re("^([^\\(]*)\\(([^\\)]*)\\)", Qt::CaseSensitive, QRegExp::RegExp );
        for ( int i = 0; i < filters.size(); i++ ) {
            if ( selectedFilter && selectedFilter->compare( filters[ i ] ) == 0 )
                pfdextra->ulCurExt = i;
            if ( re.indexIn( filters[ i ], 0 ) != -1 ) {
                typeList.append( filters[ i ] ); //re.cap( 1 ).trimmed();
                filterList.append( re.cap( 2 ).replace(' ', ';'));
            }
        }
    }

    apszTypeList = (PSZ *) malloc( typeList.size() * sizeof( PSZ ) + 1 );
    for ( i = 0; i < typeList.size(); i++ )
        apszTypeList[ i ] = (PSZ) strdup( typeList.at( i ).toLocal8Bit().data() );
    apszTypeList[ i ] = NULL;
    pfd->papszITypeList = (PAPSZ) apszTypeList;
    if ( selectedFilter && !selectedFilter->isEmpty() )
        pfd->pszIType = (PSZ) strdup( selectedFilter->toLocal8Bit().data() );

    apszFilterList = (PSZ *) malloc( filterList.size() * sizeof( PSZ ) + 1 );
    for ( i = 0; i < filterList.size(); i++ )
        apszFilterList[ i ] = (PSZ) strdup( filterList.at( i ).toLocal8Bit().data() );
    apszFilterList[ i ] = NULL;
    pfdextra->papszIFilterList = (PAPSZ) apszFilterList;
}


void FreeFileDialogData( PFILEDLG pfd )
{
    LONG i;

    if ( pfd->papszITypeList ) {
        for ( i = 0; *(pfd->papszITypeList[ i ]) != NULL; i++ )
            free( *(pfd->papszITypeList[ i ] ));
        free( pfd->papszITypeList );
    }
    if ( pfd->ulUser ) {
        PFDDATA pfdu = (PFDDATA)pfd->ulUser;
        for ( i = 0; *(pfdu->papszIFilterList[ i ]) != NULL; i++ )
            free( *(pfdu->papszIFilterList[ i ] ));
        free( pfdu->papszIFilterList );
        free( pfdu );
    }
    if ( pfd->pszIType )
        free( pfd->pszIType );
    if ( pfd->pszTitle )
        free( pfd->pszTitle );
}


QString OS2Native::getOpenFileName(       QWidget *parent,
                                    const QString &caption,
                                    const QString &dir,
                                    const QString &filter,
                                          QString *selectedFilter,
                                          QFileDialog::Options options )
{
    HWND    hwndFD;
    FILEDLG fd;
    QString selected;

    memset( &fd, 0, sizeof(fd) );
    fd.cbSize   = sizeof( FILEDLG );
    fd.fl       = FDS_CENTER | FDS_OPEN_DIALOG;

    if ( !caption.isNull() && !caption.isEmpty() )
        fd.pszTitle = (PSZ) strdup( caption.toLocal8Bit().data() );
    strncpy( fd.szFullFile, QDir::toNativeSeparators( dir ).toLocal8Bit().constData(), CCHMAXPATH-1 );
    strncat( fd.szFullFile, "\\", CCHMAXPATH-1 );

    EnhanceFileDialog( &fd, filter, selectedFilter );

    hwndFD = WinFileDlg( HWND_DESKTOP, parent? parent->winId(): HWND_DESKTOP, &fd );
    if ( hwndFD ) {
        if (( fd.lReturn == DID_OK ) && fd.ulFQFCount )
            selected = QString::fromLocal8Bit( fd.szFullFile );
        FreeFileDialogData( &fd );
    }
    return selected;
}


QStringList	OS2Native::getOpenFileNames(       QWidget *parent,
                                         const QString &caption,
                                         const QString &dir,
                                         const QString &filter,
                                               QString *selectedFilter,
                                               QFileDialog::Options options )
{
    HWND        hwndFD;
    FILEDLG     fd;
    QStringList selected;

    memset( &fd, 0, sizeof(fd) );
    fd.cbSize   = sizeof( FILEDLG );
    fd.fl       = FDS_CENTER | FDS_OPEN_DIALOG | FDS_MULTIPLESEL;

    if ( !caption.isNull() && !caption.isEmpty() )
        fd.pszTitle = (PSZ) strdup( caption.toLocal8Bit().data() );
    strncpy( fd.szFullFile, QDir::toNativeSeparators( dir ).toLocal8Bit().constData(), CCHMAXPATH-1 );
    strncat( fd.szFullFile, "\\", CCHMAXPATH-1 );

    EnhanceFileDialog( &fd, filter, selectedFilter );

    hwndFD = WinFileDlg( HWND_DESKTOP, parent? parent->winId(): HWND_DESKTOP, &fd );
    if ( hwndFD ) {
        if (( fd.lReturn == DID_OK ) && fd.ulFQFCount ) {
            for ( ULONG i = 0; i < fd.ulFQFCount; i++ ) {
                selected << QString::fromLocal8Bit( (char *)(fd.papszFQFilename[i]) );
            }
            WinFreeFileDlgList( fd.papszFQFilename );
        }
        FreeFileDialogData( &fd );
    }
    return selected;
}


QString OS2Native::getSaveFileName(       QWidget *parent,
                                    const QString &caption,
                                    const QString &dir,
                                    const QString &filter,
                                          QString *selectedFilter,
                                          QFileDialog::Options options )
{
    HWND    hwndFD;
    FILEDLG fd;
    QString selected;

    memset( &fd, 0, sizeof(fd) );
    fd.cbSize     = sizeof( FILEDLG );
    fd.fl         = FDS_CENTER | FDS_SAVEAS_DIALOG | FDS_ENABLEFILELB;

    if ( !caption.isNull() && !caption.isEmpty() )
        fd.pszTitle = (PSZ) strdup( caption.toLocal8Bit().data() );
    strncpy( fd.szFullFile, QDir::toNativeSeparators( dir ).toLocal8Bit().constData(), CCHMAXPATH-1 );
    strncat( fd.szFullFile, "\\", CCHMAXPATH-1 );

    EnhanceFileDialog( &fd, filter, selectedFilter );

    hwndFD = WinFileDlg( HWND_DESKTOP, parent? parent->winId(): HWND_DESKTOP, &fd );
    if ( hwndFD ) {
        if (( fd.lReturn == DID_OK ) && fd.ulFQFCount )
            selected = QString::fromLocal8Bit( fd.szFullFile );
        FreeFileDialogData( &fd );
    }
    return selected;
}



