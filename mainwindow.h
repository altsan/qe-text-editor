/******************************************************************************
** QE - mainwindow.h
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define USE_IO_THREADS


#include <QMainWindow>
#include <QDateTime>
#include <QProcess>
#include "version.h"


#define PROGRAM_VERSION     VER_FILEVERSION_STR
#define SETTINGS_VENDOR     "ATutils"
#define SETTINGS_APP        "QE"
#define ENCODING_EA_NAME    ".CODEPAGE"

// Constants for QtAssistant-based cross-platform help
#define HELP_HTML_ROOT      "qthelp://altsan.qe/help/"
#define HELP_HTML_GENERAL   "qe.1.html"
#define HELP_HTML_LINE      "qe.5.html"
#define HELP_HTML_FIND      "qe.6.html"
#define HELP_HTML_REPLACE   "qe.7.html"
#define HELP_HTML_KEYS      "qe.9.html"

// Constants for OS/2 native help
#define HELP_PANEL_GENERAL        1
#define HELP_PANEL_LINE         110
#define HELP_PANEL_FIND         120
#define HELP_PANEL_REPLACE      130
#define HELP_PANEL_KEYS         200


#if 1
#define DEFAULT_FILENAME_FILTERS                            \
    "Text files (*.txt readme*);;"                          \
    "Log files (*.log *.l? *.l? *.err);;"                   \
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
class QeTextEdit;
class QTextCursor;
class FindDialog;
class ReplaceDialog;
class QeOpenThread;
class QeSaveThread;


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
    ~MainWindow();
    bool loadFile( const QString &fileName, bool createIfNew );
    bool mapNameToEncoding( QString &encoding );
    void openAsEncoding( QString fileName, bool createIfNew, QString encoding );
    void showUsage();
    void setReadOnly( bool readonly );
    void setTextEncoding( QString newEncoding );

protected:
    void closeEvent( QCloseEvent *event );
    void dragEnterEvent( QDragEnterEvent *event );
    void dropEvent (QDropEvent *event );

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
    void showGeneralHelp();
    void showKeysHelp();
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
    void updateFindHistory( const QString &findString );
    void updateReplaceHistory( const QString &replaceString );
    void goToLine();
    void setTextEncoding();
    void readProgress( int percent );
    void readDone();
    void readCancel();
    void saveProgress( int percent );
    void saveDone( qint64 iSize );


private:
    // Setup methods
    void createActions();
    void createEncodingActions();
    void createMenus();
    void createContextMenu();
    void createStatusBar();
    void createHelp();
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
    bool showFindResult( QTextCursor found, const QString &str );
    bool replaceFindResult( QTextCursor found, const QString newText, bool confirm );
    QString getFileCodepage( const QString &fileName );
    void setFileCodepage( const QString &fileName, const QString &encodingName );
    void updateEncoding();
    void launchAssistant( const QString &panel );

    // GUI objects
    QeTextEdit    *editor;
    FindDialog    *findDialog;
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

    // Central/East European
    QMenu   *centEuroMenu;
    QAction *ibm852Action;      // IBM-852 (Latin-2)                    (codepage 852)
    QAction *iso88592Action;    // ISO 8859-2 (Latin-2)                 (codepage 912)
    QAction *win1250Action;     // Windows-1250 (MS Latin-2)            (codepage 1250)

    // Cyrillic
    QMenu   *cyrillicMenu;
    QAction *ibm855Action;      // IBM-855 (Cyrillic)                   (codepage 852)
    QAction *ibm866Action;      // IBM-866 (Russian)                    (codepage 866)
    QAction *ibm1125Action;     // IBM-1125 (Ukrainian)                 (codepage 1125)
    QAction *ibm1131Action;     // IBM-1131 (Belarusian)                (codepage 1131)
    QAction *iso88595Action;    // ISO 8859-5 (Cyrillic)                (codepage 915)
    QAction *koi8rAction;       // KOI8-R (Russian Internet encoding)   (codepage 878)
    QAction *koi8uAction;       // KOI8-U (Ukrainian Internet encoding) (codepage 1168)
    QAction *win1251Action;     // Windows-1251 (Cyrillic)              (codepage 1251)

    // East Asian
    QMenu   *eastAsiaMenu;
    QAction *big5Action;        // Big-5                                (codepage 950)
    QAction *gbkAction;         // GBK/GB18030                          (codepage 1386)
    QAction *gbAction;          // GB2312                               (codepage 1381)
    QAction *eucJpAction;       // EUC-JP                               (codepage 954)
    QAction *iso2022JpAction;   // ISO-2022-JP (Japanese mail encoding) (codepage 4992)
    QAction *sjisAction;        // Shift-JIS (codepage 932)             (codepage 932)
    QAction *eucKrAction;       // EUC-KR                               (codepage 970)
    QAction *uhcAction;         // Unified Hangul Code                  (codepage 1363)

    // Middle Eastern
    QMenu   *midEastMenu;
    QAction *ibm864Action;      // IBM-864 (Arabic)                     (codepage 864)
    QAction *iso88596Action;    // ISO 8859-6 (Arabic)                  (codepage 1089)
    QAction *iso88598Action;    // ISO 8859-8 (Hebrew)                  (codepage 916)
    QAction *ibm862Action;      // IBM-867 (Hebrew)                     (codepage 867)
    QAction *win1255Action;     // Windows-1255 (MS Hebrew)             (codepage 1255)
    QAction *win1256Action;     // Windows-1256 (MS Arabic)             (codepage 1256)

    // North European
    QMenu   *northEuroMenu;
    QAction *ibm922Action;      // IBM-922 (Estonian)                   (codepage 922)
    QAction *ibm861Action;      // IBM-861 (PC Icelandic)               (codepage 861)
    QAction *ibm865Action;      // IBM-865 (PC Nordic)                  (codepage 865)
    QAction *iso88594Action;    // ISO 8859-4 (Latin-4)                 (codepage 914)
    QAction *iso885910Action;   // ISO 8859-10 (Latin-6)                (codepage 919)
    QAction *iso885913Action;   // ISO 8859-13 (Latin-7)                (codepage 921)
    QAction *win1257Action;     // Windows-1257 (Baltic)                (codepage 1257)
    QAction *iso885914Action;   // ISO 8859-14 (Latin-8)                (no codepage - use 1090)

    // South Asian
    QMenu   *southAsiaMenu;
    QAction *tsciiAction;       // TSCII (Tamil)                        (no codepage - use 1092)
    QAction *ibm874Action;      // IBM-874 (Thai TIS-620)               (codepage 874)
    QAction *win1258Action;     // Windows-1258 (Vietnamese)            (codepage 1258)

    // South European
    QMenu   *southEuroMenu;
    QAction *ibm857Action;      // IBM-857 (PC Latin-5)                 (codepage 857)
    QAction *ibm869Action;      // IBM-869 (PC Greek)                   (codepage 869)
    QAction *iso88593Action;    // ISO 8859-3 (Latin-3)                 (codepage 913)
    QAction *iso88597Action;    // ISO 8859-7 (Greek)                   (codepage 813)
    QAction *win1253Action;     // Windows-1253 (MS Greek)              (codepage 1253)
    QAction *win1254Action;     // Windows-1254 (MS Latin-5)            (codepage 1254)
    QAction *iso885916Action;   // ISO 8859-16 (Latin-10)               (no codepage - use 1091)

    // Western European
    QMenu   *westEuroMenu;
    QAction *aromanAction;      // Apple Roman                          (codepage 1275)
    QAction *ibm437Action;      // IBM-437 (PC United States)           (codepage 437)
    QAction *ibm850Action;      // IBM-850 (PC Latin-1)                 (codepage 850)
    QAction *ibm858Action;      // IBM-858 (PC Latin-1 w/Euro)          (codepage 858)
    QAction *ibm859Action;      // IBM-859 (PC Latin-9)                 (codepage 859)
    QAction *ibm860Action;      // IBM-860 (PC Portuguese)              (codepage 860)
    QAction *ibm863Action;      // IBM-863 (PC Canadian French)         (codepage 863)
    QAction *iso885915Action;   // ISO 8859-15 (Latin-9)                (codepage 923)
    QAction *win1252Action;     // Windows-1252 (MS Latin-1)            (codepage 1252)

    QMenu   *unicodeMenu;
    QAction *utf16Action;       // UTF-16 (LE)                          (codepage 1202)
    QAction *utf16beAction;     // UTF-16 (BE)                          (codepage 1200)
    QAction *utf8Action;        // UTF-8                                (codepage 1208)

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
    QAction *helpGeneralAction;
    QAction *helpKeysAction;
    QAction *aboutAction;


    // Global preferences from options dialog (not yet implemented)

    bool    useNativeFD;
    bool    saveLastDir;
    bool    warnUnmodified;
    bool    warnFileChanged;
    bool    detectUpdate;
    bool    quickFindCS;
    bool    quickFindWords;
    int     ctrlSBehaviour;


    // Other class variables

    enum { MaxRecentFinds = 10 };

    QStringList recentFiles;
    QString     currentFile;
    QString     currentDir;
    QString     currentEncoding;
    QString     currentFilter;
    QDateTime   currentModifyTime;
    bool        encodingChanged;
    int         lastGoTo;
    FindParams  lastFind;
    QStringList recentFinds;
    QStringList recentReplaces;


#ifdef USE_IO_THREADS
    QeOpenThread *openThread;
    QeSaveThread *saveThread;
    bool         isReadThreadActive;
    bool         isSaveThreadActive;
#endif

    // Program help (platform specific implementation)
    void *helpInstance;

    // QtAssistant process
    QProcess *helpProcess;

};

#endif
