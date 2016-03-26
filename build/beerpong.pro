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
LIBS += -lfreenect

SOURCES += ../src/main.cpp \
    ../src/mainwindow.cpp \
    ../src/controller.cpp \
    ../src/player.cpp \
    ../src/timer.cpp \
    ../src/kinect.cpp

HEADERS += ../include/mainwindow.h \
    ../include/controller.h \
    ../include/player.h \
    ../include/timer.h \
    ../include/kinect.h

FORMS += ../ui/mainwindow.ui


