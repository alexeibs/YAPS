#pragma once

class QString;

namespace yaps {

struct CryptoEngine {
    virtual ~CryptoEngine() {}
    virtual void encrypt(const QString& password, const QString& input, QString& output) = 0;
    virtual void decrypt(const QString& password, const QString& input, QString& output) = 0;
    virtual void generatePassword(QString& password) = 0;
};

void eraseString(QString& stringToErase);

}  // namespace yaps
