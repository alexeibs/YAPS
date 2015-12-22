#pragma once

#include <memory>

class QString;

namespace yaps {

struct Crypto {
  virtual ~Crypto() {}
  virtual void encrypt(QString& text) = 0;
  virtual void decrypt(const QString& input, QString& output) = 0;
  virtual void generatePassword(QString& password) = 0;
  virtual void eraseString(QString& stringToErase) = 0;
};

struct CryptoFactory {
  virtual ~CryptoFactory() {}

  // returns null if the master password is not available
  virtual std::unique_ptr<Crypto> getCrypto() = 0;

  virtual void clearMasterPassword() = 0;
};

}  // namespace yaps
