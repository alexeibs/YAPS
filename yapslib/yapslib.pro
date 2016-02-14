#-------------------------------------------------
#
# Project created by QtCreator 2015-12-23T01:38:24
#
#-------------------------------------------------

QT       += widgets sql

TARGET = yapslib
TEMPLATE = lib
CONFIG += c++11
DEFINES += YAPSLIB_EXPORTS

SOURCES += clipboard_impl.cpp \
    controller_impl.cpp \
    crypto_engine_impl.cpp \
    crypto_factory_impl.cpp \
    crypto_impl.cpp \
    Database.cpp \
    global_password_dialog.cpp \
    mainwindow.cpp \
    message_box_factory_impl.cpp \
    password_edit_dialog.cpp \
    password_prompt_impl.cpp \
    password_record.cpp \
    password_record_editor_impl.cpp \
    passwords_model_impl.cpp \
    secure_clipboard_impl.cpp \
    timer_impl.cpp \
    toolbar_view.cpp \
    View.cpp

HEADERS += clipboard.h \
    clipboard_impl.h \
    controller.h \
    controller_impl.h \
    crypto.h \
    crypto_engine.h \
    crypto_engine_impl.h \
    crypto_factory_impl.h \
    crypto_impl.h \
    crypto_status.h \
    Database.h \
    global_password_dialog.h \
    mainwindow.h \
    message_box_factory.h \
    message_box_factory_impl.h \
    password_edit_dialog.h \
    password_lock.h \
    password_prompt.h \
    password_prompt_impl.h \
    password_record.h \
    password_record_editor.h \
    password_record_editor_impl.h \
    passwords_model.h \
    passwords_model_impl.h \
    secure_clipboard.h \
    secure_clipboard_impl.h \
    timer.h \
    timer_impl.h \
    toolbar_view.h \
    View.h \
    view_state.h \
    expimp.h

RESOURCES += \
    resources/main.qrc

TRANSLATIONS = $$PWD/resources/ru_RU.ts

INCLUDEPATH += $$PWD/..
DEPENDPATH += $$PWD/..

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../cryptopp/release/
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../cryptopp/debug/
else:unix: LIBS += -L$$OUT_PWD/../cryptopp/

LIBS += -lcryptopp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../kbhook/release/ -lwinkbhook
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../kbhook/debug/ -lwinkbhook
