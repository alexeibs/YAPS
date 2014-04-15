#include "SecureClipboard.h"

#include <QTimer>
#include <QApplication>
#include <QClipboard>

#include "Crypto.h"

#define CLIPBOARD_TIMEOUT 5000

SecureClipboard& SecureClipboard::instance()
{
    static SecureClipboard single;
    return single;
}

SecureClipboard::SecureClipboard()
    : m_needToClearClipboard(false)
{
    m_clearTimer = new QTimer(this);
    m_clearTimer->setSingleShot(true);
    m_nextTimer = new QTimer(this);
    m_nextTimer->setSingleShot(true);
    connect(m_clearTimer, SIGNAL(timeout()), this, SLOT(clear()));
    connect(m_nextTimer, SIGNAL(timeout()), this, SLOT(nextItem()));
}

void SecureClipboard::setContent(const QString& content)
{
    m_clearTimer->stop();
    m_nextTimer->stop();
    m_content = content.split('\n');
    nextItem();
}

void SecureClipboard::clear()
{
    if (m_needToClearClipboard) {
        m_needToClearClipboard = false;
        QApplication::clipboard()->clear();
        m_clearTimer->stop();
        for (auto it = m_content.begin(), itEnd = m_content.end(); it != itEnd; ++it)
            Crypto::erase(*it);
        m_content.clear();
    }
}

void SecureClipboard::contentPasted()
{
    m_clearTimer->stop();
    m_nextTimer->start(50);
}

void SecureClipboard::nextItem()
{
    if (m_content.empty()) {
        clear();
    } else {
        m_needToClearClipboard = true;
        QApplication::clipboard()->setText(m_content.front());
        Crypto::erase(m_content.first());
        m_content.removeFirst();
        m_clearTimer->start(5000);
    }
}
