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

SOURCES += main.cpp\
        mainwindow.cpp \
    MainActions.cpp \
    Database.cpp \
    View.cpp \
    PasswordsModel.cpp \
    SecureClipboard.cpp \
    Crypto.cpp \
    PasswordEditDialog.cpp \
    GlobalPasswordDialog.cpp

HEADERS  += mainwindow.h \
    MainActions.h \
    Database.h \
    View.h \
    PasswordsModel.h \
    SecureClipboard.h \
    Crypto.h \
    PasswordEditDialog.h \
    GlobalPasswordDialog.h

RESOURCES += \
    resources/main.qrc

win32:RC_FILE += resources/win.rc

build_pass:CONFIG(debug, debug|release): {
    LIBS += -L../cryptopp562/debug -L../kbhook/debug
}

build_pass:CONFIG(release, debug|release): {
    LIBS += -L../cryptopp562/release -L../kbhook/release
}

INCLUDEPATH += ../
LIBS += -lcryptopp562
win32:LIBS += -lwinkbhook

TRANSLATIONS = resources/ru_RU.ts
win32:TRANSLATIONS = resources\ru_RU.ts
