#ifndef SECURECLIPBOARD_H
#define SECURECLIPBOARD_H

#include <QObject>
#include <QStringList>

class QTimer;

class SecureClipboard : public QObject { Q_OBJECT
public:
    static SecureClipboard& instance();

    void setContent(const QString& content);
    void contentPasted();

public slots:
    void clear();

private slots:
    void nextItem();

private:
    SecureClipboard();
    SecureClipboard(const SecureClipboard&) = delete;
    SecureClipboard(SecureClipboard&&) = delete;
    void operator=(const SecureClipboard&) = delete;
    void operator=(SecureClipboard&&) = delete;

private:
    QStringList m_content;
    QTimer* m_clearTimer;
    QTimer* m_nextTimer;
    bool m_needToClearClipboard;
};

#endif // SECURECLIPBOARD_H
