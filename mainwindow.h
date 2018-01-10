#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


#define PROGRAM_VERSION "0.9.2"
#define SETTINGS_VENDOR "ATutils"
#define SETTINGS_APP    "QuickEditor"

#if 0
#define DEFAULT_FILENAME_FILTERS                            \
    "Text files (*.txt readme*);;"                          \
    "Command files (*.cmd *.bat *.rex *.orx *.sh *.vrx);;"  \
    "All files (*)"
#else
#define DEFAULT_FILENAME_FILTERS                            \
    "Text files (*.txt readme*);;"                          \
    "Assembler source code (*.asm);;"                       \
    "BASIC source code (*.bas);;"                           \
    "C/C++ source code (*.c *.h *.cpp *.hpp *.cc);;"        \
    "Command files (*.cmd *.bat *.rex *.orx *.sh *.vrx);;"  \
    "Java source code (*.jav *.java);;"                     \
    "Pascal source code (*.pas);;"                          \
    "Python files (*.py);;"                                 \
    "WWW files (*.htm *.html *.css *.cgi *.js *.php);;"     \
    "All files (*)"
#endif

class QAction;
class QLabel;
class QPlainTextEdit;
class QTextCursor;
class FindDialog;
class ReplaceDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    bool loadFile( const QString &fileName, bool createIfNew );
    void showUsage();
    void setReadOnly( bool readonly );

protected:
    void closeEvent( QCloseEvent *event );

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    bool print();
    void find();
    void findAgain();
    void replace();
    void about();
    void openRecentFile();
    void clearRecentFiles();
    bool toggleEditMode( bool ovr );
    bool toggleReadOnly( bool readOnly );
    bool toggleWordWrap( bool bWrap );
    void updateStatusBar();
    void updateModeLabel();
    void updatePositionLabel();
    void updateModified();
    void updateModified( bool isModified );
    void setEditorFont();
    void findNext( const QString &str, bool cs, bool words, bool fromStart );
    void findNextRegExp( const QString &str, bool cs, bool fromStart );
    void findPrevious( const QString &str, bool cs, bool words, bool fromEnd );
    void findPreviousRegExp( const QString &str, bool cs, bool fromEnd );
    void replaceNext( const QString &str, const QString &repl, bool cs, bool words, bool absolute, bool confirm );
    void replaceNextRegExp( const QString &str, const QString &repl, bool cs, bool absolute, bool confirm );
    void replacePrevious( const QString &str, const QString &repl, bool cs, bool words, bool absolute, bool confirm );
    void replacePreviousRegExp( const QString &str, const QString &repl, bool cs, bool absolute, bool confirm );
    void replaceAll( const QString &str, const QString &repl, bool cs, bool words, bool absolute, bool confirm, bool backwards );
    void replaceAllRegExp( const QString &str, const QString &repl, bool cs, bool absolute, bool confirm, bool backwards );
    void goToLine();

private:
    // Setup methods
    void createActions();
    void createMenus();
    void createContextMenu();
    void createStatusBar();
    void readSettings();
    void writeSettings();

    // Action methods
    bool okToContinue();
    bool saveFile( const QString &fileName );

    // Misc methods
    void setCurrentFile( const QString &fileName );
    void updateRecentFileActions();
    QString strippedName( const QString &fullFileName );
    void showMessage( const QString &message );
    bool showFindResult( QTextCursor found );
    bool replaceFindResult( QTextCursor found, const QString newText, bool confirm );

    // GUI objects
    QPlainTextEdit *editor;
    FindDialog *findDialog;
    ReplaceDialog *replaceDialog;

    QLabel *editModeLabel;
    QLabel *positionLabel;
    QLabel *modifiedLabel;
    QLabel *messagesLabel;

    enum { MaxRecentFiles = 5 };

    QMenu   *fileMenu;
    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *printAction;
    QAction *recentFileActions[ MaxRecentFiles ];
    QAction *clearRecentAction;
    QAction *separatorAction;
    QAction *exitAction;

    QMenu   *editMenu;
    QAction *undoAction;
    QAction *redoAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *selectAllAction;
    QAction *findAction;
    QAction *findAgainAction;
    QAction *replaceAction;
    QAction *goToAction;

    QMenu   *optionsMenu;
    QAction *wrapAction;
    QAction *editModeAction;
    QAction *readOnlyAction;
    QAction *fontAction;
    QAction *coloursAction;
    QAction *autosaveAction;

    QMenu   *helpMenu;
    QAction *aboutAction;

    // Other class variables

    QStringList recentFiles;
    QString     currentFile;
    QString     currentDir;
    int         lastGoTo;

};

#endif
