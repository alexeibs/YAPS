#include "crypto_impl.h"

#include "crypto_engine.h"
#include "password_lock.h"

namespace yaps {

CryptoImpl::CryptoImpl(std::shared_ptr<CryptoEngine> engine,
                       std::shared_ptr<PasswordLock> passwordLock)
  : cryptoEngine_(move(engine)),
    passwordLock_(move(passwordLock)),
    masterPassword_(passwordLock_->lockPassword()) {
}

CryptoImpl::~CryptoImpl() {
  passwordLock_->unlockPassword();
}

void CryptoImpl::encrypt(QString& text) {
  cryptoEngine_->encrypt(masterPassword_, text, text);
}

void CryptoImpl::decrypt(const QString& input, QString& output) {
  cryptoEngine_->decrypt(masterPassword_, input, output);
}

void CryptoImpl::generatePassword(QString& password) {
  cryptoEngine_->generatePassword(password);
}

void CryptoImpl::eraseString(QString& stringToErase)
{
  yaps::eraseString(stringToErase);
}

}  // namespace yaps
