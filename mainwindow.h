#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#define PROGRAM_VERSION "0.9"
#define SETTINGS_VENDOR "ATutils"
#define SETTINGS_APP    "QuickEditor"

class QAction;
class QLabel;
class FindDialog;
class QTextEdit;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    bool loadFile( const QString &fileName );
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
    void replace();
    void about();
    void openRecentFile();
    void clearRecentFiles();
    void toggleEditMode( bool ovr );
    void toggleReadOnly( bool readOnly );
    void toggleWordWrap();
    void updateStatusBar();
    void updateModeLabel();
    void updatePositionLabel();
    void updateModified();
    void updateModified( bool isModified );
    void setEditorFont();

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

    // GUI objects
    QTextEdit *editor;
//    FindDialog *findDialog;

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

};

#endif
