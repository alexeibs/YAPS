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

build_pass:CONFIG(debug, debug|release): {
    LIBS += -L../cryptopp562/debug
}

build_pass:CONFIG(release, debug|release): {
    LIBS += -L../cryptopp562/release
}

INCLUDEPATH += ../
LIBS += -lcryptopp562
