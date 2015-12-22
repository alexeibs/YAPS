#pragma once

#include "crypto_engine.h"

namespace yaps {

struct CryptoEngineImpl : CryptoEngine {
  void encrypt(const QString& password, const QString& input, QString& output);
  void decrypt(const QString& password, const QString& input, QString& output);
  void generatePassword(QString& password);
};

}  // namespace yaps
