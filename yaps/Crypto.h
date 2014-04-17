#ifndef CRYPTO_H
#define CRYPTO_H

#include <memory>
#include <QObject>

class QTimer;

class Crypto : public QObject { Q_OBJECT
public:
    typedef std::unique_ptr<Crypto, void(*)(Crypto*)> CryptoPointer;

    static CryptoPointer instance();
    static void erase(QString& stringToErase);
    static void clearGlobalPassword();

    void encrypt(QString& text);
    void decrypt(const QString& input, QString& output);
    void generatePassword(QString& password, int length = 4); // length will be multiplied by 4

    void lock();
    void unlock();
    static void unlockCrypto(Crypto* object);

signals:
    void globalPasswordRefreshed();
    void globalPasswordExpired();

public slots:
    void clear();

private:
    Crypto();
    Crypto(const Crypto&) = delete;
    Crypto(Crypto&&) = delete;
    void operator=(const Crypto&) = delete;
    void operator=(Crypto&&) = delete;
    static Crypto& privateInstance();

    bool refreshPassword();

private:
    QString m_globalPassword;
    QTimer* m_clearTimer = nullptr;
    bool m_locked = false;
};

#endif // CRYPTO_H
