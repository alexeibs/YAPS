#ifndef SECURECLIPBOARD_H
#define SECURECLIPBOARD_H

#include <QObject>

class QTimer;

class SecureClipboard : public QObject { Q_OBJECT
public:
    static SecureClipboard& instance();

public slots:
    void clear();
    void clearLater();
    void setText(const QString& text);

private:
    SecureClipboard();
    SecureClipboard(const SecureClipboard&) = delete;
    SecureClipboard(SecureClipboard&&) = delete;
    void operator=(const SecureClipboard&) = delete;
    void operator=(SecureClipboard&&) = delete;

private:
    QTimer* m_timer;
    bool m_needToClearClipboard;
};

#endif // SECURECLIPBOARD_H
