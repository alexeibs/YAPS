#ifndef CRYPTO_H
#define CRYPTO_H

#include <QObject>

class Crypto : public QObject { Q_OBJECT
public:
    static Crypto& instance();

    bool getGlobalPassword();
    void encrypt(QString& text);
    void decrypt(const QString& input, QString& output);
    void erase(QString& stringToErase);

private:
    Crypto();
    Crypto(const Crypto&) = delete;
    Crypto(Crypto&&) = delete;
    void operator=(const Crypto&) = delete;
    void operator=(Crypto&&) = delete;

private:
    QString m_globalPassword;
};

#endif // CRYPTO_H
