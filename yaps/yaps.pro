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
    PasswordEditDialog.cpp \
    timer_impl.cpp \
    crypto_factory_impl.cpp \
    crypto_impl.cpp \
    crypto_engine_impl.cpp \
    GlobalPasswordDialog.cpp \
    password_prompt_impl.cpp

HEADERS  += mainwindow.h \
    MainActions.h \
    Database.h \
    View.h \
    PasswordsModel.h \
    SecureClipboard.h \
    PasswordEditDialog.h \
    crypto_factory_impl.h \
    timer_impl.h \
    timer.h \
    crypto_status.h \
    crypto_engine_impl.h \
    crypto_engine.h \
    crypto_impl.h \
    password_lock.h \
    crypto.h \
    password_prompt.h \
    GlobalPasswordDialog.h \
    password_prompt_impl.h

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
