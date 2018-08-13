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
#define INCL_WINHELP
#include <os2.h>

#include <QMessageBox>
#include "os2native.h"


// ===========================================================================
// OS/2 Native File Dialog Logic
//

#define DID_NEWTYPE_CB  290

//
// [BEGIN]
// This code (with some modifications) taken from the Mozilla for OS/2 sources:
// https://github.com/bitwiseworks/mozilla-os2/blob/master/widget/os2/nsFilePicker.cpp

typedef struct _FdData
{
   PAPSZ papszIFilterList;
   ULONG ulCurExt;
   ULONG ulNumFilters;
} FDDATA, *PFDDATA;


// ---------------------------------------------------------------------------
// Dialog procedure for the subclassed OS/2 file dialog.
//
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

// This code (with some modifications) taken from the Mozilla for OS/2 sources.
// [END]
//


// ---------------------------------------------------------------------------
// Enhances the standard PM file dialog with a tweaked layout and custom
// dialog procedure.  Basically this adds a file-type filter drop-down in
// place of the rather useless .TYPE extended attribute drop-down, and
// populates it with the specified list of filters.  This, too, is largely
// inspired by the Mozilla/2 code, although the implementation is original.
//
void EnhanceFileDialog( PFILEDLG pfd, QString filter, QString *selectedFilter )
{
    PFDDATA pfdextra;
    int     i;
    PSZ     *apszTypeList,
            *apszFilterList;
    QStringList typeList,
                filterList;

    pfdextra = (PFDDATA) calloc( 1, sizeof( FDDATA ));
    pfd->fl        |= FDS_FILTERUNION;
    pfd->pfnDlgProc = EnhancedFileDlgProc;
    pfd->ulUser     = (ULONG) pfdextra;

    if ( !( filter.isNull() || filter.isEmpty() )) {
        QStringList filters = filter.split( QRegExp(";;"), QString::SkipEmptyParts );
        QRegExp re("\\(([^\\)]*)\\)", Qt::CaseSensitive, QRegExp::RegExp );
        for ( i = 0; i < filters.size(); i++ ) {
            if ( selectedFilter && selectedFilter->compare( filters[ i ] ) == 0 )
                pfdextra->ulCurExt = i;
            if ( re.indexIn( filters[ i ], 0 ) != -1 ) {
                typeList.append( filters[ i ] );
                filterList.append( re.cap( 1 ).replace(' ', ';'));
            }
        }
    }
    pfdextra->ulNumFilters = (ULONG) typeList.size();

    apszTypeList = (PSZ *) malloc( pfdextra->ulNumFilters * sizeof( PSZ ) + 1 );
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


// ---------------------------------------------------------------------------
void UpdateSelectedFilter( PFILEDLG pfd, QString *selectedFilter )
{
    if ( !selectedFilter || !pfd->ulUser ) return;
    PFDDATA pfdu = (PFDDATA)pfd->ulUser;
    *selectedFilter = QString::fromLocal8Bit( (char *)( *(pfd->papszITypeList[ pfdu->ulCurExt ] )));
}


// ---------------------------------------------------------------------------
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


// ---------------------------------------------------------------------------
// OS2Native::getOpenFileName
//
// Implements OS/2 native File Open dialog (single selection).  Input/output
// are the same as the standard Qt4 getOpenFileName function.
//
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
    fd.cbSize = sizeof( FILEDLG );
    fd.fl     = FDS_CENTER | FDS_OPEN_DIALOG;

    if ( !caption.isNull() && !caption.isEmpty() )
        fd.pszTitle = (PSZ) strdup( caption.toLocal8Bit().data() );
    strncpy( fd.szFullFile, QDir::toNativeSeparators( dir ).toLocal8Bit().constData(), CCHMAXPATH-1 );
    if ( !QString( fd.szFullFile ).endsWith('\\'))
        strncat( fd.szFullFile, "\\", CCHMAXPATH-1 );

    EnhanceFileDialog( &fd, filter, selectedFilter );

    hwndFD = WinFileDlg( HWND_DESKTOP, parent? parent->winId(): HWND_DESKTOP, &fd );
    if ( hwndFD ) {
        if (( fd.lReturn == DID_OK ) && fd.ulFQFCount ) {
            selected = QDir::fromNativeSeparators( QString::fromLocal8Bit( fd.szFullFile ));
            UpdateSelectedFilter( &fd, selectedFilter );
        }
        FreeFileDialogData( &fd );
    }
    else {
//        QMessageBox::critical( parent, QObject::tr("Dialog Failed"),
//                               QObject::tr("WinFileDlg failed with error %1").arg( fd.lSRC ),
//                               QMessageBox::Ok );
        return QFileDialog::getOpenFileName( parent, caption, dir, filter, selectedFilter, options );
    }

    return selected;
}


// ---------------------------------------------------------------------------
// OS2Native::getOpenFileNames
//
// Implements OS/2 native File Open dialog (multiple selection).  Input/output
// are the same as the standard Qt4 getOpenFileNames function.
//
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
    fd.cbSize = sizeof( FILEDLG );
    fd.fl     = FDS_CENTER | FDS_OPEN_DIALOG | FDS_MULTIPLESEL;

    if ( !caption.isNull() && !caption.isEmpty() )
        fd.pszTitle = (PSZ) strdup( caption.toLocal8Bit().data() );
    strncpy( fd.szFullFile, QDir::toNativeSeparators( dir ).toLocal8Bit().constData(), CCHMAXPATH-1 );
    if ( !QString( fd.szFullFile ).endsWith('\\'))
        strncat( fd.szFullFile, "\\", CCHMAXPATH-1 );

    EnhanceFileDialog( &fd, filter, selectedFilter );

    hwndFD = WinFileDlg( HWND_DESKTOP, parent? parent->winId(): HWND_DESKTOP, &fd );
    if ( hwndFD ) {
        if (( fd.lReturn == DID_OK ) && fd.ulFQFCount ) {
            for ( ULONG i = 0; i < fd.ulFQFCount; i++ ) {
                selected.append( QDir::fromNativeSeparators( QString::fromLocal8Bit( (char *)(fd.papszFQFilename[i]) )));
            }
            WinFreeFileDlgList( fd.papszFQFilename );
            UpdateSelectedFilter( &fd, selectedFilter );
        }
        FreeFileDialogData( &fd );
    }
    else {
        return QFileDialog::getOpenFileNames( parent, caption, dir, filter, selectedFilter, options );
    }
    return selected;
}


// ---------------------------------------------------------------------------
// OS2Native::getSaveFileName
//
// Implements OS/2 native File Save dialog.  Input/output are the same as the
// standard Qt4 getSaveFileName function.
//
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
    fd.cbSize = sizeof( FILEDLG );
    fd.fl     = FDS_CENTER | FDS_SAVEAS_DIALOG | FDS_ENABLEFILELB;

    if ( !caption.isNull() && !caption.isEmpty() )
        fd.pszTitle = (PSZ) strdup( caption.toLocal8Bit().data() );
    strncpy( fd.szFullFile, QDir::toNativeSeparators( dir ).toLocal8Bit().constData(), CCHMAXPATH-1 );
    if ( !QString( fd.szFullFile ).endsWith('\\'))
        strncat( fd.szFullFile, "\\", CCHMAXPATH-1 );

    EnhanceFileDialog( &fd, filter, selectedFilter );

    hwndFD = WinFileDlg( HWND_DESKTOP, parent? parent->winId(): HWND_DESKTOP, &fd );
    if ( hwndFD ) {
        if (( fd.lReturn == DID_OK ) && fd.ulFQFCount ) {
            selected = QDir::fromNativeSeparators( QString::fromLocal8Bit( fd.szFullFile ));
            UpdateSelectedFilter( &fd, selectedFilter );
        }
        FreeFileDialogData( &fd );
    }
    else {
        return QFileDialog::getSaveFileName( parent, caption, dir, filter, selectedFilter, options );
    }
    return selected;
}



// ===========================================================================
// OS/2 Native Help Logic
//


// ---------------------------------------------------------------------------
// OS2Native::setNativeHelp
//
// Enables OS/2 native (compiled IPF) help for an application window.  This
// function initializes the OS/2 Help Manager and associates the specified help
// library (i.e. .hlp file) with the specified window.  (The application should
// call OS2Native::destroyNativeHelp on shutdown/cleanup.)
//
// PARAMETERS:
//     QWidget *parent      : The application window to be help-enabled
//     QString &help_library: Filename of the help library (*.hlp)
//     QString &help_title  : Title to be displayed on the help window
//
// RETURNS: void *
//     Help instance handle which can be passed to other functions.
//
void *OS2Native::setNativeHelp(       QWidget *parent,
                                const QString &help_library,
                                const QString &help_title    )
{
    HAB      hab;
    HWND     hwndFrame,
             hwndHelp;
    HELPINIT hinit;

    if ( !parent ) return NULL;

    hwndFrame = parent->winId();
    if ( !hwndFrame ) return NULL;
    hab = WinQueryAnchorBlock( hwndFrame );
    if ( !hab ) hab = 1;

    memset( &hinit, 0, sizeof( HELPINIT ));
    hinit.cb = sizeof( HELPINIT );
    hinit.phtHelpTable = NULL;
    hinit.pszHelpWindowTitle = (PSZ)( help_title.toLocal8Bit().data() );
    hinit.pszHelpLibraryName = (PSZ)( QDir::toNativeSeparators( help_library ).toLocal8Bit().data() );
    hwndHelp = WinCreateHelpInstance( hab, &hinit );
    if ( hwndHelp )
        WinAssociateHelpInstance( hwndHelp, hwndFrame );

    return (PVOID) hwndHelp;
}


// ---------------------------------------------------------------------------
// OS2Native::destroyNativeHelp
//
// Destroys the specified OS/2 help instance. Should be called on application
// shutdown (or when application help is otherwise no longer needed).
//
// PARAMETERS:
//     void *help_instance: Help instance handle returned from setNativeHelp()
//
void OS2Native::destroyNativeHelp( void *help_instance )
{
    HWND hwndHelp = (HWND) help_instance;
    if ( !hwndHelp ) return;

    WinDestroyHelpInstance( hwndHelp );
}


// ---------------------------------------------------------------------------
// OS2Native::showHelpPanel
//
// Opens the associated help file and displays the panel with the specified ID.
//
// PARAMETERS:
//     void *help_instance: Help instance handle returned from setNativeHelp()
//     unsigned short id  : Requested help panel ID to be displayed
//
void OS2Native::showHelpPanel( void *help_instance, unsigned short id )
{
    HWND hwndHelp = (HWND) help_instance;
    if ( !hwndHelp ) return;

    WinSendMsg( hwndHelp, HM_DISPLAY_HELP,
                MPFROMSHORT( id ), MPFROMSHORT( HM_RESOURCEID ));
}



// ===========================================================================
// Other Useful Functions
//

// ---------------------------------------------------------------------------
// Get a widget's Presentation Manager window numeric identifier (resource ID).
//
unsigned short OS2Native::getWindowId( QWidget *window )
{
    HWND   hwnd;
    USHORT usID;

    if ( !window ) return 0;
    hwnd = window->winId();
    if ( !hwnd ) return 0;

    usID = WinQueryWindowUShort( hwnd, QWS_ID );
    return usID;
}

