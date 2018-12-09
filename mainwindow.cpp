/******************************************************************************
** QE - mainwindow.cpp
**
**  Copyright (C) 2018 Alexander Taylor
**  Some parts based on sample code from Blanchette & Summerfield, "C++ GUI
**  Programming with Qt4" (Second Edition), Pearson 2007.
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

#include <QtGui>
#include <QPrinter>

#include "finddialog.h"
#include "replacedialog.h"
#include "gotolinedialog.h"
#include "mainwindow.h"
#include "qetextedit.h"
#include "threads.h"
#include "os2codec.h"
#ifdef __OS2__
#include "os2native.h"
#endif
#include "eastring.h"


// #ifdef __OS2__
// We now also use these mappings to look up the encoding from the codepage
// number when parsing the /cp: command line switch.

/* We tag a file with a non-default encoding under OS/2 by setting its .CODEPAGE
 * extended attribute. This (standardized but rarely-used) EA is meant to store
 * the OS/2 codepage number for the file's encoding.  What we do is use a lookup
 * table to map each character encoding to a codepage number.  In the rare event
 * that there isn't an OS/2 codepage number for the encoding, we use the official
 * IBM-registered codepage (CCSID) number from:
 *   https://www-01.ibm.com/software/globalization/ccsid/ccsid_registered.html
 * However, there are a couple of encodings that don't have even those.  In
 * those cases we simply assign an otherwise unused number ourselves (currently
 * all in the 1090-1096 range, since IBM seems not to have assigned those).
 *
 * If we somehow missed assigning a codepage number to an encoding, the fallback
 * logic is to simply write the encoding name as a string, verbatim; but this
 * should never actually happen in practice.
 *
 * In all honesty, the actual value shouldn't matter much, as long as it's
 * unique.  Using the actual OS/2 codepage number is for the sake of other
 * applications that might check this field; QE itself simply uses it as a
 * unique identifier (it could be anything).  If it finds an unrecognized value
 * in this EA, it treats it the same as if it's absent (i.e. use the locale
 * default encoding).
 *
 * Any other (well-written) OS/2 applications that actually check the .CODEPAGE
 * (if there are any - I don't know of one) should, similarly, simply ignore
 * values that they don't recognize or cannot parse.
 */

// The following two structs must be kept in sync!

unsigned int Codepage_CCSIDs[] = {
     437,  // "IBM-437"
     813,  // "ISO 8859-7"
     819,  // "Windows-1252"
     850,  // "IBM-850"
     858,  // "IBM-858"
     859,  // "IBM-859"
     862,  // "IBM-867"
     863,  // "IBM-863"
     864,  // "IBM-864"
     865,  // "IBM-865"
     866,  // "IBM-866"
     867,  // "IBM-867"
     869,  // "IBM-869"
     874,  // "IBM-874"
     878,  // "KOI8-R"
     912,  // "ISO 8859-2"
     913,  // "ISO 8859-3"
     914,  // "ISO 8859-4"
     915,  // "ISO 8859-5"
     916,  // "ISO 8859-8"
     919,  // "ISO 8859-10"
     921,  // "ISO 8859-13"
     923,  // "ISO 8859-15"
     932,  // "Shift-JIS"
     943,  // "Shift-JIS"
     950,  // "Big5-HKSCS"
     954,  // "EUC-JP"
     970,  // "EUC-KR"
    1089,  // "ISO 8859-6"
    1090,  // "ISO 8859-14"
    1091,  // "ISO 8859-16"
    1092,  // "TSCII"
    1168,  // "KOI8-U"
    1200,  // "UTF-16BE"
    1202,  // "UTF-16LE"
    1208,  // "UTF-8"
    1250,  // "Windows-1250"
    1251,  // "Windows-1251"
    1252,  // "Windows-1252"
    1253,  // "Windows-1253"
    1254,  // "Windows-1254"
    1255,  // "Windows-1255"
    1256,  // "Windows-1256"
    1257,  // "Windows-1257"
    1258,  // "Windows-1258"
    1275,  // "Apple Roman"
    1363,  // "cp949"
    1381,  // "GB2312"
    1386,  // "GB18030-0"
    4992   // "ISO-2022-JP"
};

QString Codepage_Mappings[] = {
    "IBM-437",
    "ISO 8859-7",
    "Windows-1252",
    "IBM-850",
    "IBM-858",
    "IBM-859",
    "IBM-867",
    "IBM-863",
    "IBM-864",
    "IBM-865",
    "IBM-866",
    "IBM-867",
    "IBM-869",
    "IBM-874",
    "KOI8-R",
    "ISO 8859-2",
    "ISO 8859-3",
    "ISO 8859-4",
    "ISO 8859-5",
    "ISO 8859-8",
    "ISO 8859-10",
    "ISO 8859-13",
    "ISO 8859-15",
    "Shift-JIS",
    "Shift-JIS",
    "Big5-HKSCS",
    "EUC-JP",
    "EUC-KR",
    "ISO 8859-6",
    "ISO 8859-14",
    "ISO 8859-16",
    "TSCII",
    "KOI8-U",
    "UTF-16BE",
    "UTF-16LE",
    "UTF-8",
    "Windows-1250",
    "Windows-1251",
    "Windows-1252",
    "Windows-1253",
    "Windows-1254",
    "Windows-1255",
    "Windows-1256",
    "Windows-1257",
    "Windows-1258",
    "Apple Roman",
    "cp949",
    "GB2312",
    "GB18030-0",
    "ISO-2022-JP"
};

// #endif


// ---------------------------------------------------------------------------
// PUBLIC CONSTRUCTOR
//

MainWindow::MainWindow()
{
    // Instantiate our new text codecs (must be created on the heap; Qt takes
    // over responsibility for these objects so we do nothing more with them).
    //
#ifndef DISABLE_NEW_CODECS
    QeOS2Codec *codec437  = new QeOS2Codec( QeOS2Codec::IBM437 );
    QeOS2Codec *codec852  = new QeOS2Codec( QeOS2Codec::IBM852 );
    QeOS2Codec *codec855  = new QeOS2Codec( QeOS2Codec::IBM855 );
    QeOS2Codec *codec857  = new QeOS2Codec( QeOS2Codec::IBM857 );
    QeOS2Codec *codec858  = new QeOS2Codec( QeOS2Codec::IBM858 );
    QeOS2Codec *codec859  = new QeOS2Codec( QeOS2Codec::IBM859 );
    QeOS2Codec *codec860  = new QeOS2Codec( QeOS2Codec::IBM860 );
    QeOS2Codec *codec861  = new QeOS2Codec( QeOS2Codec::IBM861 );
    QeOS2Codec *codec863  = new QeOS2Codec( QeOS2Codec::IBM863 );
    QeOS2Codec *codec864  = new QeOS2Codec( QeOS2Codec::IBM864 );
    QeOS2Codec *codec865  = new QeOS2Codec( QeOS2Codec::IBM865 );
    QeOS2Codec *codec867  = new QeOS2Codec( QeOS2Codec::IBM867 );
    QeOS2Codec *codec869  = new QeOS2Codec( QeOS2Codec::IBM869 );
    QeOS2Codec *codec922  = new QeOS2Codec( QeOS2Codec::IBM922 );
    QeOS2Codec *codec1125 = new QeOS2Codec( QeOS2Codec::IBM1125 );
    QeOS2Codec *codec1131 = new QeOS2Codec( QeOS2Codec::IBM1131 );

    // Keep the compiler happy
    if ( codec437 ) {;}
    if ( codec852 ) {;}
    if ( codec855 ) {;}
    if ( codec857 ) {;}
    if ( codec858 ) {;}
    if ( codec859 ) {;}
    if ( codec860 ) {;}
    if ( codec861 ) {;}
    if ( codec863 ) {;}
    if ( codec864 ) {;}
    if ( codec865 ) {;}
    if ( codec867 ) {;}
    if ( codec869 ) {;}
    if ( codec922 ) {;}
    if ( codec1125) {;}
    if ( codec1131) {;}
#endif

    setAttribute( Qt::WA_DeleteOnClose );

    editor = new QeTextEdit( this );

    editor->setBackgroundVisible( true );
    editor->document()->setUseDesignMetrics( false );
    QPalette p = editor->palette();
    p.setColor( QPalette::Background, QColor("#F0F0F0"));
    editor->setPalette(p);

    setCentralWidget( editor );

    createActions();
    createMenus();
    createContextMenu();
    createStatusBar();

    readSettings();

    openThread = 0;
    isReadThreadActive = false;
    findDialog = 0;
    replaceDialog = 0;
    lastGoTo = 1;

    setAcceptDrops( true );
    connect( editor, SIGNAL( cursorPositionChanged() ), this, SLOT( updatePositionLabel() ));
    connect( editor->document(), SIGNAL( contentsChanged() ), this, SLOT( updateModified() ));

    setMinimumWidth( statusBar()->minimumWidth() + 20 );
    setWindowTitle( tr("Text Editor") );

    // Qt4 on OS/2 doesn't render PNGs well, so leave it with a native icon; otherwise...
#ifndef __OS2__
    QIcon icon;
    icon.addFile(":/images/editor_16.png", QSize( 16, 16 ), QIcon::Normal, QIcon::On );
    icon.addFile(":/images/editor_20.png", QSize( 20, 20 ), QIcon::Normal, QIcon::On );
    icon.addFile(":/images/editor_32.png", QSize( 32, 32 ), QIcon::Normal, QIcon::On );
    icon.addFile(":/images/editor_40.png", QSize( 40, 40 ), QIcon::Normal, QIcon::On );
    icon.addFile(":/images/editor_64.png", QSize( 64, 64 ), QIcon::Normal, QIcon::On );
    icon.addFile(":/images/editor_80.png", QSize( 80, 80 ), QIcon::Normal, QIcon::On );
    setWindowIcon( icon );

#endif

    helpInstance = NULL;
    createHelp();

    currentEncoding = "";
    currentDir = QDir::currentPath();
    setCurrentFile("");
}



// ---------------------------------------------------------------------------
// DESTRUCTOR
//
MainWindow::~MainWindow()
{
#ifdef __OS2__
    if ( helpInstance ) OS2Native::destroyNativeHelp( helpInstance );
#endif
}



// ---------------------------------------------------------------------------
// EVENT FILTER
//
#if 0
bool MainWindow::eventFilter( QObject *target, QEvent *event )
{

    if ( target == editor ) {
         if ( event->type() == QEvent::MouseButtonRelease ) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if ( mouseEvent->button() == Qt::RightButton ) {
                editor->doContextMenuClick( mouseEvent->pos() );
                return true;
            }
        }
    }

    return QMainWindow::eventFilter( target, event );
}
#endif


// ---------------------------------------------------------------------------
// OVERRIDDEN EVENTS
//

void MainWindow::closeEvent( QCloseEvent *event )
{
    if ( okToContinue() ) {
        writeSettings();
        event->accept();
    }
    else {
        event->ignore();
    }
}


void MainWindow::dragEnterEvent( QDragEnterEvent *event )
{
    if (( event->mimeData()->hasFormat("text/uri-list")) ||
        ( event->mimeData()->hasFormat("text/plain")))
    {
        event->acceptProposedAction();
    }
}


void MainWindow::dropEvent (QDropEvent *event )
{
    QList<QUrl> urls = event->mimeData()->urls();
    if ( !urls.isEmpty() ) {
        QString fileName = urls.first().toLocalFile();
        if ( !fileName.isEmpty() ) {
            if ( okToContinue() )
                loadFile( fileName, false );
        }
    }
}


// ---------------------------------------------------------------------------
// SLOTS
//

void MainWindow::newFile()
{
    if ( okToContinue() ) {
        editor->clear();
        setCurrentFile("");
    }
}


void MainWindow::open()
{
    if ( okToContinue() ) {
/*
        QFileDialog dialog( this );
        dialog.setFileMode( QFileDialog::ExistingFile );
        dialog.setAcceptMode( QFileDialog::AcceptOpen );
        dialog.setNameFilter( tr( DEFAULT_FILENAME_FILTERS ));
        dialog.setOption( QFileDialog::DontUseNativeDialog, false );
        QFileInfoList drives = QDir::drives();
        QList<QUrl> urls;
        for ( int i = 0; i < drives.size(); i++ ) {
            urls.append( QUrl::fromLocalFile( drives.at( i ).canonicalFilePath()));
        }
        urls.removeOne( tr(""));
        urls.append( QUrl::fromLocalFile( QDir::homePath() ));
        dialog.setSidebarUrls( urls );
        if ( dialog.exec() ) {
            QStringList fileNameList = dialog.selectedFiles();
            if ( !fileNameList.isEmpty() )
                loadFile( fileNameList.at( 0 ), false );
        }
*/
#ifndef __OS2__
        QString fileName = QFileDialog::getOpenFileName( this,
                                                         tr("Open File"),
                                                         currentDir,
                                                         tr( DEFAULT_FILENAME_FILTERS ),
                                                         &currentFilter
                                                       );
#else
        QString fileName = OS2Native::getOpenFileName( this,
                                                       tr("Open File"),
                                                       currentDir,
                                                       tr( DEFAULT_FILENAME_FILTERS ),
                                                       &currentFilter
                                                     );
#endif
        if ( !fileName.isEmpty() ) {
            // always reset the encoding when explicitly opening a file
            currentEncoding = "";
            loadFile( fileName, false );
        }
//        else showMessage( tr("Cancelled."));
    }
}


bool MainWindow::save()
{
    if ( currentFile.isEmpty() )
        return saveAs();
    else
        return saveFile( currentFile );
}


bool MainWindow::saveAs()
{
#ifndef __OS2__
    QString fileName = QFileDialog::getSaveFileName( this,
                                                     tr("Save File"),
                                                     currentDir,
                                                     tr("All files (*)"));
#else
    QString fileName = OS2Native::getSaveFileName( this,
                                                   tr("Save File"),
                                                   currentDir,
                                                   tr("All files (*)"));
#endif
    if ( fileName.isEmpty() )
        return false;
    return saveFile( fileName );
}


void MainWindow::find()
{
    if ( !findDialog ) {
        findDialog = new FindDialog( this );
        connect( findDialog,
                 SIGNAL( findNext( const QString &, bool, bool, bool )),
                 this,
                 SLOT( findNext( const QString &, bool, bool, bool )));
        connect( findDialog,
                 SIGNAL( findNextRegExp( const QString &, bool, bool )),
                 this,
                 SLOT( findNextRegExp( const QString &, bool, bool )));
        connect( findDialog,
                 SIGNAL( findPrevious( const QString &, bool, bool, bool )),
                 this,
                 SLOT( findPrevious( const QString &, bool, bool, bool )));
        connect( findDialog,
                 SIGNAL( findPreviousRegExp( const QString &, bool, bool )),
                 this,
                 SLOT( findPreviousRegExp( const QString &, bool, bool )));
    }

    // Don't allow both find and replace dialogs to be visible at once
    if ( replaceDialog && !replaceDialog->isHidden() ) {
        replaceDialog->close();
    }

    if ( findDialog->isHidden() ) {
        findDialog->show();
    }
    else {
        findDialog->raise();
        findDialog->activateWindow();
    }
    QString selected = editor->textCursor().selectedText();
    if ( recentFinds.count() > 0 )
        findDialog->populateHistory( recentFinds );
    if ( ! selected.trimmed().isEmpty() )
        findDialog->setFindText( selected );
}


void MainWindow::findAgain()
{
    /* if ( findDialog ) findDialog->doFind(); */
    if ( lastFind.text.isNull() || lastFind.text.isEmpty() )
        return;

    // N.B. Start/end of file option makes no sense for Find Again operations, so always set false

    if ( lastFind.bBackward ) {
        if ( lastFind.bRe )
            findPreviousRegExp( lastFind.text, lastFind.bCase, false );
        else
            findPrevious( lastFind.text, lastFind.bCase, lastFind.bWords, false );
    } else {
        if ( lastFind.bRe )
            findNextRegExp( lastFind.text, lastFind.bCase, false );
        else
            findNext( lastFind.text, lastFind.bCase, lastFind.bWords, false );
    }
}


void MainWindow::replace()
{
    if ( !replaceDialog ) {
        replaceDialog = new ReplaceDialog( this );

        connect( replaceDialog,
                 SIGNAL( findNext( const QString &, bool, bool, bool )),
                 this,
                 SLOT( findNext( const QString &, bool, bool, bool )));
        connect( replaceDialog,
                 SIGNAL( findNextRegExp( const QString &, bool, bool )),
                 this,
                 SLOT( findNextRegExp( const QString &, bool, bool )));
        connect( replaceDialog,
                 SIGNAL( findPrevious( const QString &, bool, bool, bool )),
                 this,
                 SLOT( findPrevious( const QString &, bool, bool, bool )));
        connect( replaceDialog,
                 SIGNAL( findPreviousRegExp( const QString &, bool, bool )),
                 this,
                 SLOT( findPreviousRegExp( const QString &, bool, bool )));

        connect( replaceDialog,
                 SIGNAL( replaceNext( const QString &, const QString &, bool, bool, bool, bool )),
                 this,
                 SLOT( replaceNext( const QString &, const QString &, bool, bool, bool, bool )));
        connect( replaceDialog,
                 SIGNAL( replaceNextRegExp( const QString &, const QString &, bool, bool, bool )),
                 this,
                 SLOT( replaceNextRegExp( const QString &, const QString &, bool, bool, bool )));
        connect( replaceDialog,
                 SIGNAL( replacePrevious( const QString &, const QString &, bool, bool, bool, bool )),
                 this,
                 SLOT( replacePrevious( const QString &, const QString &, bool, bool, bool, bool )));
        connect( replaceDialog,
                 SIGNAL( replacePreviousRegExp( const QString &, const QString &, bool, bool, bool )),
                 this,
                 SLOT( replacePreviousRegExp( const QString &, const QString &, bool, bool, bool )));
        connect( replaceDialog,
                 SIGNAL( replaceAll( const QString &, const QString &, bool, bool, bool, bool, bool )),
                 this,
                 SLOT( replaceAll( const QString &, const QString &, bool, bool, bool, bool, bool )));
        connect( replaceDialog,
                 SIGNAL( replaceAllRegExp( const QString &, const QString &, bool, bool, bool, bool )),
                 this,
                 SLOT( replaceAllRegExp( const QString &, const QString &, bool, bool, bool, bool )));
    }

    // Don't allow both find and replace dialogs to be visible at once
    if ( findDialog && !findDialog->isHidden() ) {
        findDialog->close();
    }

    if ( replaceDialog->isHidden() ) {
        replaceDialog->show();
    }
    else {
        replaceDialog->raise();
        replaceDialog->activateWindow();
    }
    QString selected = editor->textCursor().selectedText();
    if ( recentFinds.count() > 0 )
        replaceDialog->populateHistory( recentFinds, recentReplaces );
    if ( ! selected.trimmed().isEmpty() )
        replaceDialog->setFindText( selected );
}


void MainWindow::goToLine()
{
    int min = 1;
    int max = editor->document()->blockCount();
    if (( lastGoTo < 1 ) || ( lastGoTo > max ))
        lastGoTo = 1;
    GoToLineDialog dialog( this, min, max, lastGoTo );
    if ( dialog.exec() ) {
        QString str = dialog.lineEdit->text();
        lastGoTo = str.toInt();
        QTextCursor cursor( editor->document()->findBlockByLineNumber( lastGoTo - 1 ));
        editor->setTextCursor( cursor );
    }
}


void MainWindow::about()
{
    QMessageBox::about( this,
                        tr("Product Information"),
                        tr("<b>QE Text Editor</b><br>Version %1<hr>"
                           "Copyright &copy;2018 Alexander Taylor"
                           "<p>Licensed under the GNU General Public License "
                           "version 3.0&nbsp;<br>"
                           "<a href=\"https://www.gnu.org/licenses/gpl.html\">"
                           "https://www.gnu.org/licenses/gpl.html</a>"
                           "<br></p>").arg( PROGRAM_VERSION )
                      );
}


void MainWindow::showGeneralHelp()
{
#ifdef __OS2__
    OS2Native::showHelpPanel( helpInstance, HELP_PANEL_GENERAL );
#else
    launchAssistant( HELP_HTML_GENERAL );
#endif
}


void MainWindow::showKeysHelp()
{
#ifdef __OS2__
    OS2Native::showHelpPanel( helpInstance, HELP_PANEL_KEYS );
#else
    launchAssistant( HELP_HTML_KEYS );
#endif
}


void MainWindow::openRecentFile()
{
    if ( okToContinue() ) {
        QAction *action = qobject_cast<QAction *>( sender() );
        if ( action ) {
            // always reset the encoding when explicitly opening a file
            currentEncoding = "";
            loadFile( action->data().toString(), false );
        }
    }
}


void MainWindow::clearRecentFiles()
{
    int r = QMessageBox::question( this,
                                   tr("Clear List?"),
                                   tr("Clear the list of recent files?"),
                                   QMessageBox::Yes | QMessageBox::No
                                 );
    if ( r == QMessageBox::Yes ) {
        recentFiles.clear();
        updateRecentFileActions();
    }
}


bool MainWindow::toggleEditMode( bool ovr )
{
    editor->setOverwriteMode( ovr );
    updateModeLabel();
    return ovr;
}


bool MainWindow::toggleWordWrap( bool bWrap )
{
    QTextOption::WrapMode mode;
    mode = bWrap ? QTextOption::WrapAtWordBoundaryOrAnywhere :
                   QTextOption::NoWrap;
    editor->setWordWrapMode( mode );
    return bWrap;
}


bool MainWindow::toggleReadOnly( bool readOnly )
{
    setReadOnly( readOnly );
    return readOnly;
}


void MainWindow::updateStatusBar()
{
    updateModeLabel();
    updatePositionLabel();
    updateEncodingLabel();
}


void MainWindow::updateEncodingLabel()
{
    if ( currentEncoding.isEmpty() )
        encodingLabel->setText( tr("Default"));
    else
        encodingLabel->setText( currentEncoding );
}


void MainWindow::updateModeLabel()
{
    if ( editor->isReadOnly() )
        editModeLabel->setText(" RO ");
    else if ( editor->overwriteMode() )
        editModeLabel->setText(" OVR ");
    else
        editModeLabel->setText(" INS ");
}


void MainWindow::updatePositionLabel()
{
    QTextCursor cursor;
    int row = 0,
        col = 0;

    cursor = editor->textCursor();
    col = cursor.positionInBlock();
    row = cursor.blockNumber() + 1;
    positionLabel->setText( QString("%1:%2").arg( row ).arg( col ));
}


void MainWindow::updateModified()
{
    bool isModified = editor->document()->isModified();
    setWindowModified( isModified );
    modifiedLabel->setText( isModified? tr("Modified"): "");
    if ( isModified ) messagesLabel->setText("");
}
void MainWindow::updateModified( bool isModified )
{
    editor->document()->setModified( isModified );
    setWindowModified( isModified );
    modifiedLabel->setText( isModified? tr("Modified"): "");
    if ( isModified ) messagesLabel->setText("");
}


void MainWindow::setEditorFont() {
    bool fontSelected;
    QFont font = QFontDialog::getFont( &fontSelected, editor->font(), this );
    if ( fontSelected ) {
        editor->setFont( font );
        editor->adjustSize();
    }
}


void MainWindow::findNext( const QString &str, bool cs, bool words, bool fromStart )
{
    lastFind.text      = str;
    lastFind.bCase     = cs;
    lastFind.bWords    = words;
    lastFind.bBackward = false;
    lastFind.bRe       = false;

    updateFindHistory( str );

    QTextDocument::FindFlags flags = QTextDocument::FindFlags( 0 );
    if ( cs )
        flags |= QTextDocument::FindCaseSensitively;
    if ( words )
        flags |= QTextDocument::FindWholeWords;
    int pos = fromStart ? 0 :
                          editor->textCursor().selectionEnd();

    showFindResult( editor->document()->find( str, pos, flags ), str );
}


void MainWindow::findNextRegExp( const QString &str, bool cs, bool fromStart )
{
    lastFind.text      = str;
    lastFind.bCase     = cs;
    lastFind.bWords    = false;
    lastFind.bBackward = false;
    lastFind.bRe       = true;

    updateFindHistory( str );

    QRegExp regexp( str );
    regexp.setCaseSensitivity( cs? Qt::CaseSensitive: Qt::CaseInsensitive );

    QTextDocument::FindFlags flags = QTextDocument::FindFlags( 0 );
    int pos = fromStart ? 0 :
                          editor->textCursor().selectionEnd();
    showFindResult( editor->document()->find( regexp, pos, flags ), str );
}


void MainWindow::findPrevious( const QString &str, bool cs, bool words, bool fromEnd )
{
    lastFind.text      = str;
    lastFind.bCase     = cs;
    lastFind.bWords    = words;
    lastFind.bBackward = true;
    lastFind.bRe       = false;

    updateFindHistory( str );

    QTextDocument::FindFlags flags = QTextDocument::FindBackward;
    if ( cs )
        flags |= QTextDocument::FindCaseSensitively;
    if ( words )
        flags |= QTextDocument::FindWholeWords;
    int pos = fromEnd ? editor->document()->characterCount() :
                        editor->textCursor().selectionStart();
    showFindResult( editor->document()->find( str, pos, flags ), str );
}


void MainWindow::findPreviousRegExp( const QString &str, bool cs, bool fromEnd )
{
    lastFind.text      = str;
    lastFind.bCase     = cs;
    lastFind.bWords    = false;
    lastFind.bBackward = true;
    lastFind.bRe       = true;

    updateFindHistory( str );

    QRegExp regexp( str );
    regexp.setCaseSensitivity( cs? Qt::CaseSensitive: Qt::CaseInsensitive );

    QTextDocument::FindFlags flags = QTextDocument::FindBackward;
    int pos = fromEnd ? editor->document()->characterCount() :
                        editor->textCursor().selectionStart();
    showFindResult( editor->document()->find( regexp, pos, flags ), str );
}


void MainWindow::replaceNext( const QString &str, const QString &repl, bool cs, bool words, bool fromStart, bool confirm )
{
    updateFindHistory( str );
    updateReplaceHistory( repl );
    QTextDocument::FindFlags flags = QTextDocument::FindFlags( 0 );
    if ( cs )
        flags |= QTextDocument::FindCaseSensitively;
    if ( words )
        flags |= QTextDocument::FindWholeWords;
    int pos = fromStart ? 0 :
                          editor->textCursor().selectionStart();
    QTextCursor found = editor->document()->find( str, pos, flags );
    if ( showFindResult( found, str )) {
        if ( ! replaceFindResult( editor->textCursor(), repl, confirm )) {
            // Clear selection but move the cursor position to its end
            pos = found.selectionEnd();
            found = editor->textCursor();
            found.clearSelection();
            found.setPosition( pos );
            editor->setTextCursor( found );
        }
    }
}


void MainWindow::replaceNextRegExp( const QString &str, const QString &repl, bool cs, bool fromStart, bool confirm )
{
    updateFindHistory( str );
    updateReplaceHistory( repl );

    QRegExp regexp( str );
    regexp.setCaseSensitivity( cs? Qt::CaseSensitive: Qt::CaseInsensitive );
    QString replaceStr = repl;
    replaceStr.replace("\\t", "\t");
    replaceStr.replace("\\a", "\a");
    replaceStr.replace("\\b", "\b");
    replaceStr.replace("\\f", "\f");
    replaceStr.replace("\\n", "\n");
    replaceStr.replace("\\r", "\r");
    replaceStr.replace("\\v", "\v");

    QTextDocument::FindFlags flags = QTextDocument::FindFlags( 0 );
    int pos = fromStart ? 0 :
                          editor->textCursor().selectionStart();
    QTextCursor found = editor->document()->find( regexp, pos, flags );
    if ( showFindResult( found, str )) {
        QString newText = found.selectedText();
        newText.replace( regexp, replaceStr );
        if ( !replaceFindResult( editor->textCursor(), newText, confirm )) {
            // Clear selection but move the cursor position to its end
            pos = found.selectionEnd();
            found = editor->textCursor();
            found.clearSelection();
            found.setPosition( pos );
            editor->setTextCursor( found );
        }
    }
}


void MainWindow::replacePrevious( const QString &str, const QString &repl, bool cs, bool words, bool fromEnd, bool confirm )
{
    updateFindHistory( str );
    updateReplaceHistory( repl );
    QTextDocument::FindFlags flags = QTextDocument::FindBackward;
    if ( cs )
        flags |= QTextDocument::FindCaseSensitively;
    if ( words )
        flags |= QTextDocument::FindWholeWords;
    int pos = fromEnd ? editor->document()->characterCount() :
                        editor->textCursor().selectionEnd();

    QTextCursor found = editor->document()->find( str, pos, flags );
    if ( showFindResult( found, str )) {
        if ( ! replaceFindResult( editor->textCursor(), repl, confirm )) {
            // Move the cursor to the selection start, then clear the selection
            pos = found.selectionStart();
            found = editor->textCursor();
            found.clearSelection();
            found.setPosition( pos );
            editor->setTextCursor( found );
        }
    }
}


void MainWindow::replacePreviousRegExp( const QString &str, const QString &repl, bool cs, bool fromEnd, bool confirm )
{
    updateFindHistory( str );
    updateReplaceHistory( repl );

    QRegExp regexp( str );
    regexp.setCaseSensitivity( cs? Qt::CaseSensitive: Qt::CaseInsensitive );
    QString replaceStr = repl;
    replaceStr.replace("\\t", "\t");
    replaceStr.replace("\\a", "\a");
    replaceStr.replace("\\b", "\b");
    replaceStr.replace("\\f", "\f");
    replaceStr.replace("\\n", "\n");
    replaceStr.replace("\\r", "\r");
    replaceStr.replace("\\v", "\v");

    QTextDocument::FindFlags flags = QTextDocument::FindBackward;
    int pos = fromEnd ? editor->document()->characterCount() :
                        editor->textCursor().selectionEnd();
    QTextCursor found = editor->document()->find( regexp, pos, flags );
    if ( showFindResult( found, str )) {
        QString newText = found.selectedText();
        newText.replace( regexp, replaceStr );
        if ( !replaceFindResult( editor->textCursor(), newText, confirm )) {
            // Move the cursor to the selection start, then clear the selection
            pos = found.selectionStart();
            found = editor->textCursor();
            found.clearSelection();
            found.setPosition( pos );
            editor->setTextCursor( found );
        }
    }
}


void MainWindow::replaceAll( const QString &str, const QString &repl, bool cs, bool words, bool fromStart, bool confirm, bool backwards )
{
    updateFindHistory( str );
    QTextDocument::FindFlags flags = QTextDocument::FindFlags( 0 );
    if ( cs )
        flags |= QTextDocument::FindCaseSensitively;
    if ( words )
        flags |= QTextDocument::FindWholeWords;
    if ( backwards )
        flags = QTextDocument::FindBackward;

    int pos = fromStart ? 0 :
                          ( backwards? editor->textCursor().selectionEnd():
                                       editor->textCursor().selectionStart() );
    QTextCursor found = editor->document()->find( str, pos, flags );

    if ( found.isNull() ) {
        showMessage( tr("No matches found for: %1").arg( str ));
        found = editor->textCursor();
        found.clearSelection();
        return;
    }

    replaceDialog->close();
    QMessageBox confirmBox( QMessageBox::Question,
                            tr("Replace"),
                            tr("Replace this text?"),
                            0L, this );
    confirmBox.addButton( tr("&Replace"), QMessageBox::YesRole );
    QPushButton *btnAll   = confirmBox.addButton( tr("Replace &All"), QMessageBox::YesRole );
    QPushButton *btnSkip  = confirmBox.addButton( tr("&Skip"),        QMessageBox::NoRole );
    QPushButton *btnClose = confirmBox.addButton( QMessageBox::Close );

    bool skip;
    int count = 0;
    while ( !found.isNull() ) {
        skip = false;

        QTextCursor temp( found );
        temp.setPosition( temp.selectionStart() );
        showMessage( tr("Found match at %1:%2").arg( temp.blockNumber() + 1 ).arg( temp.positionInBlock() ));
        editor->setTextCursor( found );

        if ( confirm ) {
            confirmBox.exec();
            QPushButton *r = (QPushButton *) confirmBox.clickedButton();
            if ( r == btnSkip )  skip = true;
            if ( r == btnClose ) break;
            if ( r == btnAll )   confirm = false;
        }
        if ( !skip ) {
            count++;
            found.insertText( repl );
        }
        found = editor->document()->find( str,
                                          (backwards? found.selectionStart(): found.selectionEnd()),
                                          flags );
    }
    showMessage( tr("%1 occurences replaced.").arg( count ));
    found = editor->textCursor();
    found.clearSelection();
    editor->setCenterOnScroll( true );
    editor->setTextCursor( found );
    editor->setCenterOnScroll( false );
}


void MainWindow::replaceAllRegExp( const QString &str, const QString &repl, bool cs, bool fromStart, bool confirm, bool backwards )
{
    updateFindHistory( str );
    updateReplaceHistory( repl );

    QRegExp regexp( str );
    regexp.setCaseSensitivity( cs? Qt::CaseSensitive: Qt::CaseInsensitive );
    QString replaceStr = repl;
    replaceStr.replace("\\t", "\t");
    replaceStr.replace("\\a", "\a");
    replaceStr.replace("\\b", "\b");
    replaceStr.replace("\\f", "\f");
    replaceStr.replace("\\n", "\n");
    replaceStr.replace("\\r", "\r");
    replaceStr.replace("\\v", "\v");

    QTextDocument::FindFlags flags = QTextDocument::FindFlags( 0 );
    if ( cs )
        flags |= QTextDocument::FindCaseSensitively;
    if ( backwards )
        flags = QTextDocument::FindBackward;
    int pos = fromStart ? 0 :
                          ( backwards? editor->textCursor().selectionEnd():
                                       editor->textCursor().selectionStart() );
    QTextCursor found = editor->document()->find( regexp, pos, flags );
    if ( found.isNull() ) {
        showMessage( tr("No matches found for: %1").arg( str ));
        found = editor->textCursor();
        found.clearSelection();
        return;
    }

    replaceDialog->close();
    QMessageBox confirmBox( QMessageBox::Question,
                            tr("Replace"),
                            tr("Replace this text?"),
                            0L, this );
    confirmBox.addButton( tr("&Replace"), QMessageBox::YesRole );
    QPushButton *btnAll   = confirmBox.addButton( tr("Replace &All"), QMessageBox::YesRole );
    QPushButton *btnSkip  = confirmBox.addButton( tr("&Skip"),        QMessageBox::NoRole );
    QPushButton *btnClose = confirmBox.addButton( QMessageBox::Close );

    bool skip;
    int count = 0;
    QString newText;
    while ( !found.isNull() ) {
        skip = false;

        QTextCursor temp( found );
        temp.setPosition( temp.selectionStart() );
        showMessage( tr("Found match at %1:%2").arg( temp.blockNumber() + 1 ).arg( temp.positionInBlock() ));
        editor->setTextCursor( found );

        if ( confirm ) {
            confirmBox.exec();
            QPushButton *r = (QPushButton *) confirmBox.clickedButton();
            if ( r == btnSkip )  skip = true;
            if ( r == btnClose ) break;
            if ( r == btnAll )   confirm = false;
        }
        if ( !skip ) {
            count++;
            newText = found.selectedText();
            newText.replace( regexp, replaceStr );
            found.insertText( newText );
        }
        found = editor->document()->find( regexp,
                                          (backwards? found.selectionStart(): found.selectionEnd()),
                                          flags );
    }
    showMessage( tr("%1 occurences replaced.").arg( count ));
    found = editor->textCursor();
    found.clearSelection();
    editor->setTextCursor( found );
}


void MainWindow::updateFindHistory( const QString &findString )
{
    if ( findString.isEmpty() ) return;
    if ( recentFinds.startsWith( findString )) return;
    recentFinds.removeAll( findString );
    recentFinds.prepend( findString );
    while ( recentFinds.size() > MaxRecentFinds )
        recentFinds.removeLast();
}


void MainWindow::updateReplaceHistory( const QString &replaceString )
{
    if ( replaceString.isEmpty() ) return;
    if ( recentReplaces.startsWith( replaceString )) return;
    recentReplaces.removeAll( replaceString );
    recentReplaces.prepend( replaceString );
    while ( recentReplaces.size() > MaxRecentFinds )
        recentReplaces.removeLast();
}


void MainWindow::setTextEncoding()
{
    QAction *action = qobject_cast<QAction *>( sender() );
    QString newEncoding = action->data().toString();
    if ( newEncoding.compare( currentEncoding ) != 0 ) {
        currentEncoding = newEncoding;

        // A currentEncoding of "" normally means use the default locale; however
        // when opening a file it will trigger an attempt to load the encoding from
        // the file attribute (OS/2 only), falling back to default otherwise.  To
        // force the file to be opened in the default encoding, set currentEncoding
        // to "Default" before calling loadFile (it will be reset to "" after the file
        // is opened).

        if ( isWindowModified() ) {
#if 0
            QMessageBox::warning( this,
                                  tr("Encoding Changed"),
                                  tr("You have changed the text encoding for this file. "
                                     "Because the file has been modified, it cannot be refreshed from disk "
                                     "using the new encoding.  The new encoding will be applied to this file "
                                     "the next time it is saved."),
                                  QMessageBox::Ok
                                );
#endif
            encodingChanged = true;
        }
        else if ( !currentFile.isEmpty() ) {
            // Ask whether to re-parse the file with the new encoding
            int r = QMessageBox::question( this,
                                           tr("Re-load File?"),
                                           tr("You have changed the text encoding for this file. "
                                              "Do you want to refresh the file from disk using the new encoding?"),
                                           QMessageBox::Yes | QMessageBox::No,
                                           QMessageBox::Yes
                                        );
            if ( r == QMessageBox::Yes ) {
                // Change empty to "Default" temporarily to get the right logic path in loadFile
                if ( currentEncoding == "") currentEncoding = "Default";
                loadFile( currentFile, false );
                return;     // the file-loading routine will update the GUI as needed
            }
            else
                encodingChanged = true;
        }
        updateEncodingLabel();
    }
    action->setChecked( true );
}


void MainWindow::setTextEncoding( QString newEncoding )
{
    if ( newEncoding.compare( currentEncoding ) != 0 ) {
        currentEncoding = newEncoding;

        if ( isWindowModified() ) {
            encodingChanged = true;
        }
        else if ( !currentFile.isEmpty() ) {
            // Ask whether to re-parse the file with the new encoding
            int r = QMessageBox::question( this,
                                           tr("Re-load File?"),
                                           tr("You have changed the text encoding for this file. "
                                              "Do you want to refresh the file from disk using the new encoding?"),
                                           QMessageBox::Yes | QMessageBox::No,
                                           QMessageBox::Yes
                                        );
            if ( r == QMessageBox::Yes ) {
                // Change empty to "Default" temporarily to get the right logic path in loadFile
                if ( currentEncoding == "") currentEncoding = "Default";
                loadFile( currentFile, false );
                return;     // the file-loading routine will update the GUI as needed
            }
            else
                encodingChanged = true;
        }
        updateEncoding();
    }
}


void MainWindow::openAsEncoding( QString fileName, bool createIfNew, QString encoding )
{
    // The user has told us to force a specific codepage to be used for reading this
    // file (overriding any that may be set in the EA).
    //
    if ( mapNameToEncoding( encoding )) {
        currentEncoding = encoding;
        // Changing empty to "Default" temporarily disables setting encoding from EA
        if ( currentEncoding == "") currentEncoding = "Default";
    }
    loadFile( fileName, createIfNew );
}


// ---------------------------------------------------------------------------
// OTHER METHODS
//

void MainWindow::createActions()
{
    // NOTE: Since we don't have an OS/2-native Qt theme, we have to use some
    // #ifdef's to specify a few OS/2-specific shortcuts.  Other platforms can
    // mostly use the standard aliases.

    // File menu actions

    newAction = new QAction( tr("&New"), this );
    newAction->setShortcut( QKeySequence::New );
    newAction->setStatusTip( tr("Create a new file") );
    connect( newAction, SIGNAL( triggered() ), this, SLOT( newFile() ));

    openAction = new QAction( tr("&Open..."), this );
    openAction->setShortcut( QKeySequence::Open );
    openAction->setStatusTip( tr("Open a file") );
    connect( openAction, SIGNAL( triggered() ), this, SLOT( open() ));

    saveAction = new QAction( tr("&Save"), this );
// It's a bad idea to have a hidden shortcut for save, especially one as easy to use by mistake as Ctrl+S
//    QList<QKeySequence> saveShortcuts;
//    saveShortcuts << QKeySequence("F2") << QKeySequence("Ctrl+S");
//    saveAction->setShortcuts( saveShortcuts );
#ifdef __OS2__
    saveAction->setShortcut( tr("F2"));
#else
    saveAction->setShortcut( QKeySequence::Save );
#endif
    saveAction->setStatusTip( tr("Save the current file") );
    connect( saveAction, SIGNAL( triggered() ), this, SLOT( save() ));

    saveAsAction = new QAction( tr("Save &as..."), this );
    saveAsAction->setShortcut( QKeySequence::SaveAs );
    saveAsAction->setStatusTip( tr("Save the current file under a new name") );
    connect( saveAsAction, SIGNAL( triggered() ), this, SLOT( saveAs() ));

    printAction = new QAction( tr("&Print..."), this );
    printAction->setShortcut( QKeySequence::Print );
    printAction->setStatusTip( tr("Print the current file") );
    connect( printAction, SIGNAL( triggered() ), this, SLOT( print() ));

    for ( int i = 0; i < MaxRecentFiles; i++ )
    {
        recentFileActions[ i ] = new QAction( this );
        recentFileActions[ i ]->setVisible( false );
        connect( recentFileActions[ i ], SIGNAL( triggered() ), this, SLOT( openRecentFile() ));
    }

    clearRecentAction = new QAction( tr("&Clear list"), this );
    clearRecentAction->setVisible( false );
    clearRecentAction->setStatusTip( tr("Clear the list of recent files") );
    connect( clearRecentAction, SIGNAL( triggered() ), this, SLOT( clearRecentFiles() ));

    exitAction = new QAction( tr("E&xit"), this );
    exitAction->setShortcut( tr("F3") );
    exitAction->setStatusTip( tr("Exit the program") );
    connect( exitAction, SIGNAL( triggered() ), this, SLOT( close() ));

    createEncodingActions();


    // Edit menu actions

    undoAction = new QAction( tr("&Undo"), this );
    undoAction->setShortcut( QKeySequence::Undo );
    undoAction->setStatusTip( tr("Undo the previous edit") );
    connect( undoAction, SIGNAL( triggered() ), editor, SLOT( undo() ));

    redoAction = new QAction( tr("Re&do"), this );
    redoAction->setShortcut( QKeySequence::Redo );
    redoAction->setStatusTip( tr("Revert the previous undo operation") );
    connect( redoAction, SIGNAL( triggered() ), editor, SLOT( redo() ));

    cutAction = new QAction( tr("Cu&t"), this );
#ifdef __OS2__
    QList<QKeySequence> cutShortcuts;
    cutShortcuts << QKeySequence("Shift+Del") << QKeySequence::Cut;
    cutAction->setShortcuts( cutShortcuts );
#else
    cutAction->setShortcut( QKeySequence::Cut );
#endif
    cutAction->setStatusTip( tr("Move the selected text to the clipboard") );
    connect( cutAction, SIGNAL( triggered() ), editor, SLOT( cut() ));

    copyAction = new QAction( tr("&Copy"), this );
#ifdef __OS2__
    QList<QKeySequence> copyShortcuts;
    copyShortcuts << QKeySequence("Ctrl+Ins") << QKeySequence::Copy;
    copyAction->setShortcuts( copyShortcuts );
#else
    copyAction->setShortcut( QKeySequence::Copy );
#endif
    copyAction->setStatusTip( tr("Copy the selected text to the clipboard") );
    connect( copyAction, SIGNAL( triggered() ), editor, SLOT( copy() ));

    pasteAction = new QAction( tr("&Paste"), this );
#ifdef __OS2__
    QList<QKeySequence> pasteShortcuts;
    pasteShortcuts << QKeySequence("Shift+Ins") << QKeySequence::Paste;
    pasteAction->setShortcuts( pasteShortcuts );
#else
    pasteAction->setShortcut( QKeySequence::Paste );
#endif
    pasteAction->setStatusTip( tr("Insert the current clipboard text") );
    connect( pasteAction, SIGNAL( triggered() ), editor, SLOT( paste() ));

    selectAllAction = new QAction( tr("Select &all"), this );
    QList<QKeySequence> selectAllShortcuts;
    selectAllShortcuts << QKeySequence::SelectAll << QKeySequence("Ctrl+/");
    selectAllAction->setShortcuts( selectAllShortcuts );
    selectAllAction->setStatusTip( tr("Select all text in the edit window") );
    connect( selectAllAction, SIGNAL( triggered() ), editor, SLOT( selectAll() ));

    findAction = new QAction( tr("&Find..."), this );
    findAction->setShortcut( QKeySequence::Find );
    findAction->setStatusTip( tr("Search for text") );
    connect( findAction, SIGNAL( triggered() ), this, SLOT( find() ));

    findAgainAction = new QAction( tr("Find a&gain"), this );
    findAgainAction->setShortcut( tr("Ctrl+G"));
    findAgainAction->setStatusTip( tr("Repeat the last search") );
    findAgainAction->setEnabled( false );
    connect( findAgainAction, SIGNAL( triggered() ), this, SLOT( findAgain() ));

    replaceAction = new QAction( tr("Find/&Replace..."), this );
    QList<QKeySequence> replaceShortcuts;
    replaceShortcuts << QKeySequence("Ctrl+R") << QKeySequence::Replace;
    replaceAction->setShortcuts( replaceShortcuts );
    replaceAction->setStatusTip( tr("Search and replace text") );
    connect( replaceAction, SIGNAL( triggered() ), this, SLOT( replace() ));

    goToAction = new QAction( tr("Go to &line..."), this );
    goToAction->setShortcut( tr("Ctrl+L"));
    goToAction->setStatusTip( tr("Go to the specified line of the file") );
    connect( goToAction, SIGNAL( triggered() ), this, SLOT( goToLine() ));

    // Options menu actions

    wrapAction = new QAction( tr("&Word wrap"), this );
    wrapAction->setCheckable( true );
    wrapAction->setShortcut( tr("Alt+W") );
    wrapAction->setStatusTip( tr("Toggle word wrap") );
    connect( wrapAction, SIGNAL( toggled( bool )), this, SLOT( toggleWordWrap( bool )));

    editModeAction = new QAction( tr("&Overwrite"), this );
    editModeAction->setCheckable( true );
    editModeAction->setShortcut( tr("Ins") );
    editModeAction->setStatusTip( tr("Toggle overwrite mode") );
    connect( editModeAction, SIGNAL( toggled( bool )), this, SLOT( toggleEditMode( bool )));

    readOnlyAction = new QAction( tr("&Read-only"), this );
    readOnlyAction->setCheckable( true );
    readOnlyAction->setShortcut( tr("Alt+R") );
    readOnlyAction->setStatusTip( tr("Toggle read-only mode") );
    connect( readOnlyAction, SIGNAL( toggled( bool )), this, SLOT( toggleReadOnly( bool )));

    fontAction = new QAction( tr("&Font..."), this );
    fontAction->setStatusTip( tr("Change the edit window font") );
    connect( fontAction, SIGNAL( triggered() ), this, SLOT( setEditorFont() ));

    helpGeneralAction = new QAction( tr("General &help"), this );
    helpGeneralAction->setStatusTip( tr("General program help") );
    helpGeneralAction->setShortcut( tr("F1") );
    connect( helpGeneralAction, SIGNAL( triggered() ), this, SLOT( showGeneralHelp() ));

    helpKeysAction = new QAction( tr("&Keys help"), this );
    helpKeysAction->setStatusTip( tr("Help on keyboard commands") );
    connect( helpKeysAction, SIGNAL( triggered() ), this, SLOT( showKeysHelp() ));

    aboutAction = new QAction( tr("&Product information"), this );
    aboutAction->setStatusTip( tr("Show product information") );
    connect( aboutAction, SIGNAL( triggered() ), this, SLOT( about() ));
}


void MainWindow::createEncodingActions()
{
    encodingGroup = new QActionGroup( this );

    localeAction = new QAction( tr("Use &default encoding"), this );
    encodingGroup->addAction( localeAction );
    localeAction->setCheckable( true );
    localeAction->setData("");
    localeAction->setStatusTip( tr("Use the system's current default encoding."));
    connect( localeAction, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    // Western Europe

    win1252Action = new QAction( tr("Latin-1 (Windows-1252)"), this );
    encodingGroup->addAction( win1252Action );
    win1252Action->setCheckable( true );
    win1252Action->setData("Windows-1252");
    win1252Action->setStatusTip( tr("Microsoft Latin-1 encoding for Western European languages (a superset of the official ISO-8859-1 standard)."));
    connect( win1252Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    iso885915Action = new QAction( tr("Latin-9 (ISO-8859-15)"), this );
    encodingGroup->addAction( iso885915Action );
    iso885915Action->setCheckable( true );
    iso885915Action->setData("ISO 8859-15");
    iso885915Action->setStatusTip( tr("ISO Latin-9 encoding for Western European languages. Supports the same languages as Windows-1252 but is not 100% compatible."));
    connect( iso885915Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    aromanAction = new QAction( tr("Mac OS Roman (Macintosh)"), this );
    encodingGroup->addAction( aromanAction );
    aromanAction->setCheckable( true );
    aromanAction->setData("Apple Roman");
    aromanAction->setStatusTip( tr("A text encoding used under Mac OS 9 and older; supports several Western languages and includes various graphical symbols."));
    connect( aromanAction, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    ibm850Action = new QAction( tr("Multilingual (IBM-850)"), this );
    encodingGroup->addAction( ibm850Action );
    ibm850Action->setCheckable( true );
    ibm850Action->setData("IBM-850");
    ibm850Action->setStatusTip( tr("Encoding for Western languages under DOS and older versions of OS/2. Generally superseded by IBM-850+euro."));
    connect( ibm850Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

#ifndef DISABLE_NEW_CODECS
    ibm437Action = new QAction( tr("United States (IBM-437)"), this );
    encodingGroup->addAction( ibm437Action );
    ibm437Action->setCheckable( true );
    ibm437Action->setData("IBM-437");
    ibm437Action->setStatusTip( tr("Classic text encoding for DOS and OS/2. Considered obsolete but sometimes still encountered."));
    connect( ibm437Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    ibm858Action = new QAction( tr("Multilingual (IBM-850+euro)"), this );
    encodingGroup->addAction( ibm858Action );
    ibm858Action->setCheckable( true );
    ibm858Action->setData("IBM-858");
    ibm858Action->setStatusTip( tr("Commonly used for Western languages under OS/2; this is an updated version of IBM-850 (official designation IBM-858)."));
    connect( ibm858Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    ibm859Action = new QAction( tr("Western European (IBM-859)"), this );
    encodingGroup->addAction( ibm859Action );
    ibm859Action->setCheckable( true );
    ibm859Action->setData("IBM-859");
    ibm859Action->setStatusTip( tr("IBM Latin-9 encoding for OS/2 systems. It is not compatible with ISO-8859-15, and is rarely used."));
    connect( ibm859Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    ibm860Action = new QAction( tr("Portuguese (IBM-860)"), this );
    encodingGroup->addAction( ibm860Action );
    ibm860Action->setCheckable( true );
    ibm860Action->setData("IBM-860");
    ibm860Action->setStatusTip( tr("Portuguese text encoding used under OS/2 and DOS."));
    connect( ibm860Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    ibm863Action = new QAction( tr("Canadian French (IBM-863)"), this );
    encodingGroup->addAction( ibm863Action );
    ibm863Action->setCheckable( true );
    ibm863Action->setData("IBM-863");
    ibm863Action->setStatusTip( tr("An encoding for Canadian French under OS/2 and DOS."));
    connect( ibm863Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));
#endif

    // Central Europe

    win1250Action = new QAction( tr("Central/East European (Windows-1250)"), this );
    encodingGroup->addAction( win1250Action );
    win1250Action->setCheckable( true );
    win1250Action->setData("Windows-1250");
    win1250Action->setStatusTip( tr("Microsoft encoding for Central and East European languages. It is not (quite) compatible with ISO-8859-2."));
    connect( win1250Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    iso88592Action = new QAction( tr("Latin-2 (ISO-8859-2)"), this );
    encodingGroup->addAction( iso88592Action );
    iso88592Action->setCheckable( true );
    iso88592Action->setData("ISO 8859-2");
    iso88592Action->setStatusTip( tr("ISO Latin-2 encoding for languages such as Polish, Hungarian, Czech, Slovakian, and the Balkan languages."));
    connect( iso88592Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

#ifndef DISABLE_NEW_CODECS
    ibm852Action = new QAction( tr("Central/East European (IBM-852)"), this );
    encodingGroup->addAction( ibm852Action );
    ibm852Action->setCheckable( true );
    ibm852Action->setData("IBM-852");
    ibm852Action->setStatusTip( tr("IBM Latin-2 encoding for Central and East European languages. Mainly used under DOS and OS/2."));
    connect( ibm852Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));
#endif

    // North Europe

    win1257Action = new QAction( tr("Baltic (Windows-1257)"), this );
    encodingGroup->addAction( win1257Action );
    win1257Action->setCheckable( true );
    win1257Action->setData("Windows-1257");
    win1257Action->setStatusTip( tr("Microsoft encoding for Baltic languages. It is not compatible with ISO Latin-4, -6 or -7."));
    connect( win1257Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    iso88594Action = new QAction( tr("Latin-4 (ISO-8859-4)"), this );
    encodingGroup->addAction( iso88594Action );
    iso88594Action->setCheckable( true );
    iso88594Action->setData("ISO 8859-4");
    iso88594Action->setStatusTip( tr("ISO Latin-4 encoding for languages such as Sami and Greenlandic; also sometimes used for Baltic languages."));
    connect( iso88594Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    iso885910Action = new QAction( tr("Latin-6 (ISO-8859-10)"), this );
    encodingGroup->addAction( iso885910Action );
    iso885910Action->setCheckable( true );
    iso885910Action->setData("ISO 8859-10");
    iso885910Action->setStatusTip( tr("ISO Latin-6 encoding for Nordic languages."));
    connect( iso885910Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    iso885913Action = new QAction( tr("Latin-7 (ISO-8859-13)"), this );
    encodingGroup->addAction( iso885913Action );
    iso885913Action->setCheckable( true );
    iso885913Action->setData("ISO 8859-13");
    iso885913Action->setStatusTip( tr("ISO Latin-7 encoding for Baltic languages, for which it contains slightly broader support than Latin-4."));

    iso885914Action = new QAction( tr("Latin-8 (ISO-8859-14)"), this );
    encodingGroup->addAction( iso885914Action );
    iso885914Action->setCheckable( true );
    iso885914Action->setData("ISO 8859-14");
    iso885914Action->setStatusTip( tr("ISO Latin-8 encoding for Celtic languages, such as Irish, Scottish Gaelic, and Welsh."));
    connect( iso885914Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

#ifndef DISABLE_NEW_CODECS
    ibm861Action = new QAction( tr("Icelandic (IBM-861)"), this );
    encodingGroup->addAction( ibm861Action );
    ibm861Action->setCheckable( true );
    ibm861Action->setData("IBM-861");
    ibm861Action->setStatusTip( tr("Icelandic text encoding used under OS/2 and DOS."));
    connect( ibm861Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    ibm865Action = new QAction( tr("Nordic (IBM-865)"), this );
    encodingGroup->addAction( ibm865Action );
    ibm865Action->setCheckable( true );
    ibm865Action->setData("IBM-865");
    ibm865Action->setStatusTip( tr("IBM encoding for Nordic languages, used on OS/2 and DOS systems."));
    connect( ibm865Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    ibm922Action = new QAction( tr("Estonian (IBM-922)"), this );
    encodingGroup->addAction( ibm922Action );
    ibm922Action->setCheckable( true );
    ibm922Action->setData("IBM-922");
    ibm922Action->setStatusTip( tr("Estonian text encoding used under OS/2 and DOS."));
    connect( ibm922Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));
#endif

    // South Europe

    iso88597Action = new QAction( tr("Greek (ISO-8859-7)"), this );
    encodingGroup->addAction( iso88597Action );
    iso88597Action->setCheckable( true );
    iso88597Action->setData("ISO 8859-7");
    iso88597Action->setStatusTip( tr("ISO encoding for modern Greek."));
    connect( iso88597Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    win1253Action = new QAction( tr("Greek (Windows-1253)"), this );
    encodingGroup->addAction( win1253Action );
    win1253Action->setCheckable( true );
    win1253Action->setData("Windows-1253");
    win1253Action->setStatusTip( tr("Microsoft encoding for modern Greek; not quite compatible with ISO-8859-7."));
    connect( win1253Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    iso88593Action = new QAction( tr("Latin-3 (ISO-8859-3)"), this );
    encodingGroup->addAction( iso88593Action );
    iso88593Action->setCheckable( true );
    iso88593Action->setData("ISO 8859-3");
    iso88593Action->setStatusTip( tr("ISO Latin-3 encoding for Maltese and Esperanto; sometimes also used for Turkish."));
    connect( iso88593Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    win1254Action = new QAction( tr("Latin-5 (Windows-1254)"), this );
    encodingGroup->addAction( win1254Action );
    win1254Action->setCheckable( true );
    win1254Action->setData("Windows-1254");
    win1254Action->setStatusTip( tr("Microsoft Latin-5 encoding for Turkish; it is a superset of the ISO-8859-9 standard."));
    connect( win1254Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    iso885916Action = new QAction( tr("Latin-10 (ISO-8859-16)"), this );
    encodingGroup->addAction( iso885916Action );
    iso885916Action->setCheckable( true );
    iso885916Action->setData("ISO 8859-16");
    iso885916Action->setStatusTip( tr("ISO Latin-10 encoding for South-Eastern European languages."));
    connect( iso885916Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

#ifndef DISABLE_NEW_CODECS
    ibm857Action = new QAction( tr("Turkish (IBM-857)"), this );
    encodingGroup->addAction( ibm857Action );
    ibm857Action->setCheckable( true );
    ibm857Action->setData("IBM-857");
    ibm857Action->setStatusTip( tr("Turkish (Latin-5) text encoding used under OS/2 and DOS. Not compatible with ISO-8859-9."));
    connect( ibm857Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    ibm869Action = new QAction( tr("Greek (IBM-869)"), this );
    encodingGroup->addAction( ibm869Action );
    ibm869Action->setCheckable( true );
    ibm869Action->setData("IBM-869");
    ibm869Action->setStatusTip( tr("IBM encoding for Greek, used under OS/2 and DOS."));
    connect( ibm869Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));
#endif

    // Cyrillic

    iso88595Action = new QAction( tr("Cyrillic (ISO-8859-5)"), this );
    encodingGroup->addAction( iso88595Action );
    iso88595Action->setCheckable( true );
    iso88595Action->setData("ISO 8859-5");
    iso88595Action->setStatusTip( tr("ISO Cyrillic encoding. Supports Russian, Belorusian, Bulgarian, Macedonian, Moldovan and Serbian, but not Ukrainian."));
    connect( iso88595Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    win1251Action = new QAction( tr("Cyrillic (Windows-1251)"), this );
    encodingGroup->addAction( win1251Action );
    win1251Action->setCheckable( true );
    win1251Action->setData("Windows-1251");
    win1251Action->setStatusTip( tr("Microsoft encoding for Cyrillic languages. It is not compatible with ISO-8859-5, but includes Ukrainian support."));
    connect( win1251Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    ibm866Action = new QAction( tr("Russian (IBM-866)"), this );
    encodingGroup->addAction( ibm866Action );
    ibm866Action->setCheckable( true );
    ibm866Action->setData("IBM-866");
    ibm866Action->setStatusTip( tr("Commonly used for Russian text under OS/2 and DOS."));
    connect( ibm866Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    koi8rAction = new QAction( tr("Russian (KOI8-R)"), this );
    encodingGroup->addAction( koi8rAction );
    koi8rAction->setCheckable( true );
    koi8rAction->setData("KOI8-R");
    koi8rAction->setStatusTip( tr("An encoding for Russian text which is commonly used on the Internet."));
    connect( koi8rAction, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    koi8uAction = new QAction( tr("Ukrainian (KOI8-U)"), this );
    encodingGroup->addAction( koi8uAction );
    koi8uAction->setCheckable( true );
    koi8uAction->setData("KOI8-U");
    koi8uAction->setStatusTip( tr("An encoding for Ukrainian text which is commonly used on the Internet."));
    connect( koi8uAction, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

#ifndef DISABLE_NEW_CODECS
    ibm855Action = new QAction( tr("Cyrillic (IBM-855)"), this );
    encodingGroup->addAction( ibm855Action );
    ibm855Action->setCheckable( true );
    ibm855Action->setData("IBM-855");
    ibm855Action->setStatusTip( tr("IBM encoding for Bulgarian and the Balkan languages. Mainly used under DOS and OS/2."));
    connect( ibm855Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    ibm1125Action = new QAction( tr("Ukrainian (IBM-1125)"), this );
    encodingGroup->addAction( ibm1125Action );
    ibm1125Action->setCheckable( true );
    ibm1125Action->setData("IBM-1125");
    ibm1125Action->setStatusTip( tr("Ukrainian text encoding used under OS/2 and DOS."));
    connect( ibm1125Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    ibm1131Action = new QAction( tr("Belarusian (IBM-1131)"), this );
    encodingGroup->addAction( ibm1131Action );
    ibm1131Action->setCheckable( true );
    ibm1131Action->setData("IBM-1131");
    ibm1131Action->setStatusTip( tr("Belarusian text encoding used under OS/2 and DOS."));
    connect( ibm1131Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));
#endif

    // East Asia

    big5Action = new QAction( tr("Chinese (Big-5)"), this );
    encodingGroup->addAction( big5Action );
    big5Action->setCheckable( true );
    big5Action->setData("Big5-HKSCS");
    big5Action->setStatusTip( tr("Big-5 (with HKSCS supplement) is a Chinese text encoding used in Taiwan and Hong Kong."));
    connect( big5Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    gbkAction = new QAction( tr("Chinese (GB18030)"), this );
    encodingGroup->addAction( gbkAction );
    gbkAction->setCheckable( true );
    gbkAction->setData("GB18030-0");
    gbkAction->setStatusTip( tr("This is used for Chinese text encoding in mainland China; it is a superset of a previous standard called GBK."));
    connect( gbkAction, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    gbAction = new QAction( tr("Chinese (GB2312)"), this );
    encodingGroup->addAction( gbAction );
    gbAction->setCheckable( true );
    gbAction->setData("GB2312");
    gbAction->setStatusTip( tr("This is an older standard for Chinese text in mainland China. Largely superseded by GBK and GB18030."));
    connect( gbAction, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    eucJpAction = new QAction( tr("Japanese (EUC-JP)"), this );
    encodingGroup->addAction( eucJpAction );
    eucJpAction->setCheckable( true );
    eucJpAction->setData("EUC-JP");
    eucJpAction->setStatusTip( tr("An encoding for Japanese text which is often used under Unix."));
    connect( eucJpAction, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    iso2022JpAction = new QAction( tr("Japanese (ISO-2022-JP)"), this );
    encodingGroup->addAction( iso2022JpAction );
    iso2022JpAction->setCheckable( true );
    iso2022JpAction->setData("ISO 2022-JP");
    iso2022JpAction->setStatusTip( tr("An older encoding sometimes still used for Japanese text, especially in e-mail."));
    connect( iso2022JpAction, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    sjisAction = new QAction( tr("Japanese (Shift-JIS)"), this );
    encodingGroup->addAction( sjisAction );
    sjisAction->setCheckable( true );
    sjisAction->setData("Shift-JIS");
    sjisAction->setStatusTip( tr("A common encoding for Japanese text under Windows and OS/2; also known as codepage 932."));
    connect( sjisAction, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    eucKrAction = new QAction( tr("Korean (EUC-KR)"), this );
    encodingGroup->addAction( eucKrAction );
    eucKrAction->setCheckable( true );
    eucKrAction->setData("EUC-KR");
    eucKrAction->setStatusTip( tr("An encoding for Korean text which is often used under Unix."));
    connect( eucKrAction, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    uhcAction = new QAction( tr("Korean (Windows-949)"), this );
    encodingGroup->addAction( uhcAction );
    uhcAction->setCheckable( true );
    uhcAction->setData("cp949");
    uhcAction->setStatusTip( tr("Windows Unified Hangul Code for Korean."));
    connect( uhcAction, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    // South Asia

    tsciiAction = new QAction( tr("Tamil (TSCII)"), this );
    encodingGroup->addAction( tsciiAction  );
    tsciiAction->setCheckable( true );
    tsciiAction->setData("TSCII");
    tsciiAction->setStatusTip( tr("An encoding for Tamil text."));
    connect( tsciiAction , SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    ibm874Action = new QAction( tr("Thai (IBM-874)"), this );
    encodingGroup->addAction( ibm874Action );
    ibm874Action->setCheckable( true );
    ibm874Action->setData("IBM-874");
    ibm874Action->setStatusTip( tr("A superset of the TIS-620 standard, commonly used for Thai text under Windows and OS/2."));
    connect( ibm874Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    win1258Action = new QAction( tr("Vietnamese (Windows-1258)"), this );
    encodingGroup->addAction( win1258Action );
    win1258Action->setCheckable( true );
    win1258Action->setData("Windows-1258");
    win1258Action->setStatusTip( tr("Microsoft encoding for Vietnamese text."));
    connect( win1258Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    // Middle East

    iso88596Action = new QAction( tr("Arabic (ISO-8859-6)"), this );
    encodingGroup->addAction( iso88596Action );
    iso88596Action->setCheckable( true );
    iso88596Action->setData("ISO 8859-6");
    iso88596Action->setStatusTip( tr("ISO encoding for Arabic text."));
    connect( iso88596Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    win1256Action = new QAction( tr("Arabic (Windows-1256)"), this );
    encodingGroup->addAction( win1256Action );
    win1256Action->setCheckable( true );
    win1256Action->setData("Windows-1256");
    win1256Action->setStatusTip( tr("Microsoft encoding for Arabic text; it is not compatible with ISO-8859-6."));
    connect( win1256Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    iso88598Action = new QAction( tr("Hebrew (ISO-8859-8)"), this );
    encodingGroup->addAction( iso88598Action );
    iso88598Action->setCheckable( true );
    iso88598Action->setData("ISO 8859-8");
    iso88598Action->setStatusTip( tr("ISO encoding for Hebrew text."));
    connect( iso88598Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    win1255Action = new QAction( tr("Hebrew (Windows-1255)"), this );
    encodingGroup->addAction( win1255Action );
    win1255Action->setCheckable( true );
    win1255Action->setData("Windows-1255");
    win1255Action->setStatusTip( tr("Microsoft encoding for Hebrew and Yiddish text. Mainly a superset of ISO-8859-8, but not 100% compatible."));
    connect( win1255Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

#ifndef DISABLE_NEW_CODECS
    ibm862Action = new QAction( tr("Hebrew (IBM-862+euro)"), this );
    encodingGroup->addAction( ibm862Action );
    ibm862Action->setCheckable( true );
    ibm862Action->setData("IBM-867");
    ibm862Action->setStatusTip( tr("Hebrew text encoding used under OS/2 and DOS; this is an updated version of IBM-862 (official designation IBM-867)"));
    connect( ibm862Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    ibm864Action = new QAction( tr("Arabic (IBM-864)"), this );
    encodingGroup->addAction( ibm864Action );
    ibm864Action->setCheckable( true );
    ibm864Action->setData("IBM-864");
    ibm864Action->setStatusTip( tr("Arabic text encoding used under OS/2 and DOS."));
    connect( ibm864Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));
#endif

    // Unicode

    utf16Action = new QAction( tr("Unicode UTF-16 &LE"), this );
    encodingGroup->addAction( utf16Action );
    utf16Action->setCheckable( true );
    utf16Action->setData("UTF-16LE");
    utf16Action->setStatusTip( tr("UTF-16 (little endian) is a multi-byte Unicode encoding. It is rarely used for text files, and is not directly compatible with basic ASCII."));
    connect( utf16Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    utf16beAction = new QAction( tr("Unicode UTF-16 &BE"), this );
    encodingGroup->addAction( utf16beAction );
    utf16beAction->setCheckable( true );
    utf16beAction->setData("UTF-16BE");
    utf16beAction->setStatusTip( tr("UTF-16 (big endian) is a multi-byte Unicode encoding. It is rarely used for text files, and is not directly compatible with basic ASCII."));
    connect( utf16beAction, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));

    utf8Action = new QAction( tr("&Unicode UTF-8"), this );
    encodingGroup->addAction( utf8Action );
    utf8Action->setCheckable( true );
    utf8Action->setData("UTF-8");
    utf8Action->setStatusTip( tr("UTF-8 is the recommended format for Unicode text files."));
    connect( utf8Action, SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));


/*
 = new QAction( tr(""), this );
    encodingGroup->addAction(  );
    connect( , SIGNAL( triggered() ), this, SLOT( setTextEncoding() ));
->setCheckable( true );
->setData("");
->setStatusTip( tr(""));
*/

}


void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu( tr("&File"));
    fileMenu->addAction( newAction );
    fileMenu->addAction( openAction );
    fileMenu->addAction( saveAction );
    fileMenu->addAction( saveAsAction );
    fileMenu->addSeparator();
    fileMenu->addAction( printAction );
    fileMenu->addSeparator();
    encodingMenu = fileMenu->addMenu( tr("&Encoding"));
    separatorAction = fileMenu->addSeparator();
    for ( int i = 0; i < MaxRecentFiles; i++ )
        fileMenu->addAction( recentFileActions[ i ] );
    fileMenu->addAction( clearRecentAction );
    fileMenu->addSeparator();
    fileMenu->addAction( exitAction );

    // Encoding menu actions
    encodingMenu->addAction( localeAction );
    encodingMenu->addSeparator();

    centEuroMenu = encodingMenu->addMenu( tr("&Central/East European"));
#ifndef DISABLE_NEW_CODECS
    centEuroMenu->addAction( ibm852Action );
#endif
    centEuroMenu->addAction( win1250Action );
    centEuroMenu->addAction( iso88592Action );

    cyrillicMenu = encodingMenu->addMenu( tr("C&yrillic"));
#ifndef DISABLE_NEW_CODECS
    cyrillicMenu->addAction( ibm1131Action );
    cyrillicMenu->addAction( ibm855Action );
#endif
    cyrillicMenu->addAction( iso88595Action );
    cyrillicMenu->addAction( win1251Action );
    cyrillicMenu->addAction( ibm866Action );
    cyrillicMenu->addAction( koi8rAction );
#ifndef DISABLE_NEW_CODECS
    cyrillicMenu->addAction( ibm1125Action );
#endif
    cyrillicMenu->addAction( koi8uAction );

    eastAsiaMenu = encodingMenu->addMenu( tr("&East Asian"));
    eastAsiaMenu->addAction( big5Action );
    eastAsiaMenu->addAction( gbkAction );
    if ( QTextCodec::codecForName("GB2312") != 0 )
        eastAsiaMenu->addAction( gbAction );
    eastAsiaMenu->addAction( eucJpAction );
    eastAsiaMenu->addAction( iso2022JpAction );
    eastAsiaMenu->addAction( sjisAction );
    eastAsiaMenu->addAction( eucKrAction );
    if ( QTextCodec::codecForName("cp949") != 0 )
        eastAsiaMenu->addAction( uhcAction );

    midEastMenu = encodingMenu->addMenu( tr("&Middle Eastern"));
#ifndef DISABLE_NEW_CODECS
    midEastMenu->addAction( ibm864Action );
#endif
    midEastMenu->addAction( iso88596Action );
    midEastMenu->addAction( win1256Action );
#ifndef DISABLE_NEW_CODECS
    midEastMenu->addAction( ibm862Action );
#endif
    midEastMenu->addAction( iso88598Action );
    midEastMenu->addAction( win1255Action );

    northEuroMenu = encodingMenu->addMenu( tr("&North European"));
    northEuroMenu->addAction( win1257Action );
#ifndef DISABLE_NEW_CODECS
    northEuroMenu->addAction( ibm922Action );
    northEuroMenu->addAction( ibm861Action );
#endif
    northEuroMenu->addAction( iso88594Action );
    northEuroMenu->addAction( iso885910Action );
    northEuroMenu->addAction( iso885913Action );
    northEuroMenu->addAction( iso885914Action );
#ifndef DISABLE_NEW_CODECS
    northEuroMenu->addAction( ibm865Action );
#endif

    southAsiaMenu = encodingMenu->addMenu( tr("South &Asian"));
    southAsiaMenu->addAction( tsciiAction );
    southAsiaMenu->addAction( ibm874Action );
    southAsiaMenu->addAction( win1258Action );

    southEuroMenu = encodingMenu->addMenu( tr("S&outh European"));
#ifndef DISABLE_NEW_CODECS
    southEuroMenu->addAction( ibm869Action );
#endif
    southEuroMenu->addAction( iso88597Action );
    southEuroMenu->addAction( win1253Action );
    southEuroMenu->addAction( iso88593Action );
    southEuroMenu->addAction( win1254Action );
    southEuroMenu->addAction( iso885916Action );
#ifndef DISABLE_NEW_CODECS
    southEuroMenu->addAction( ibm857Action );
#endif

    westEuroMenu = encodingMenu->addMenu( tr("&Western"));
#ifndef DISABLE_NEW_CODECS
    westEuroMenu->addAction( ibm863Action );
#endif
    westEuroMenu->addAction( win1252Action );
    westEuroMenu->addAction( iso885915Action );
    westEuroMenu->addAction( aromanAction );
    westEuroMenu->addAction( ibm850Action );
#ifndef DISABLE_NEW_CODECS
    westEuroMenu->addAction( ibm858Action );
    westEuroMenu->addAction( ibm860Action );
    westEuroMenu->addAction( ibm437Action );
    westEuroMenu->addAction( ibm859Action );
#endif

    encodingMenu->addSeparator();
    unicodeMenu = encodingMenu->addMenu( tr("&Unicode"));
    unicodeMenu->addAction( utf16Action );
    unicodeMenu->addAction( utf16beAction );
    unicodeMenu->addAction( utf8Action );

    editMenu = menuBar()->addMenu( tr("&Edit"));
    editMenu->addAction( undoAction );
    editMenu->addAction( redoAction );
    editMenu->addSeparator();
    editMenu->addAction( cutAction );
    editMenu->addAction( copyAction );
    editMenu->addAction( pasteAction );
    editMenu->addSeparator();
    editMenu->addAction( selectAllAction );
    editMenu->addSeparator();
    editMenu->addAction( goToAction );
    editMenu->addSeparator();
    editMenu->addAction( findAction );
    editMenu->addAction( findAgainAction );
    editMenu->addAction( replaceAction );

    optionsMenu = menuBar()->addMenu( tr("&Options"));
    optionsMenu->addAction( wrapAction );
    optionsMenu->addAction( editModeAction );
    optionsMenu->addAction( readOnlyAction );
    optionsMenu->addSeparator();
    optionsMenu->addAction( fontAction );

    menuBar()->addSeparator();
    helpMenu = menuBar()->addMenu( tr("&Help"));
    helpMenu->addAction( helpGeneralAction );
    helpMenu->addAction( helpKeysAction );
    helpMenu->addSeparator();
    helpMenu->addAction( aboutAction );

}


void MainWindow::createContextMenu()
{
}


void MainWindow::createStatusBar()
{
    editModeLabel = new QLabel(" OVR ", this );
    editModeLabel->setAlignment( Qt::AlignHCenter );
    editModeLabel->setMinimumSize( editModeLabel->sizeHint() );

    positionLabel = new QLabel("000000:000000", this );
    positionLabel->setAlignment( Qt::AlignHCenter );
    positionLabel->setMinimumSize( positionLabel->sizeHint() );

    encodingLabel = new QLabel( tr(" Unknown encoding "), this );
    encodingLabel->setAlignment( Qt::AlignHCenter );
    encodingLabel->setMinimumSize( encodingLabel->sizeHint() );

    messagesLabel = new QLabel("                                       ", this );
    messagesLabel->setIndent( 3 );
    messagesLabel->setMinimumSize( messagesLabel->sizeHint() );

    modifiedLabel = new QLabel(" Modified ", this );
    modifiedLabel->setAlignment( Qt::AlignHCenter );
    modifiedLabel->setMinimumSize( modifiedLabel->sizeHint() );

    statusBar()->addWidget( messagesLabel, 1 );
    statusBar()->addWidget( encodingLabel );
    statusBar()->addWidget( editModeLabel );
    statusBar()->addWidget( positionLabel );
    statusBar()->addWidget( modifiedLabel );
    statusBar()->setMinimumSize( statusBar()->sizeHint() );

    messagesLabel->setForegroundRole( QPalette::ButtonText );
    encodingLabel->setForegroundRole( QPalette::ButtonText );
    editModeLabel->setForegroundRole( QPalette::ButtonText );
    positionLabel->setForegroundRole( QPalette::ButtonText );
    modifiedLabel->setForegroundRole( QPalette::ButtonText );

    updateStatusBar();
}


void MainWindow::createHelp()
{
#ifdef __OS2__
    helpInstance = OS2Native::setNativeHelp( this, QString("qe.hlp"), tr("QE Help") );
#else
    helpProcess = new QProcess( this );
#endif
}


void MainWindow::readSettings()
{
    QSettings settings( SETTINGS_VENDOR, SETTINGS_APP );

    restoreGeometry( settings.value("geometry").toByteArray() );

    recentFiles = settings.value("recentFiles").toStringList();
    updateRecentFileActions();

    currentFilter = settings.value("lastFilter", tr("All files (*)")).toString();
    recentFinds = settings.value("recentFinds").toStringList();
    recentReplaces = settings.value("recentReplaces").toStringList();

    toggleEditMode( settings.value("overwrite", false ).toBool() );
    editModeAction->setChecked( editor->overwriteMode() );

    bool wrap = settings.value("wrapMode", true ).toBool();
    toggleWordWrap( wrap );
    wrapAction->setChecked( wrap );

    QString defaultFont;
    QFontDatabase fontdb;
    QStringList matching = fontdb.families().filter("Droid Sans Mono");
    if ( !matching.isEmpty() )
        defaultFont = matching.at( 0 );
    else
        defaultFont = "Courier";
    QFont font("");
    font.fromString( settings.value("editorFont", defaultFont ).toString() );
    editor->setFont( font );

    readOnlyAction->setChecked( editor->isReadOnly() );
}


void MainWindow::writeSettings()
{
    QSettings settings( SETTINGS_VENDOR, SETTINGS_APP );

    settings.setValue("geometry",       saveGeometry() );
    settings.setValue("lastFilter",     currentFilter );
    settings.setValue("recentFinds",    recentFinds );
    settings.setValue("recentReplaces", recentReplaces );
    settings.setValue("recentFiles",    recentFiles );
    settings.setValue("overwrite",      editor->overwriteMode() );
    settings.setValue("wrapMode",
                      (editor->wordWrapMode() == QTextOption::WrapAtWordBoundaryOrAnywhere )?
                      true: false
                     );
    settings.setValue("editorFont",     editor->font().toString() );
}


bool MainWindow::okToContinue()
{
    if ( isWindowModified() ) {
        // This approach allows us to set a shortcut on the Discard button
        QMessageBox confirm( QMessageBox::Warning,
                             tr("Text Editor"),
                             tr("There are unsaved changes.<p>Do you want to save the changes?"),
                             QMessageBox::Save, this );
        confirm.addButton( tr("&Discard"), QMessageBox::DestructiveRole );
        confirm.addButton( QMessageBox::Cancel );
        int r = confirm.exec();
        if ( r == QMessageBox::Save )
            return save();
        else if ( r == QMessageBox::Cancel )
            return false;
    }
    return true;
}


bool MainWindow::loadFile( const QString &fileName, bool createIfNew )
{
    QFile *file = new QFile( fileName );

    if ( !file->open( QIODevice::ReadOnly | QFile::Text )) {
        if ( createIfNew ) {
            editor->clear();
            showMessage( tr("New file: %1").arg( QDir::toNativeSeparators( fileName )));
        }
        else {
            QMessageBox::critical( this, tr("Error"), tr("The file could not be opened."));
            if ( currentEncoding == "Default") currentEncoding = "";
            return false;
        }
    }
    else {
        QTextCodec *codec;
        // currentEncoding is always reset to "" when doing an explicit open
        if ( currentEncoding.isEmpty() ) {
            currentEncoding = getFileCodepage( fileName );
            if ( !currentEncoding.isEmpty() )
                codec = QTextCodec::codecForName( currentEncoding.toLatin1().data() );
            else
                codec = QTextCodec::codecForLocale();
        }
        else {
            // This will only be used if we're doing a reload of the current file
            if ( currentEncoding == "Default") {
                codec = QTextCodec::codecForLocale();
                currentEncoding = "";
            }
            else
                codec = QTextCodec::codecForName( currentEncoding.toLatin1().data() );
        }
        QApplication::setOverrideCursor( Qt::WaitCursor );

#ifdef USE_IO_THREADS

        showMessage( tr("Opening %1").arg( QDir::toNativeSeparators( fileName )));
        menuBar()->setEnabled( false );
//        editor->document()->clear();
        editor->setEnabled( false );
        if ( !openThread )
            openThread = new QeOpenThread();
        connect( openThread, SIGNAL( updateProgress( int )), this, SLOT( readProgress( int )));
        connect( openThread, SIGNAL( finished() ), this, SLOT( readDone() ));
        openThread->setFile( file, codec, fileName );
        openThread->start();
        isReadThreadActive = true;
        return true;
#else

        QTextStream in( file );
        in.setCodec( codec );
        QString text = in.readAll();
        editor->setPlainText( text );
        file->close();
        delete file;
        QApplication::restoreOverrideCursor();
        showMessage( tr("Opened file: %1").arg( QDir::toNativeSeparators( fileName )));
#endif

    }
    setCurrentFile( fileName );
    return true;
}


bool MainWindow::saveFile( const QString &fileName )
{
    if ( encodingChanged && ( !currentEncoding.startsWith("UTF-"))) {
        int r = QMessageBox::warning( this,
                                      tr("Encoding Changed"),
                                      tr("The text encoding for this file has been changed to %1."
                                         "<p>If the file contains characters which are not supported "
                                         "by this encoding, they may not be preserved when the file "
                                         "is saved.<p>Save the file now?").arg(
                                            currentEncoding.isEmpty() ? tr("Default") : currentEncoding
                                          ),
                                      QMessageBox::Yes | QMessageBox::No,
                                      QMessageBox::Yes
                                    );
        if ( r == QMessageBox::No )
            return false;
    }

    QFile file( fileName );
    bool bExists = ( file.exists() );

    if ( bExists ) {
        QDateTime fileTime = QFileInfo( fileName ).lastModified();
        if ( fileTime > currentModifyTime ) {
            int r = QMessageBox::warning( this,
                                          tr("File Modified"),
                                          tr("The modification time on %1 has changed."
                                             "<p>This file may have been modified by another "
                                             "application or process. If you save now, any "
                                             "such modifications will be lost.</p>"
                                             "<p>Save anyway?</p>").arg( QDir::toNativeSeparators( fileName )),
                                          QMessageBox::Yes | QMessageBox::No,
                                          QMessageBox::Yes
                                        );
            if ( r == QMessageBox::No )
                return false;
        }
    }

    // Opening in read/write mode seems to preserve EAs on existing files.
    if ( !file.open( QIODevice::ReadWrite | QFile::Text )) {
        QMessageBox::critical( this, tr("Error"), tr("Error writing file"));
        return false;
    }
#ifdef USE_IO_THREADS
    QTextStream out( &file );
    out.setCodec( QTextCodec::codecForName( currentEncoding.toLatin1().data() ));
    QString text = editor->toPlainText();
    out << text;
    out.flush();
    qint64 iSize = out.pos();
    if ( iSize != -1 ) file.resize( iSize );
    file.flush();
    file.close();

    showMessage( tr("Saved file: %1 (%2 bytes written)").arg( QDir::toNativeSeparators( fileName )).arg( iSize ));
    setCurrentFile( fileName );

    if ( !bExists ) {
#ifdef __OS2__
        // If this is a new file, get rid of the useless default EAs added by klibc
        EASetString( (PSZ) fileName.toLocal8Bit().data(), (PSZ) "UID",   (PSZ) "");
        EASetString( (PSZ) fileName.toLocal8Bit().data(), (PSZ) "GID",   (PSZ) "");
        EASetString( (PSZ) fileName.toLocal8Bit().data(), (PSZ) "MODE",  (PSZ) "");
        EASetString( (PSZ) fileName.toLocal8Bit().data(), (PSZ) "INO",   (PSZ) "");
        EASetString( (PSZ) fileName.toLocal8Bit().data(), (PSZ) "RDEV",  (PSZ) "");
        EASetString( (PSZ) fileName.toLocal8Bit().data(), (PSZ) "GEN",   (PSZ) "");
        EASetString( (PSZ) fileName.toLocal8Bit().data(), (PSZ) "FLAGS", (PSZ) "");
#endif
    }

    if ( !currentEncoding.isEmpty() ) {
        setFileCodepage( fileName, currentEncoding );
    }
#else

    QApplication::setOverrideCursor( Qt::WaitCursor );
    menuBar()->setEnabled( false );
    editor->setEnabled( false );

    showMessage( tr("Saving %1").arg( QDir::toNativeSeparators( fileName )));
    if ( !saveThread )
        saveThread = new QeSaveThread();
    connect( saveThread, SIGNAL( updateProgress( int )), this, SLOT( saveProgress( int )));
    connect( saveThread, SIGNAL( saveComplete( qint64 )), this, SLOT( saveDone( qint64 )));
    QTextCodec codec( QTextCodec::codecForName( currentEncoding.toLatin1().data() ));
    saveThread->setFile( file, codec, fileName );
    saveThread->fullText = editor->toPlainText();
    saveThread->start();
    isSaveThreadActive = true;

#endif      // USE_IO_THREADS

    return true;
}


bool MainWindow::print()
{
    QPrinter printer( QPrinter::HighResolution );
    QPrintDialog printDialog( &printer, this );
    if ( printDialog.exec() ) {
        editor->print( &printer );
        return true;
    }
    else
        return false;
}


void MainWindow::setCurrentFile( const QString &fileName )
{
    currentFile = fileName;
    updateModified( false );
    encodingChanged = false;
    QString shownName = tr("Untitled");
    currentModifyTime = QDateTime::currentDateTime();
    if ( !currentFile.isEmpty() ) {
        currentModifyTime = QFileInfo( fileName ).lastModified();
        currentDir = QDir::cleanPath( QFileInfo( fileName ).absolutePath() );
        shownName  = strippedName( currentFile );
        recentFiles.removeAll( currentFile );
        recentFiles.prepend( currentFile );
        updateRecentFileActions();
    }
    updateEncoding();
    setWindowTitle( tr("Text Editor - %1 [*]").arg( shownName ));
}


void MainWindow::updateRecentFileActions()
{
    QMutableStringListIterator i( recentFiles );
    int fileCount = 0;
    while ( i.hasNext() ) {
        fileCount++;
        if ( !QFile::exists( i.next() ) || ( fileCount > MaxRecentFiles )) {
            i.remove();
        }
    }
    for ( int j = 0; j < MaxRecentFiles; j++ ) {
        if ( j < recentFiles.count() ) {
            QString text = tr("&%1 %2").arg( j+1 ).arg( strippedName( recentFiles[ j ] ));
            recentFileActions[ j ]->setText( text );
            recentFileActions[ j ]->setData( recentFiles[ j ] );
            recentFileActions[ j ]->setStatusTip( QDir::toNativeSeparators( recentFiles[ j ] ));
            recentFileActions[ j ]->setVisible( true );
        }
        else {
            recentFileActions[ j ]->setVisible( false );
        }
    }
    separatorAction->setVisible( !recentFiles.isEmpty() );
    clearRecentAction->setVisible( !recentFiles.isEmpty() );
}


QString MainWindow::strippedName( const QString &fullFileName )
{
    return QFileInfo( fullFileName ).fileName();
}


void MainWindow::showMessage( const QString &message )
{
    messagesLabel->setText( message );
}


void MainWindow::showUsage()
{
#if defined( Q_OS_WIN32 )
#define SWITCH_CHAR     '/'
#elif defined( Q_OS_OS2 )
#define SWITCH_CHAR     '/'
#else
#define SWITCH_CHAR     '-'
#endif

    QMessageBox::information( this, tr("Usage"),
                              tr("<b>Usage:</b><br> &nbsp; <tt>qe [ <i>filename</i> ] [ <i>options</i> ]</tt>"
                                 "<p><b>Options:</b>"
                                 "<table>"
                                  "<tr><td> &nbsp; %1read</td> <td style=\"padding-left: 1em;\">Read-only mode</td></tr>"
                                  "<tr><td> &nbsp; %1enc:&lt;encoding&gt;</td> <td style=\"padding-left: 1em;\">Use the specified encoding</td></tr>"
                                  "<tr><td> &nbsp; %1?   </td> <td style=\"padding-left: 1em;\">Show usage information</td></tr>"
                                  "</table>").arg( SWITCH_CHAR ),
                              QMessageBox::Ok
                            );
}


void MainWindow::setReadOnly( bool readOnly )
{
    editor->setTextInteractionFlags( readOnly ?
                                        Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard :
                                        Qt::TextEditorInteraction
                                   );
    updateModeLabel();
}


void MainWindow::updateEncoding()
{
    QList<QAction *> actions = encodingGroup->actions();
    for ( int i = 0; i < actions.size(); i++ ) {
        if ( QString::compare( actions.at( i )->data().toString(), currentEncoding ) == 0 ) {
            actions.at( i )->setChecked( true );
            break;
        }
    }
    updateEncodingLabel();
}


bool MainWindow::showFindResult( QTextCursor found, const QString &str )
{
    bool isFound = false;
    findAgainAction->setEnabled( true );
    if ( found.isNull() ) {
        showMessage( tr("No matches found for: %1").arg( str ));
        found = editor->textCursor();
        found.clearSelection();
    }
    else {
        QTextCursor temp( found );
        temp.setPosition( temp.selectionStart() );
        showMessage( tr("Found match at %1:%2").arg( temp.blockNumber() + 1 ).arg( temp.positionInBlock() ));
        isFound = true;
    }
    editor->setCenterOnScroll( true );
    editor->setTextCursor( found );
    editor->setCenterOnScroll( false );
    return isFound;
}


bool MainWindow::replaceFindResult( QTextCursor found, const QString newText, bool confirm )
{
    if ( confirm ) {
        replaceDialog->close();
        int r = QMessageBox::question( this,
                                       tr("Confirm"),
                                       tr("Replace this text?"),
                                       QMessageBox::Yes | QMessageBox::No,
                                       QMessageBox::Yes
                                    );
        replaceDialog->show();
        if ( r != QMessageBox::Yes ) {
            return false;
        }
    }
    QTextCursor temp( found );
    temp.setPosition( temp.selectionStart() );
    int column = temp.positionInBlock();
    found.insertText( newText );
    showMessage( tr("Replaced text at %1:%2").arg( found.blockNumber() + 1 ).arg( column ));

    return true;
}


bool MainWindow::mapNameToEncoding( QString &encoding )
{
    bool bOK = false;

    // First we try to interpret it as a numeric codepage number
    unsigned int iCP = encoding.toUInt( &bOK );
    if ( bOK ) {
        int iMax = sizeof( Codepage_CCSIDs ) / sizeof ( int );
        for ( int i = 0; i < iMax; i++ ) {
            if ( iCP == Codepage_CCSIDs[ i ] ) {
                encoding = Codepage_Mappings[ i ];
                bOK = true;
                break;
            }
        }
    }
    // If that didn't work, see if it matches one of our encoding names verbatim
    else {
        QList<QAction *> actions = encodingGroup->actions();
        for ( int i = 0; i < actions.size(); i++ ) {
            if ( QString::compare( actions.at( i )->data().toString(), encoding, Qt::CaseInsensitive ) == 0 ) {
                bOK = true;
                // Sync the passed string with the matching one so that it has the proper case
                encoding = actions.at( i )->data().toString();
                break;
            }
        }
    }
    if ( !bOK )
        encoding = "";

    return ( bOK );
}


QString MainWindow::getFileCodepage( const QString &fileName )
{
    QString encoding("");

#ifdef __OS2__
    char szBuf[ 32 ] = {'\0'};

    // See if the file has a .CODEPAGE extended attribute
    EAQueryString( (PSZ) fileName.toLocal8Bit().data(), (PSZ) ENCODING_EA_NAME,
                   sizeof( szBuf ), (PSZ) szBuf );
    encoding = QString::fromLatin1( szBuf );

    if ( !encoding.isEmpty() ) {
        // We found a value, now try and map it to something meaningful
        // (if this call fails, encoding will be changed to "")
        mapNameToEncoding( encoding );
    }
#else
    // Keep the compiler happy
    if ( fileName.isEmpty() ) {;}
#endif

    return encoding;
}


void MainWindow::setFileCodepage( const QString &fileName, const QString &encodingName )
{
#ifdef __OS2__
    QString encoding("");

    // Look up the codepage number (official or otherwise) for this encoding.
    int iMax = sizeof( Codepage_CCSIDs ) / sizeof ( int );
    int iCP = 0;
    for ( int i = 0; i < iMax; i++ ) {
        if ( QString::compare( encodingName, Codepage_Mappings[ i ] ) == 0 ) {
            iCP = Codepage_CCSIDs[ i ];
            encoding.setNum( iCP );
            break;
        }
    }

    // The encoding isn't in our lookup table - in this case just use the name verbatim.
    if ( encoding.isEmpty() ) {
        encoding = encodingName;
    }

    if ( !encoding.isEmpty() ) {
        APIRET rc = EASetString( (PSZ) fileName.toLocal8Bit().data(),
                                 (PSZ) ENCODING_EA_NAME,
                                 (PSZ) encoding.toLatin1().data() );
        if ( rc ) {
//            showMessage( tr("Failed to update EA: %1").arg( rc ));
        }
    }
#else
    // Keep the compiler happy
    if ( fileName.isEmpty() ) {;}
    if ( encodingName.isEmpty() ) {;}
#endif
}


void MainWindow::readProgress( int percent )
{
#ifdef USE_IO_THREADS

    if ( !isReadThreadActive ) return;
    showMessage( tr("Opening %1 (%2%)").arg( QDir::toNativeSeparators( openThread->inputFileName )).arg( percent ));

    // TODO update progress bar, once implemented
#endif
}


void MainWindow::readDone()
{
#ifdef USE_IO_THREADS

    isReadThreadActive = false;
    if ( !openThread ) return;

    showMessage( tr("Opening %1 (100%)").arg( QDir::toNativeSeparators( openThread->inputFileName )));
    editor->setUpdatesEnabled( false );
    editor->setPlainText( openThread->getText() );
    editor->setUpdatesEnabled( true );

//  TODO hide progress bar, once implemented

    menuBar()->setEnabled( true );
    editor->setEnabled( true );

    QApplication::restoreOverrideCursor();
    showMessage( tr("Opened file: %1").arg( QDir::toNativeSeparators( openThread->inputFileName )));
    setCurrentFile( openThread->inputFileName );

    editor->setFocus( Qt::OtherFocusReason );

#endif
}


void MainWindow::readCancel()
{
#ifdef USE_IO_THREADS
    if ( openThread )
        openThread->cancel();

    menuBar()->setEnabled( true );
    editor->setEnabled( true );
    QApplication::restoreOverrideCursor();
    editor->setFocus( Qt::OtherFocusReason );

    showMessage( tr("Cancelled."));

#endif
}


void MainWindow::saveDone( qint64 iSize )
{
#ifdef USE_IO_THREADS

    isSaveThreadActive = false;
    if ( !saveThread ) return;

//    cancel progress indicator, if any

    if ( !currentEncoding.isEmpty() ) {
        setFileCodepage( saveThread->outputFileName, currentEncoding );
    }
    showMessage( tr("Saved file: %1 (%2 bytes written)").arg( QDir::toNativeSeparators( saveThread->outputFileName )).arg( iSize ));
    setCurrentFile( saveThread->outputFileName );

    menuBar()->setEnabled( true );
    editor->setEnabled( true );
    QApplication::restoreOverrideCursor();

    editor->setFocus( Qt::OtherFocusReason );

#endif
}


void MainWindow::saveProgress( int percent )
{
#ifdef USE_IO_THREADS

    if ( !isSaveThreadActive ) return;
    showMessage( tr("Saving %1 (%2%)").arg( QDir::toNativeSeparators( saveThread->outputFileName )).arg( percent ));

#endif
}


void MainWindow::launchAssistant( const QString &panel )
{
    QString assistant = QLibraryInfo::location( QLibraryInfo::BinariesPath )
                        + QLatin1String("/assistant");
    QStringList args;
    args << QLatin1String("-collectionFile")
         << QLatin1String("qe.qhc")
         << QLatin1String("-enableRemoteControl");
    helpProcess->start( assistant, args );
    if ( !helpProcess->waitForStarted() ) {
        helpProcess->start( QLatin1String("assistant"), args );
        if ( !helpProcess->waitForStarted() ) {
            showMessage( tr("Help viewer not available."));
            return;
        }
    }
    QByteArray assistantInput;
    assistantInput.append("setSource " + QString( HELP_HTML_ROOT ) + panel + QString("\n"));
    helpProcess->write( assistantInput );
}

