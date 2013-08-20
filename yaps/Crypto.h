#ifndef CRYPTO_H
#define CRYPTO_H

#include <QObject>

class Crypto : public QObject { Q_OBJECT
public:
    static Crypto& instance();

    void encrypt(QString& input, QString& output);
    void decrypt(const QString& input, QString& output);
    void erase(QString& stringToErase);

private:
    Crypto();
    Crypto(const Crypto&) = delete;
    Crypto(Crypto&&) = delete;
    void operator=(const Crypto&) = delete;
    void operator=(Crypto&&) = delete;
};

#endif // CRYPTO_H
