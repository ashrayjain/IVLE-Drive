#-------------------------------------------------
#
# Project created by QtCreator 2013-12-11T20:33:23
#
#-------------------------------------------------

QT       += core gui network webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IVLEDrive
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    parser.cpp \
    ivleconnector.cpp \
    storage.cpp \
    ivlelogindialog.cpp

HEADERS  += mainwindow.h \
    parser.h \
    ivleconnector.h \
    storage.h \
    ivlelogindialog.h

FORMS    += mainwindow.ui \
    ivlelogindialog.ui
