#include "SecureClipboard.h"

#include <QTimer>
#include <QApplication>
#include <QClipboard>

#define CLIPBOARD_TIMEOUT 5000

SecureClipboard& SecureClipboard::instance()
{
    static SecureClipboard single;
    return single;
}

SecureClipboard::SecureClipboard()
{
    m_timer = new QTimer(this);
    m_timer->setSingleShot(true);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(clear()));
}

void SecureClipboard::clear()
{
    QApplication::clipboard()->clear();
}

void SecureClipboard::setText(const QString& text)
{
    m_timer->stop();
    QApplication::clipboard()->setText(text);
    m_timer->start(5000);
}
