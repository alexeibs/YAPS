#pragma once

#include "crypto_engine.h"
#include "expimp.h"

namespace yaps {

struct YAPSLIB_API CryptoEngineImpl : CryptoEngine {
  void encrypt(const QString& password, const QString& input, QString& output) override;
  void decrypt(const QString& password, const QString& input, QString& output) override;
  void generatePassword(QString& password) override;
  void eraseString(QString& stringToErase) override;
};

}  // namespace yaps
