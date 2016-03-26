 QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = beerpong
TEMPLATE = app

CONFIG -= console
CONFIG += c++11
CONFIG += warn_off

INCLUDEPATH += /usr/include
INCLUDEPATH += /usr/local/include
INCLUDEPATH += ../include
INCLUDEPATH += ../../include

LIBS += -L/usr/lib
LIBS += -L/usr/local/lib
LIBS += -lopencv_core
LIBS += -lopencv_imgproc
LIBS += -lopencv_highgui
LIBS += -lopencv_videoio
LIBS += -lopencv_video

release: DESTDIR = ..
debug:   DESTDIR = ..

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

SOURCES += ../src/main.cpp \
../src/mainwindow.cpp \

HEADERS += ../include/mainwindow.h

FORMS += ../ui/mainwindow.ui


