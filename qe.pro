######################################################################
# Automatically generated by qmake (2.01a) Fri Jan 5 22:44:28 2018
######################################################################

TEMPLATE = app
TARGET =
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += finddialog.h replacedialog.h gotolinedialog.h iodialog.h eastring.h os2codec.h mainwindow.h qetextedit.h ctlutils.h threads.h
FORMS += finddialog.ui replacedialog.ui gotolinedialog.ui
SOURCES += eastring.cpp os2codec.cpp finddialog.cpp replacedialog.cpp gotolinedialog.cpp iodialog.cpp main.cpp mainwindow.cpp qetextedit.cpp ctlutils.cpp threads.cpp
RESOURCES += qe.qrc
os2:HEADERS += os2native.h
os2:SOURCES += os2native.cpp
os2:RC_FILE = qe.rc
win32:RC_FILE = qe_win.rc

