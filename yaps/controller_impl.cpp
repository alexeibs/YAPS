#include "controller_impl.h"

#include <QMessageBox>

#include "crypto.h"
#include "passwords_model.h"
#include "view_state.h"

#include "PasswordEditDialog.h"
#include "SecureClipboard.h"

namespace yaps {

ControllerImpl::ControllerImpl(std::shared_ptr<CryptoFactory> cryptoFactory,
                               std::shared_ptr<PasswordsModel> passwordsModel)
    : cryptoFactory_(move(cryptoFactory)),
      passwordsModel_(move(passwordsModel)) {
}

ControllerImpl::~ControllerImpl() {}

void ControllerImpl::setViewState(ViewState* viewState) {
  viewState_ = viewState;
}

ViewState& ControllerImpl::viewState() {
  if (!viewState_) {
    throw std::logic_error("View state is null");
  }
  return *viewState_;
}

const ViewState& ControllerImpl::viewState() const {
  if (!viewState_) {
    throw std::logic_error("View state is null");
  }
  return *viewState_;
}

template<typename Callback>
void tryTo(Callback&& callback) {
  try {
    callback();
  } catch (std::exception& exception) {
    QMessageBox::critical(nullptr, QObject::tr("Error"), exception.what());
  }
}

template<typename Callback>
void ControllerImpl::copyToClipboardImpl(Callback&& callback) {
  tryTo([this, &callback]() {
    int recordIndex = viewState().currentRecordIndex();
    PasswordRecord record;
    passwordsModel_->getRecord(recordIndex, record);

    auto crypto = cryptoFactory_->getCrypto();
    if (!crypto)
      return;

    QString decrypted;
    crypto->decrypt(record.password, decrypted);

    callback(decrypted, *crypto);

    crypto->eraseString(decrypted);
    viewState().toggleMainWindow();
  });
}

void ControllerImpl::copyToClipboard() {
  copyToClipboardImpl([](const QString& decrypted, Crypto& /*crypto*/) {
    SecureClipboard::instance().setContent(decrypted);
  });
}

void ControllerImpl::copyPasswordToClipboard() {
  copyToClipboardImpl([](const QString& decrypted, Crypto& crypto) {
    QString password = decrypted.mid(decrypted.lastIndexOf('\n') + 1);
    SecureClipboard::instance().setContent(password);
    crypto.eraseString(password);
  });
}

void yaps::ControllerImpl::copyNextItemToClipboard() {
  SecureClipboard::instance().contentPasted();
}

void ControllerImpl::addPassword() {
  tryTo([this]() {
    auto crypto = cryptoFactory_->getCrypto();
    if (!crypto)
      return;

    PasswordRecord record;
    PasswordEditDialog dialog(QObject::tr("New password"), move(crypto));
    dialog.setPasswordRecord(record);
    if (dialog.exec() == QDialog::Rejected)
      return;

    record = dialog.passwordRecord();
    if (passwordsModel_->hasRecord(record.name)) {
      auto question = QObject::tr("Password \"%1\" already exists. Overwrite it?").arg(record.name);
      auto userAnswer = QMessageBox::question(nullptr, QObject::tr("Confirm"), question);
      if (userAnswer != QMessageBox::Yes)
        return;
    }
    passwordsModel_->addOrSetRecord(record);
    viewState().setCurrentRecordIndex(passwordsModel_->getIndexByName(record.name));
  });
}

void ControllerImpl::editPassword() {
  tryTo([this]() {
    int recordIndex = viewState().currentRecordIndex();
    PasswordRecord record;
    passwordsModel_->getRecord(recordIndex, record);

    auto crypto = cryptoFactory_->getCrypto();
    if (!crypto)
      return;

    PasswordEditDialog dialog(QObject::tr("Edit password"), move(crypto));
    dialog.setPasswordRecord(record);
    dialog.setNameReadOnly(true);
    if (dialog.exec() == QDialog::Rejected)
      return;

    record = dialog.passwordRecord();
    passwordsModel_->addOrSetRecord(record);
    viewState().setCurrentRecordIndex(passwordsModel_->getIndexByName(record.name));
  });
}

void ControllerImpl::deletePassword() {
  tryTo([this]() {
    int recordIndex = viewState().currentRecordIndex();
    passwordsModel_->removeRecord(recordIndex);
    viewState().setCurrentRecordIndex(passwordsModel_->fixIndex(recordIndex));
  });
}

void ControllerImpl::clearMasterPassword() {
  cryptoFactory_->clearMasterPassword();
}

}  // namespace yaps
