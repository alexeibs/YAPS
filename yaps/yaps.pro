#-------------------------------------------------
#
# Project created by QtCreator 2013-08-19T19:21:48
#
#-------------------------------------------------

CONFIG += c++11

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = yaps
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    MainActions.cpp

HEADERS  += mainwindow.h \
    MainActions.h

RESOURCES += \
    resources/main.qrc

debug:DEFINES += DEBUG_VERSION
