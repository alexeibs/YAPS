#pragma once

#include <memory>

#include "crypto.h"
#include "expimp.h"

namespace yaps {

struct CryptoEngine;
struct PasswordLock;

struct YAPSLIB_API CryptoImpl : Crypto {
  CryptoImpl(std::shared_ptr<CryptoEngine> engine, std::shared_ptr<PasswordLock> passwordLock);
  ~CryptoImpl();

  void encrypt(QString& text) override;
  void decrypt(const QString& input, QString& output) override;
  void generatePassword(QString& password) override;
  void eraseString(QString &stringToErase) override;

private:
  std::shared_ptr<CryptoEngine> cryptoEngine_;
  std::shared_ptr<PasswordLock> passwordLock_;
  const QString& masterPassword_;
};

}  // namespace yaps
