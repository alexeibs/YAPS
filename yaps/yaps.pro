#-------------------------------------------------
#
# Project created by QtCreator 2013-08-19T19:21:48
#
#-------------------------------------------------

CONFIG += c++11

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = yaps
TEMPLATE = app

SOURCES += main.cpp

INCLUDEPATH += $$PWD/..
DEPENDPATH += $$PWD/..

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../yapslib/release/
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../yapslib/debug/
else:unix: LIBS += -L$$OUT_PWD/../yapslib/

LIBS += -lyapslib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../cryptopp/release/
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../cryptopp/debug/
else:unix: LIBS += -L$$OUT_PWD/../cryptopp/

LIBS += -lcryptopp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../kbhook/release/ -lwinkbhook
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../kbhook/debug/ -lwinkbhook

win32:RC_FILE += win.rc
