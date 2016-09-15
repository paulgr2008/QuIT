#-------------------------------------------------
#
# Project created by QtCreator 2016-08-03T14:59:03
#
#-------------------------------------------------

QT       += core gui widgets\
        network \
        serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CATools
TEMPLATE = app

LIBS += -luser32

SOURCES += main.cpp\
        mainwindow.cpp \
    styleloader.cpp \
    console.cpp \
    consoletab.cpp \
    treemodelcompleter.cpp \
    ipwidget.cpp \
    consolecontroller.cpp

HEADERS  += mainwindow.h \
    styleloader.h \
    console.h \
    consoletab.h \
    treemodelcompleter.h \
    ipwidget.h \
    line.h \
    consolecontroller.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
QMAKE_CXXFLAGS += -std=gnu++11
DISTFILES += \
    xmlfile.txt
