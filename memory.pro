#-------------------------------------------------
#
# Project created by QtCreator 2016-03-06T16:34:27
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
include(QtAwesome/QtAwesome.pri)

QMAKE_MAC_SDK = macosx10.11

TARGET = memory
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
