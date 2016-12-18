#-------------------------------------------------
#
# Project created by QtCreator 2016-08-03T14:59:03
#
#-------------------------------------------------

QT       += core gui serialport # testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#TARGET = CATools
TARGET = QuickInstall
TEMPLATE = app


LIBS += -luser32

SOURCES += main.cpp\
        mainwindow.cpp \
    styleloader.cpp \
    console.cpp \
    treemodelcompleter.cpp \
    ipwidget.cpp \
    consolecontroller.cpp \
    consoleview.cpp \
    clickablelabel.cpp \
    netconfigtab.cpp \
    linestab.cpp \
    switcheswidget.cpp \
    switchlabel.cpp \
    warningwidget.cpp \
    headerwidget.cpp \
    consoledialog.cpp \
    infowidget.cpp \
    myhighlighter.cpp \
    logbrowserdialog.cpp \
    logbrowser.cpp \
    consolefooter.cpp

HEADERS  += mainwindow.h \
    styleloader.h \
    console.h \
    treemodelcompleter.h \
    ipwidget.h \
    line.h \
    consolecontroller.h \
    consoleview.h \
    clickablelabel.h \
    netconfigtab.h \
    linestab.h \
    switcheswidget.h \
    switchlabel.h \
    warningwidget.h \
    headerwidget.h \
    consoledialog.h \
    infowidget.h \
    myhighlighter.h \
    globalvars.h \
    logbrowserdialog.h \
    logbrowser.h \
    consolefooter.h

FORMS    += mainwindow.ui \
    switcheswidget.ui \
    warningwidget.ui \
    infowidget.ui \
    form.ui \
    consolefooter.ui
CONFIG -= embed_manifest_exe
RC_FILE += ico.rc
RESOURCES += res.qrc
QMAKE_CXXFLAGS += -std=gnu++11
DISTFILES += \
    xmlfile.txt \
    fonts/Linotype - Neue Haas Unica Pro Bold.otf \
    fonts/Linotype - Neue Haas Unica Pro Light.otf \
    fonts/Linotype - Neue Haas Unica Pro Medium.otf \
    fonts/Linotype - Neue Haas Unica Pro Thin.otf \
    fonts/Linotype - Neue Haas Unica Pro.otf \
    images/port-icon.png


