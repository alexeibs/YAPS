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
    : m_needToClearClipboard(false)
{
    m_timer = new QTimer(this);
    m_timer->setSingleShot(true);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(clear()));
}

void SecureClipboard::clear()
{
    if (m_needToClearClipboard) {
        m_needToClearClipboard = false;
        QApplication::clipboard()->clear();
        m_timer->stop();
    }
}

void SecureClipboard::clearLater()
{
    m_timer->start(50);
}

void SecureClipboard::setText(const QString& text)
{
    m_timer->stop();
    QApplication::clipboard()->setText(text);
    m_needToClearClipboard = true;
    m_timer->start(5000);
}
