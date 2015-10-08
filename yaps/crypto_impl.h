#pragma once

#include <memory>

#include "crypto.h"
#include "crypto_engine.h"
#include "password_lock.h"

namespace yaps {

struct CryptoImpl : Crypto {
  CryptoImpl(std::shared_ptr<CryptoEngine> engine, std::shared_ptr<PasswordLock> passwordLock);
  ~CryptoImpl();

  void encrypt(QString& text) override;
  void decrypt(const QString& input, QString& output) override;
  void generatePassword(QString& password) override;

private:
  std::shared_ptr<CryptoEngine> cryptoEngine_;
  std::shared_ptr<PasswordLock> passwordLock_;
  const QString& masterPassword_;
};

}  // namespace yaps
