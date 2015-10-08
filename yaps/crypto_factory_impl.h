#pragma once

#include <memory>

#include <QString>

#include "crypto.h"
#include "crypto_engine.h"
#include "crypto_status.h"
#include "password_lock.h"
#include "password_prompt.h"
#include "timer.h"

namespace yaps {

struct CryptoFactoryImpl : std::enable_shared_from_this<CryptoFactoryImpl>,
                           CryptoFactory,
                           CryptoStatus,
                           PasswordLock {
  CryptoFactoryImpl(std::shared_ptr<CryptoEngine>,
                    std::shared_ptr<PasswordPrompt>,
                    std::shared_ptr<Timer> expirationTimer,
                    std::shared_ptr<CryptoStatusView>);
  ~CryptoFactoryImpl();

  // CryptoFactory interface
  std::unique_ptr<Crypto> getCrypto() override;

  // CryptoStatus interface
  void clearPassword() override;
  bool hasPassword() const override;

  // PasswordLock interface
  const QString& lockPassword() override;
  void unlockPassword() override;

private:
  std::shared_ptr<CryptoEngine> cryptoEngine_;
  std::shared_ptr<PasswordPrompt> passwordPrompt_;
  std::shared_ptr<Timer> expirationTimer_;
  std::shared_ptr<CryptoStatusView> statusView_;
  QString masterPassword_;
  bool passwordLocked_ = false;
};

}  // namespace yaps