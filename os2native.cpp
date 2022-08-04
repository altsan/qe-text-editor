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

#define INCL_DOSERRORS
#define INCL_DOSFILEMGR
#define INCL_DOSMISC
#define INCL_WIN
#define INCL_WINHELP
#include <os2.h>

#include <QFontDatabase>
#include <QLocale>
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
                //if (pfiledlg->fl & FDS_OPEN_DIALOG)
                {
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
// library (i.e. .hlp file) with the specified window.
//
// The help_library parameter can be specified with or without the ".hlp"
// extension.  If included, it will be used as-is for the help file name.  If
// omitted, this function will attempt to locate the help file by appending "_"
// plus the value of %LANG% (first in full, then just the language portion),
// plus the ".hlp" extension.  If a matching filename is not found for either
// of these, it will fall back to using just the parameter as-is plus ".hlp".
//
// The application should call OS2Native::destroyNativeHelp on shutdown/cleanup.
//
// PARAMETERS:
//     QWidget *parent       : The application window to be help-enabled
//     QString &help_library : Filename of the help library (*.hlp)
//     QString &help_title   : Title to be displayed on the help window
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
    QString  help_file;

    if ( !parent ) return NULL;

    hwndFrame = parent->winId();
    if ( !hwndFrame ) return NULL;
    hab = WinQueryAnchorBlock( hwndFrame );
    if ( !hab ) hab = 1;

    /* Check the help file name and adjust for language if appropriate */
    if ( help_library.endsWith(".hlp", Qt::CaseInsensitive ))
        // Full filename specified, just use that
        help_file = help_library;
    else {
        // Partial filename specified, try to derive full name based on language
        PSZ pszEnv = getenv("LANG");
        if ( pszEnv ) {
            CHAR    achFound[ CCHMAXPATH ] = {0};
            APIRET  rc;
            QString language = QString( pszEnv ).mid( 0, 5 );

            // See if a language-specific filename exists
            help_file = help_library + "_" + language + ".hlp";
            rc = DosSearchPath( SEARCH_IGNORENETERRS | SEARCH_ENVIRONMENT | SEARCH_CUR_DIRECTORY,
                                "HELP", QSTRING_TO_PSZ( help_file ), achFound, sizeof( achFound ));
            if ( rc != NO_ERROR ) {
                // Full match on %LANG% not found, try just the language portion
                language = language.mid( 0, language.indexOf("_"));
                help_file = help_library + "_" + language + ".hlp";
                rc = DosSearchPath( SEARCH_IGNORENETERRS | SEARCH_ENVIRONMENT | SEARCH_CUR_DIRECTORY,
                                    "HELP", QSTRING_TO_PSZ( help_file ), achFound, sizeof( achFound ));
                if ( rc != NO_ERROR )
                    // Still not found, so fall back to the language-neutral filename
                    help_file = help_library + ".hlp";
            }
        }
        else
            help_file = help_library + ".hlp";
    }

    memset( &hinit, 0, sizeof( HELPINIT ));
    hinit.cb = sizeof( HELPINIT );
    hinit.phtHelpTable = NULL;
    hinit.pszHelpWindowTitle = QSTRING_TO_PSZ( help_title );
    hinit.pszHelpLibraryName = QSTRING_TO_PSZ( QDir::toNativeSeparators( help_file ));
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
// PARAMETERS:
//     QWidget *window: The window to be queried
//
// RETURNS: unsigned short
//     The PM resource ID, or 0 on error
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


// ---------------------------------------------------------------------------
// Set a frame window's icon to the specified OS/2-native icon resource.
//
// PARAMETERS:
//     QWidget *window    : The window in question (should be a frame window
//                          class like QDialog)
//     void    *module    : The module (EXE or DLL) handle containing the
//                          resource, or NULL for the current executable
//     unsigned short usID: The PM resource ID of the icon resource
//
void OS2Native::setFrameIcon( QWidget *window, void *module, unsigned short usID )
{
    HWND     hwnd;
    HPOINTER hIcon;

    if ( !window ) return;
    hwnd = WinQueryWindow( window->winId(), QW_PARENT );
    if ( !hwnd ) return;

    hIcon = WinLoadPointer( HWND_DESKTOP, (HMODULE) module, usID );
    WinSendMsg( hwnd, WM_SETICON, (MPARAM) hIcon, (MPARAM) 0L );
}


// ---------------------------------------------------------------------------
// Delete an extended attribute of a file or directory.
//
// PARAMETERS:
//     PSZ  pszPathName: Name of the file or directory
//     PCSZ pszEAName  : Name of the extended attribute to delete
//
// RETURNS: APIRET
//     Return code from DosSetPathInfo
//
unsigned long OS2Native::deleteEA( char *pszPathName, const char *pszEAName )
{
    PFEA2LIST pFEA2List = NULL;
    PFEA2     pFEA2 = NULL;
    EAOP2     eaop2 = {0};
    size_t    cb = 0;
    APIRET    rc = ERROR_NOT_ENOUGH_MEMORY;

    cb = sizeof( FEA2LIST ) + strlen( pszEAName );
    pFEA2List = (PFEA2LIST) calloc( cb, 1 );
    if ( pFEA2List ) {
        pFEA2List->cbList = cb;
        pFEA2 = pFEA2List->list;
        pFEA2->cbName = (BYTE) strlen( pszEAName );
        strcpy( pFEA2->szName, pszEAName );
        pFEA2->cbValue = 0;

        eaop2.fpFEA2List = pFEA2List;
        rc = DosSetPathInfo( (PCSZ) pszPathName, FIL_QUERYEASIZE,
                             (PBYTE) &eaop2, sizeof( eaop2 ), DSPI_WRTTHRU );
        free( pFEA2List );
    }
    return (unsigned long) rc;
}


// ---------------------------------------------------------------------------
// Get a suitable application font according to the current locale. This is
// needed to handle languages which aren't supported by Workplace Sans.
//
// PARAMETERS:
//     QString locale: Name of the locale
//
// RETURNS: QString
//     Name of the application font
//
QString OS2Native::getFontForLocale( const QString &locale )
{
    QFontDatabase fontdb;
    QStringList   languageFonts,
                  matching;
    QString       fontName = "";
    QString       language;

    if ( locale.isEmpty() )
        language = QLocale::system().name().left( 2 );
    else
        language = locale.left( 2 );

    // Check for some commonly-installed pan-Unicode fonts
    languageFonts << "Times New Roman MT 30" << "Arial Unicode MS" << "Droid Sans Combined";

    // Check for specific system fonts for each language
    if ( language == "ja") {
        languageFonts << "MS PMincho" << "MS PGothic" << "Meiryo";
        languageFonts << "IPAMincho" << "IPAPMincho" << "IPAGothic" << "IPAPGothic";
    }
    else if ( language == "ko") {
        languageFonts << QString::fromUtf8("명조");          // Myeongjo
    }
    else if ( locale == "zh_CN") {
        languageFonts << QString::fromUtf8("宋体常规");    // Songti changgui
    }
    else if ( language == "zh") {
        languageFonts << QString::fromUtf8("標準宋體");    // Biaozhun songti
    }
    else {          // No action needed for other languages
        return fontName;
    }

    // Look for the best-matching installed font from our list (the last entry
    // has the highest priority).
    for ( int i = languageFonts.size() - 1; i >= 0; i-- ) {
        printf("%s\n", QSTRING_TO_PSZ( languageFonts.at( i )));
        matching = fontdb.families().filter( languageFonts.at(i) );
        if ( !matching.isEmpty() ) {
            fontName = matching.at( 0 );
            break;
        }
    }
    return fontName;
}


// ---------------------------------------------------------------------------
// Get the default system font size for OS/2 Presentation Manager. This is
// the font size that a default presentation space will use when no explicit
// font metrics are set.
//
// PARAMETERS: none
//
// RETURNS: int
//
//
int OS2Native::getSystemFontSize( void )
{
    HPS hps;
    FONTMETRICS fm;
    int iSize = 10;

    hps = WinGetScreenPS( HWND_DESKTOP );
    if ( hps ) {
        if ( GpiQueryFontMetrics( hps, sizeof(fm), &fm ) == NO_ERROR )
            iSize = (int)( fm.sNominalPointSize / 10 );
        WinReleasePS( hps );
    }
    return ( iSize );
}


// ---------------------------------------------------------------------------
// Create a desktop object.
//
// PARAMETERS:
//     const char *pczClass   : WPS class name
//     const char *pczTitle   : Object (icon) title
//     const char *pczSetup   : Object setup string
//     const char *pczLocation: Object location (parent folder ID)
//     bool        bReplace   : True to replace existing object, false to fail
//
// RETURNS: bool
//
//
bool OS2Native::createDesktopObject( const char *pcszClass, const char *pcszTitle,
                                     const char *pcszSetup,  const char *pcszLocation,
                                     bool bReplace )
{
    HOBJECT hObj;
    ULONG   ulFlags;

    ulFlags = bReplace? CO_REPLACEIFEXISTS: CO_FAILIFEXISTS;
    hObj = WinCreateObject( (PSZ) pcszClass, (PSZ) pcszTitle, (PSZ) pcszSetup, (PSZ) pcszLocation, ulFlags );
/*
    if ( !hObj ) {
        fprintf( stderr, "Unable to create object: PM error code = 0x%X\n",
                 ERRORIDERROR( WinGetLastError( 1 )));
    }
*/
    return ( hObj? true: false );
}


// ---------------------------------------------------------------------------
// Destroy a desktop object.
//
// PARAMETERS:
//     const char *pczObject: Object identifier
//
// RETURNS: bool
//
//
bool OS2Native::destroyDesktopObject( const char *pcszObject )
{
    HOBJECT hObj;
    bool    rc;

    hObj = WinQueryObject( (PSZ) pcszObject );
    if ( hObj )
        rc = (bool) WinDestroyObject( hObj );
    else
        rc = false;

    return rc;
}
