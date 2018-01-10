#include <QtGui>
#include <QPrinter>

#include "finddialog.h"
#include "replacedialog.h"
#include "gotolinedialog.h"
#include "mainwindow.h"


// ---------------------------------------------------------------------------
// PUBLIC CONSTRUCTOR
//

MainWindow::MainWindow()
{
    editor = new QPlainTextEdit;

    editor->setBackgroundVisible( true );
    QPalette p = editor->palette();
    p.setColor( QPalette::Background, QColor("#F0F0F0"));
    editor->setPalette(p);

    connect( newAction, SIGNAL( triggered() ), this, SLOT( newFile() ));
    setCentralWidget( editor );

    createActions();
    createMenus();
    createContextMenu();
    createStatusBar();

    readSettings();

    findDialog = 0;
    replaceDialog = 0;
    lastGoTo = 1;

    connect( editor, SIGNAL( cursorPositionChanged() ), this, SLOT( updatePositionLabel() ));
    connect( editor->document(), SIGNAL( contentsChanged() ), this, SLOT( updateModified() ));

    setMinimumWidth( statusBar()->minimumWidth() + 20 );
    setWindowTitle( tr("Text Editor") );
    //setWindowIcon( QIcon(":/images/editor.png"));
    /*
    QIcon icon;
    icon.addFile(":/images/editor.png", QSize( 40, 40 ), QIcon::Normal, QIcon::On );
    icon.addFile(":/images/editor_mini.png", QSize( 20, 20 ), QIcon::Normal, QIcon::On );
    icon.addFile(":/images/editor_vga.png", QSize( 32, 32 ), QIcon::Normal, QIcon::On );
    icon.addFile(":/images/editor_vga_mini.png", QSize( 16, 16 ), QIcon::Normal, QIcon::On );
    setWindowIcon( icon );
    */

    setCurrentFile("");

}


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
#if 0
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
#else
        QString fileName = QFileDialog::getOpenFileName( this,
                                                         tr("Open File"),
                                                         currentDir,
                                                         tr( DEFAULT_FILENAME_FILTERS )
                                                       );
        if ( !fileName.isEmpty() )
            loadFile( fileName, false );
#endif
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
    QString fileName = QFileDialog::getSaveFileName( this,
                                                     tr("Save File"),
                                                     currentDir,
                                                     tr("All files (*)"));
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
}


void MainWindow::findAgain()
{
    if ( findDialog )
        findDialog->doFind();
}


void MainWindow::replace()
{
    if ( !replaceDialog ) {
        replaceDialog = new ReplaceDialog( this );
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
                        tr("<b>Quick Text Editor</b><br>Version %1<hr>"
                           "Copyright &copy;2018 Alexander Taylor"
                           "<p>Licensed under the GNU General Public License "
                           "version 3.0&nbsp;<br>"
                           "<a href=\"https://www.gnu.org/licenses/gpl.html\">"
                           "https://www.gnu.org/licenses/gpl.html</a>"
                           "<br></p>").arg( PROGRAM_VERSION )
                      );
}


void MainWindow::openRecentFile()
{
    if ( okToContinue() ) {
        QAction *action = qobject_cast<QAction *>( sender() );
        if ( action )
            loadFile( action->data().toString(), false );
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
    if ( fontSelected )
        editor->setFont( font );
}


void MainWindow::findNext( const QString &str, bool cs, bool words, bool fromStart )
{
    QTextDocument::FindFlags flags = QTextDocument::FindFlags( 0 );
    if ( cs )
        flags |= QTextDocument::FindCaseSensitively;
    if ( words )
        flags |= QTextDocument::FindWholeWords;
    int pos = fromStart ? 0 :
                          editor->textCursor().selectionEnd();
    showFindResult( editor->document()->find( str, pos, flags ));
}


void MainWindow::findNextRegExp( const QString &str, bool cs, bool fromStart )
{
    QRegExp regexp( str );
    regexp.setCaseSensitivity( cs? Qt::CaseSensitive: Qt::CaseInsensitive );

    QTextDocument::FindFlags flags = QTextDocument::FindFlags( 0 );
    int pos = fromStart ? 0 :
                          editor->textCursor().selectionEnd();
    showFindResult( editor->document()->find( regexp, pos, flags ));
}


void MainWindow::findPrevious( const QString &str, bool cs, bool words, bool fromEnd )
{
    QTextDocument::FindFlags flags = QTextDocument::FindBackward;
    if ( cs )
        flags |= QTextDocument::FindCaseSensitively;
    if ( words )
        flags |= QTextDocument::FindWholeWords;
    int pos = fromEnd ? editor->document()->characterCount() :
                        editor->textCursor().selectionStart();
    showFindResult( editor->document()->find( str, pos, flags ));
}


void MainWindow::findPreviousRegExp( const QString &str, bool cs, bool fromEnd )
{
    QRegExp regexp( str );
    regexp.setCaseSensitivity( cs? Qt::CaseSensitive: Qt::CaseInsensitive );

    QTextDocument::FindFlags flags = QTextDocument::FindBackward;
    int pos = fromEnd ? editor->document()->characterCount() :
                        editor->textCursor().selectionStart();
    showFindResult( editor->document()->find( regexp, pos, flags ));
}


void MainWindow::replaceNext( const QString &str, const QString &repl, bool cs, bool words, bool fromStart, bool confirm )
{
    QTextDocument::FindFlags flags = QTextDocument::FindFlags( 0 );
    if ( cs )
        flags |= QTextDocument::FindCaseSensitively;
    if ( words )
        flags |= QTextDocument::FindWholeWords;
    int pos = fromStart ? 0 :
                          editor->textCursor().selectionEnd();
    QTextCursor found = editor->document()->find( str, pos, flags );
    if ( showFindResult( found )) {
        if ( ! replaceFindResult( editor->textCursor(), repl, confirm )) {
            // Clear selection but keep the cursor position at its end
            found = editor->textCursor();
            found.clearSelection();
            editor->setTextCursor( found );
        }
    }
}


void MainWindow::replaceNextRegExp( const QString &str, const QString &repl, bool cs, bool fromStart, bool confirm )
{
    QRegExp regexp( str );
    regexp.setCaseSensitivity( cs? Qt::CaseSensitive: Qt::CaseInsensitive );

    QTextDocument::FindFlags flags = QTextDocument::FindFlags( 0 );
    int pos = fromStart ? 0 :
                          editor->textCursor().selectionEnd();
    QTextCursor found = editor->document()->find( regexp, pos, flags );
    if ( showFindResult( found )) {
        QString newText = found.selectedText();
        newText.replace( regexp, repl );
        if ( !replaceFindResult( editor->textCursor(), newText, confirm )) {
            // Clear selection but keep the cursor position at its end
            found = editor->textCursor();
            found.clearSelection();
            editor->setTextCursor( found );
        }
    }
}


void MainWindow::replacePrevious( const QString &str, const QString &repl, bool cs, bool words, bool fromEnd, bool confirm )
{
    QTextDocument::FindFlags flags = QTextDocument::FindBackward;
    if ( cs )
        flags |= QTextDocument::FindCaseSensitively;
    if ( words )
        flags |= QTextDocument::FindWholeWords;
    int pos = fromEnd ? editor->document()->characterCount() :
                        editor->textCursor().selectionStart();

    QTextCursor found = editor->document()->find( str, pos, flags );
    if ( showFindResult( found )) {
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
    QRegExp regexp( str );
    regexp.setCaseSensitivity( cs? Qt::CaseSensitive: Qt::CaseInsensitive );

    QTextDocument::FindFlags flags = QTextDocument::FindBackward;
    int pos = fromEnd ? editor->document()->characterCount() :
                        editor->textCursor().selectionStart();
    QTextCursor found = editor->document()->find( regexp, pos, flags );
    if ( showFindResult( found )) {
        QString newText = found.selectedText();
        newText.replace( regexp, repl );
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
    QTextDocument::FindFlags flags = QTextDocument::FindFlags( 0 );
    if ( cs )
        flags |= QTextDocument::FindCaseSensitively;
    if ( words )
        flags |= QTextDocument::FindWholeWords;
    if ( backwards )
        flags = QTextDocument::FindBackward;

    int pos = fromStart ? 0 :
                          editor->textCursor().selectionEnd();
    QTextCursor found = editor->document()->find( str, pos, flags );
    if ( found.isNull() ) {
        showMessage( tr("No matches."));
        found = editor->textCursor();
        found.clearSelection();
        return;
    }
    if ( confirm ) {
        int r = QMessageBox::question( this,
                                       tr("Confirm"),
                                       tr("Replace all occurences of \"%1\"?").arg( str ),
                                       QMessageBox::Yes | QMessageBox::No,
                                       QMessageBox::Yes
                                    );
        if ( r != QMessageBox::Yes )
            return;
    }
    int count = 0;
    while ( !found.isNull() ) {
        count++;
        found.insertText( repl );
        found = editor->document()->find( str, found.selectionEnd(), flags );
    }
    showMessage( tr("%1 occurences replaced.").arg( count ));
    found = editor->textCursor();
    found.clearSelection();
    editor->setTextCursor( found );
}


void MainWindow::replaceAllRegExp( const QString &str, const QString &repl, bool cs, bool fromStart, bool confirm, bool backwards )
{
    QRegExp regexp( str );
    regexp.setCaseSensitivity( cs? Qt::CaseSensitive: Qt::CaseInsensitive );

    QTextDocument::FindFlags flags = QTextDocument::FindFlags( 0 );
    if ( cs )
        flags |= QTextDocument::FindCaseSensitively;
    if ( backwards )
        flags = QTextDocument::FindBackward;
    int pos = fromStart ? 0 :
                          editor->textCursor().selectionEnd();
    QTextCursor found = editor->document()->find( regexp, pos, flags );
    if ( found.isNull() ) {
        showMessage( tr("No matches."));
        found = editor->textCursor();
        found.clearSelection();
        return;
    }
    if ( confirm ) {
        int r = QMessageBox::question( this,
                                       tr("Confirm"),
                                       tr("Replace all occurences of text matching expression \"%1\"?").arg( str ),
                                       QMessageBox::Yes | QMessageBox::No,
                                       QMessageBox::Yes
                                    );
        if ( r != QMessageBox::Yes )
            return;
    }
    int count = 0;
    QString newText;
    while ( !found.isNull() ) {
        count++;
        newText = found.selectedText();
        newText.replace( regexp, repl );
        found.insertText( repl );
        found = editor->document()->find( regexp, found.selectionEnd(), flags );
    }
    showMessage( tr("%1 occurences replaced.").arg( count ));
    found = editor->textCursor();
    found.clearSelection();
    editor->setTextCursor( found );
}



// ---------------------------------------------------------------------------
// OTHER METHODS
//

void MainWindow::createActions()
{

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
    QList<QKeySequence> saveShortcuts;
    saveShortcuts << QKeySequence("F2") << QKeySequence("Ctrl+S");
    saveAction->setShortcuts( saveShortcuts );
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


    // Edit menu actions

    undoAction = new QAction( tr("&Undo"), this );
    undoAction->setShortcut( QKeySequence::Undo );
    undoAction->setStatusTip( tr("Undo the previous edit") );
    connect( undoAction, SIGNAL( triggered() ), editor, SLOT( undo() ));

    redoAction = new QAction( tr("&Redo"), this );
    redoAction->setShortcut( QKeySequence::Redo );
    redoAction->setStatusTip( tr("Revert the previous undo operation") );
    connect( redoAction, SIGNAL( triggered() ), editor, SLOT( redo() ));

    cutAction = new QAction( tr("&Cut"), this );
    cutAction->setShortcut( QKeySequence::Cut );
    cutAction->setStatusTip( tr("Move the selected text to the clipboard") );
    connect( cutAction, SIGNAL( triggered() ), editor, SLOT( cut() ));

    copyAction = new QAction( tr("&Copy"), this );
    copyAction->setShortcut( QKeySequence::Copy );
    copyAction->setStatusTip( tr("Copy the selected text to the clipboard") );
    connect( copyAction, SIGNAL( triggered() ), editor, SLOT( copy() ));

    pasteAction = new QAction( tr("&Paste"), this );
    pasteAction->setShortcut( QKeySequence::Paste );
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

    findAgainAction = new QAction( tr("Find &again"), this );
    findAgainAction->setShortcut( tr("Ctrl+G"));
    findAgainAction->setStatusTip( tr("Repeat the last search") );
    connect( findAgainAction, SIGNAL( triggered() ), this, SLOT( findAgain() ));

    replaceAction = new QAction( tr("&Replace..."), this );
    QList<QKeySequence> replaceShortcuts;
    replaceShortcuts << QKeySequence::Replace << QKeySequence("Ctrl+R");
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


    aboutAction = new QAction( tr("&Product information"), this );
    aboutAction->setStatusTip( tr("Show product information") );
    connect( aboutAction, SIGNAL( triggered() ), this, SLOT( about() ));
}


void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu( tr("&File") );
    fileMenu->addAction( newAction );
    fileMenu->addAction( openAction );
    fileMenu->addAction( saveAction );
    fileMenu->addAction( saveAsAction );
    fileMenu->addSeparator();
    fileMenu->addAction( printAction );
    separatorAction = fileMenu->addSeparator();
    for ( int i = 0; i < MaxRecentFiles; i++ )
        fileMenu->addAction( recentFileActions[ i ] );
    fileMenu->addAction( clearRecentAction );
    fileMenu->addSeparator();
    fileMenu->addAction( exitAction );

    editMenu = menuBar()->addMenu( tr("&Edit") );
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

    optionsMenu = menuBar()->addMenu( tr("&Options") );
    optionsMenu->addAction( wrapAction );
    optionsMenu->addAction( editModeAction );
    optionsMenu->addAction( readOnlyAction );
    optionsMenu->addSeparator();
    optionsMenu->addAction( fontAction );

    menuBar()->addSeparator();
    helpMenu = menuBar()->addMenu( tr("&Help") );
    helpMenu->addAction( aboutAction );

}


void MainWindow::createContextMenu()
{
}


void MainWindow::createStatusBar()
{
    editModeLabel = new QLabel(" OVR ");
    editModeLabel->setAlignment( Qt::AlignHCenter );
    editModeLabel->setMinimumSize( editModeLabel->sizeHint() );

    positionLabel = new QLabel("000000:000000");
    positionLabel->setAlignment( Qt::AlignHCenter );
    positionLabel->setMinimumSize( positionLabel->sizeHint() );

    messagesLabel = new QLabel("                                       ");
    messagesLabel->setIndent( 3 );
    messagesLabel->setMinimumSize( messagesLabel->sizeHint() );

    modifiedLabel = new QLabel(" Modified ");
    modifiedLabel->setAlignment( Qt::AlignHCenter );
    modifiedLabel->setMinimumSize( modifiedLabel->sizeHint() );

    statusBar()->addWidget( messagesLabel, 1 );
    statusBar()->addWidget( editModeLabel );
    statusBar()->addWidget( positionLabel );
    statusBar()->addWidget( modifiedLabel );
    statusBar()->setMinimumSize( statusBar()->sizeHint() );

    messagesLabel->setForegroundRole( QPalette::ButtonText );
    editModeLabel->setForegroundRole( QPalette::ButtonText );
    positionLabel->setForegroundRole( QPalette::ButtonText );
    modifiedLabel->setForegroundRole( QPalette::ButtonText );

    updateStatusBar();
}


void MainWindow::readSettings()
{
    QSettings settings( SETTINGS_VENDOR, SETTINGS_APP );

    restoreGeometry( settings.value("geometry").toByteArray() );

    recentFiles = settings.value("recentFiles").toStringList();
    updateRecentFileActions();

    toggleEditMode( settings.value("overwrite", false ).toBool() );
    editModeAction->setChecked( editor->overwriteMode() );

    bool wrap = settings.value("wrapMode", true ).toBool();
    toggleWordWrap( wrap );
    wrapAction->setChecked( wrap );

    QString defaultFont;
    QFontDatabase fontdb;
    QStringList matching = fontdb.families().filter("Monotype Sans Duospace WT");
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

    settings.setValue("geometry",    saveGeometry() );
    settings.setValue("recentFiles", recentFiles );
    settings.setValue("overwrite",   editor->overwriteMode() );
    settings.setValue("wrapMode",
                      (editor->wordWrapMode() == QTextOption::WrapAtWordBoundaryOrAnywhere )?
                      true: false
                     );
    settings.setValue("editorFont",  editor->font().toString() );
}


bool MainWindow::okToContinue()
{
    if ( isWindowModified() ) {
        int r = QMessageBox::warning( this,
                                      tr("Text Editor"),
                                      tr("There are unsaved changes. "
                                         "Do you want to save the changes?"),
                                      QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                                      QMessageBox::Save
                                    );
        if ( r == QMessageBox::Save )
            return save();
        else if ( r == QMessageBox::Cancel )
            return false;
    }
    return true;
}


bool MainWindow::loadFile( const QString &fileName, bool createIfNew )
{
    QFile file( fileName );
    if ( !file.open( QIODevice::ReadOnly | QFile::Text )) {
        if ( createIfNew ) {
            editor->clear();
            showMessage( tr("New file: %1").arg( QDir::toNativeSeparators( fileName )));
        }
        else {
            QMessageBox::critical( this, tr("Error"), tr("The file could not be opened."));
            return false;
        }
    }
    else {
        QTextStream in( &file );
        QString text = in.readAll();
        editor->setPlainText( text );
        file.close();
        showMessage( tr("Opened file: %1").arg( QDir::toNativeSeparators( fileName )));
    }

    setCurrentFile( fileName );
    return true;
}


bool MainWindow::saveFile( const QString &fileName )
{
    QFile file( fileName );
    if ( !file.open( QIODevice::WriteOnly | QFile::Text )) {
        QMessageBox::critical( this, tr("Error"), tr("Error writing file"));
        return false;
    }
    QTextStream out( &file );
    QString text = editor->toPlainText();
    out << text;
    file.flush();
    file.close();

    setCurrentFile( fileName );
    showMessage( tr("Saved file: %1").arg( QDir::toNativeSeparators( fileName )));
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
    currentDir = QDir::currentPath();
    updateModified( false );
    QString shownName = tr("Untitled");
    if ( !currentFile.isEmpty() ) {
        currentDir = QDir::cleanPath( QFileInfo( fileName ).absolutePath() );
        shownName  = strippedName( currentFile );
        recentFiles.removeAll( currentFile );
        recentFiles.prepend( currentFile );
        updateRecentFileActions();
    }
    setWindowTitle( tr("Text Editor - %1 [*]").arg( shownName ));
}


void MainWindow::updateRecentFileActions()
{
    QMutableStringListIterator i( recentFiles );
    while ( i.hasNext() ) {
        if ( !QFile::exists( i.next() )) {
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
    QMessageBox::information( this, tr("Usage"),
                              tr("qe [ <i>filename</i> [ /read ]] &nbsp; | &nbsp; [ /h | /? ]"),
                              QMessageBox::Ok
                            );
}


void MainWindow::setReadOnly( bool readOnly )
{
    editor->setTextInteractionFlags( readOnly ?
                                Qt::TextBrowserInteraction | Qt::TextSelectableByKeyboard :
                                Qt::TextEditorInteraction
                           );
    updateModeLabel();
}


bool MainWindow::showFindResult( QTextCursor found )
{
    bool isFound = false;
    if ( found.isNull() ) {
        showMessage( tr("No matches."));
        found = editor->textCursor();
        found.clearSelection();
    }
    else {
        QTextCursor temp( found );
        temp.setPosition( temp.selectionStart() );
        showMessage( tr("Found match at %1:%2").arg( temp.blockNumber() + 1 ).arg( temp.positionInBlock() ));
        isFound = true;
    }
    editor->setTextCursor( found );
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
