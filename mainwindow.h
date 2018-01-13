#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


#define PROGRAM_VERSION "0.9.3"
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
class QActionGroup;
class QLabel;
class QPlainTextEdit;
class QTextCursor;
class FindDialog;
class ReplaceDialog;

typedef struct _FindParams_t
{
    QString text;
    bool bCase;
    bool bWords;
    bool bBackward;
    bool bRe;
} FindParams;


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
    void updateEncodingLabel();
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
    void setTextEncoding();

private:
    // Setup methods
    void createActions();
    void createEncodingActions();
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
    void updateEncoding();
    bool showFindResult( QTextCursor found );
    bool replaceFindResult( QTextCursor found, const QString newText, bool confirm );

    // GUI objects
    QPlainTextEdit *editor;
    FindDialog *findDialog;
    ReplaceDialog *replaceDialog;

    QLabel *editModeLabel;
    QLabel *messagesLabel;
    QLabel *encodingLabel;
    QLabel *positionLabel;
    QLabel *modifiedLabel;

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

    QMenu   *encodingMenu;
    QAction *localeAction;      // Default encoding for locale

    // Central/Eastern European
    QMenu   *centEuroMenu;
    QAction *iso88592Action;    // ISO 8859-2 (Latin-2)
    QAction *win1250Action;     // Windows-1250 (MS Latin-2)

    // Cyrillic
    QMenu   *cyrillicMenu;
    QAction *ibm866Action;      // IBM-866 (Russian)
    QAction *iso88595Action;    // ISO 8859-5 (Cyrillic)
    QAction *koi8rAction;       // KOI8-R (Russian Internet encoding)
    QAction *koi8uAction;       // KOI8-U (Ukrainian Internet encoding)
    QAction *win1251Action;     // Windows-1251 (Cyrillic)

    // East Asian
    QMenu   *eastAsiaMenu;
    QAction *big5Action;        // Big-5
    QAction *gbAction;          // GBK/GB18030
    QAction *eucJpAction;       // EUC-JP
    QAction *iso2022JpAction;   // ISO-2022-JP (Japanese mail encoding)
    QAction *sjisAction;        // Shift-JIS (codepage 932)
    QAction *eucKrAction;       // EUC-KR

    // Middle Eastern
    QMenu   *midEastMenu;
    QAction *iso88596Action;    // ISO 8859-6 (Arabic)
    QAction *iso88598Action;    // ISO 8859-8 (Hebrew)
    QAction *win1255Action;     // Windows-1255 (MS Hebrew)
    QAction *win1256Action;     // Windows-1256 (MS Arabic)

    // Northern European
    QMenu   *northEuroMenu;
    QAction *iso88594Action;    // ISO 8859-4 (Latin-4)
    QAction *iso885910Action;   // ISO 8859-10 (Latin-6)
    QAction *iso885913Action;   // ISO 8859-13 (Latin-7)
    QAction *win1257Action;     // Windows-1257 (Baltic)
    QAction *iso885914Action;   // ISO 8859-14 (Latin-8)

    // South Asian
    QMenu   *southAsiaMenu;
    QAction *tsciiAction;       // TSCII (Tamil)
    QAction *ibm874Action;      // IBM-874 (Thai TIS-620)
    QAction *win1258Action;     // Windows-1258 (Vietnamese)

    // Southern European
    QMenu   *southEuroMenu;
    QAction *iso88593Action;    // ISO 8859-3 (Latin-3)
    QAction *iso88597Action;    // ISO 8859-7 (Greek)
    QAction *win1253Action;     // Windows-1253 (MS Greek)
    QAction *win1254Action;     // Windows-1254 (MS Latin-5)
    QAction *iso885916Action;   // ISO 8859-16 (Latin-10)

    // Western European
    QMenu   *westEuroMenu;
    QAction *aromanAction;      // Apple Roman
    QAction *ibm850Action;      // IBM-850 (PC Latin-1)
//    QAction *iso88591Action;    // ISO 8859-1 (Latin-1)
    QAction *iso885915Action;   // ISO 8859-15 (Latin-9)
    QAction *win1252Action;     // Windows-1252 (MS Latin-1)

    QMenu   *unicodeMenu;
    QAction *utf16Action;       // UTF-16 (LE)
    QAction *utf16beAction;     // UTF-16 (BE)
    QAction *utf8Action;        // UTF-8

    QActionGroup *encodingGroup;

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
    QString     currentEncoding;
    int         lastGoTo;
    FindParams  lastFind;

};

#endif
