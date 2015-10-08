#include "crypto_factory_impl.h"

#include "crypto_impl.h"

namespace yaps {

const int EXPIRATION_INTERVAL = 300000;
const int RESCHEDULE_INTERVAL = 100;

CryptoFactoryImpl::CryptoFactoryImpl(std::shared_ptr<CryptoEngine> engine,
                                     std::shared_ptr<PasswordPrompt> prompt,
                                     std::shared_ptr<Timer> expirationTimer,
                                     std::shared_ptr<CryptoStatusView> statusView)
    : cryptoEngine_(move(engine)),
      passwordPrompt_(move(prompt)),
      expirationTimer_(move(expirationTimer)),
      statusView_(move(statusView)) {

  expirationTimer_->setCallback([this]() {
    clearPassword();
  });
}

CryptoFactoryImpl::~CryptoFactoryImpl() {
  expirationTimer_->stop();
  eraseString(masterPassword_);
}

std::unique_ptr<Crypto> CryptoFactoryImpl::getCrypto() {
  if (masterPassword_.isEmpty()) {
    passwordPrompt_->getPassword(masterPassword_);
    if (masterPassword_.isEmpty()) {
      return {};
    }
  }

  expirationTimer_->start(EXPIRATION_INTERVAL);
  statusView_->updateCryptoStatus();

  // gcc 4.8 doesn't support std::make_unique
  return std::unique_ptr<Crypto>{new CryptoImpl(cryptoEngine_, shared_from_this())};
}

void CryptoFactoryImpl::clearPassword() {
  if (passwordLocked_) {
    expirationTimer_->start(RESCHEDULE_INTERVAL);
  } else if (!masterPassword_.isEmpty()) {
    expirationTimer_->stop();
    eraseString(masterPassword_);
    statusView_->updateCryptoStatus();
  }
}

bool CryptoFactoryImpl::hasPassword() const {
  return !masterPassword_.isEmpty();
}

const QString& CryptoFactoryImpl::lockPassword() {
  if (passwordLocked_) {
    throw std::logic_error("The master password has already been locked.");
  }
  passwordLocked_ = true;
  return masterPassword_;
}

void CryptoFactoryImpl::unlockPassword() {
  if (!passwordLocked_) {
    throw std::logic_error("The master password has not been locked yet.");
  }
  passwordLocked_ = false;
}

}  // namesapce yaps
