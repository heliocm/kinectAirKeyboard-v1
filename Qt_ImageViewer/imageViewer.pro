#-------------------------------------------------
#
# Project created by QtCreator 2014-11-20T11:37:23
#
#-------------------------------------------------

QT       += core gui
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = imageViewer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    imagefile.cpp

HEADERS  += mainwindow.h \
    imagefile.h

FORMS    += mainwindow.ui
