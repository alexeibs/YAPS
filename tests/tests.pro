#-------------------------------------------------
#
# Project created by QtCreator 2015-12-23T02:21:51
#
#-------------------------------------------------

#QT       += testlib

QT       -= gui

TARGET = tests
CONFIG   += console  c++11
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    main.cpp \
    crypto_factory_test.cpp \
    test_helpers.cpp

HEADERS += \
    crypto_engine_mock.h \
    crypto_mock.h \
    password_prompt_mock.h \
    timer_mock.h \
    test_helpers.h \
    crypto_status_view_mock.h

DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += $$PWD/..
DEPENDPATH += $$PWD/..

# Google Test/Mock
INCLUDEPATH += $$PWD/../googletest/googletest/include
INCLUDEPATH += $$PWD/../googletest/googlemock/include
LIBS += -L$$OUT_PWD/../googletest/ -lgtest
LIBS += -L$$OUT_PWD/../googlemock/ -lgmock

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../yapslib/release/ -lyapslib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../yapslib/debug/ -lyapslib
else:unix: LIBS += -L$$OUT_PWD/../yapslib/ -lyapslib
