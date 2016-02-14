#pragma once

#include "expimp.h"

class QString;

namespace yaps {

struct YAPSLIB_API CryptoEngine {
    virtual ~CryptoEngine() {}
    virtual void encrypt(const QString& password, const QString& input, QString& output) = 0;
    virtual void decrypt(const QString& password, const QString& input, QString& output) = 0;
    virtual void generatePassword(QString& password) = 0;
};

YAPSLIB_API void eraseString(QString& stringToErase);

}  // namespace yaps
