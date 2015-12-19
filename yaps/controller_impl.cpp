#include "controller_impl.h"

#include <QMessageBox>

#include "crypto.h"
#include "passwords_model.h"
#include "password_record.h"
#include "password_record_editor.h"
#include "secure_clipboard.h"
#include "view_state.h"

namespace yaps {

ControllerImpl::ControllerImpl(std::shared_ptr<CryptoFactory> cryptoFactory,
                               std::shared_ptr<PasswordsModel> passwordsModel,
                               std::shared_ptr<SecureClipboard> clipboard,
                               std::shared_ptr<PasswordRecordEditor> recordEditor)
    : cryptoFactory_(move(cryptoFactory)),
      passwordsModel_(move(passwordsModel)),
      clipboard_(move(clipboard)),
      passwordRecordEditor_(move(recordEditor)){
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
    auto record = passwordsModel_->getRecord(recordIndex);

    auto crypto = cryptoFactory_->getCrypto();
    if (!crypto) {
      return;
    }
    QString decrypted;
    crypto->decrypt(record->password(), decrypted);

    callback(decrypted, *crypto);

    crypto->eraseString(decrypted);
    viewState().toggleMainWindow();
  });
}

void ControllerImpl::copyToClipboard() {
  copyToClipboardImpl([this](const QString& decrypted, Crypto& /*crypto*/) {
    clipboard_->setContent(decrypted.split('\n'));
  });
}

void ControllerImpl::copyPasswordToClipboard() {
  copyToClipboardImpl([this](const QString& decrypted, Crypto& crypto) {
    QString password = decrypted.mid(decrypted.lastIndexOf('\n') + 1);
    clipboard_->setContent({password});
    crypto.eraseString(password);
  });
}

void yaps::ControllerImpl::copyNextItemToClipboard() {
  clipboard_->copyNextItem();
}

void ControllerImpl::addPassword() {
  tryTo([this]() {
    auto crypto = cryptoFactory_->getCrypto();
    if (!crypto) {
      return;
    }
    auto record = std::make_shared<PasswordRecord>();
    if (!passwordRecordEditor_->edit(record, move(crypto), QObject::tr("New password"))) {
      return;
    }
    if (passwordsModel_->hasRecord(record->name())) {
      auto question = QObject::tr("Password \"%1\" already exists. Overwrite it?")
          .arg(record->name());
      auto userAnswer = QMessageBox::question(nullptr, QObject::tr("Confirm"), question);
      if (userAnswer != QMessageBox::Yes) {
        return;
      }
    }
    passwordsModel_->addOrSetRecord(*record);
    viewState().setCurrentRecordIndex(passwordsModel_->getIndexByName(record->name()));
  });
}

void ControllerImpl::editPassword() {
  tryTo([this]() {
    int recordIndex = viewState().currentRecordIndex();
    auto record = passwordsModel_->getRecord(recordIndex);

    auto crypto = cryptoFactory_->getCrypto();
    if (!crypto) {
      return;
    }
    if (!passwordRecordEditor_->edit(record, move(crypto), QObject::tr("Edit password"))) {
      return;
    }
    passwordsModel_->addOrSetRecord(*record);
    viewState().setCurrentRecordIndex(passwordsModel_->getIndexByName(record->name()));
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
