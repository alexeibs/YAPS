#pragma once

#include <memory>

#include <QString>

#include "crypto.h"
#include "crypto_status.h"
#include "expimp.h"
#include "password_lock.h"

namespace yaps {

struct CryptoEngine;
struct PasswordPrompt;
struct Timer;

struct YAPSLIB_API CryptoFactoryImpl : std::enable_shared_from_this<CryptoFactoryImpl>,
                           CryptoFactory,
                           CryptoStatus,
                           PasswordLock {
  CryptoFactoryImpl(std::shared_ptr<CryptoEngine>,
                    std::shared_ptr<PasswordPrompt>,
                    std::shared_ptr<Timer> expirationTimer);
  ~CryptoFactoryImpl();

  void setCryptoStatusView(CryptoStatusView*);

  // CryptoFactory interface
  std::unique_ptr<Crypto> getCrypto() override;
  void clearMasterPassword() override;

  // CryptoStatus interface
  bool hasMasterPassword() const override;

  // PasswordLock interface
  const QString& lockPassword() override;
  void unlockPassword() override;

private:
  void triggerStatusViewUpdate();

  std::shared_ptr<CryptoEngine> cryptoEngine_;
  std::shared_ptr<PasswordPrompt> passwordPrompt_;
  std::shared_ptr<Timer> expirationTimer_;
  CryptoStatusView* statusView_ = nullptr;
  QString masterPassword_;
  bool passwordLocked_ = false;
};

}  // namespace yaps
