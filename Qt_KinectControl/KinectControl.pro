#-------------------------------------------------
#
# Project created by QtCreator 2014-10-27T21:32:38
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = KinectControl
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    Main.cpp \
    objects/kinect.cpp \
    objects/video.cpp \
    objects/textinterpreter.cpp \
    objects/kkey.cpp




unix:!macx: LIBS += -L$$PWD/../../../usr/lib/i386-linux-gnu/ -lfreenect

INCLUDEPATH += $$PWD/../libfreenect/include
DEPENDPATH += $$PWD/../libfreenect/include



unix:!macx: LIBS += -L$$PWD/../../../usr/lib/i386-linux-gnu/ -lglut

INCLUDEPATH += $$PWD/../../../usr/include
DEPENDPATH += $$PWD/../../../usr/include

unix:!macx: LIBS += -L$$PWD/../../../usr/lib/i386-linux-gnu/ -lpthread

INCLUDEPATH += $$PWD/../../../usr/include
DEPENDPATH += $$PWD/../../../usr/include

unix:!macx: LIBS += -L$$PWD/../../../usr/lib/i386-linux-gnu/ -lGL

INCLUDEPATH += $$PWD/../../../usr/include
DEPENDPATH += $$PWD/../../../usr/include

unix:!macx: LIBS += -L$$PWD/../../../usr/lib/i386-linux-gnu/ -lX11

INCLUDEPATH += $$PWD/../../../usr/lib/i386-linux-gnu
DEPENDPATH += $$PWD/../../../usr/lib/i386-linux-gnu

unix:!macx: LIBS += -L$$PWD/../../../usr/lib/i386-linux-gnu/ -lXtst

INCLUDEPATH += $$PWD/../../../usr/include
DEPENDPATH += $$PWD/../../../usr/include

HEADERS += \
    objects/kinect.h \
    objects/video.h \
    objects/textinterpreter.h \
    objects/kkey.h


# added by sbVB
# LIBS += /usr/local/lib/libfreenect.so
LIBS += /usr/local/lib/libfreenect.so.0.5



