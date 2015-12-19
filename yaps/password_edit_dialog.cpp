#include "password_edit_dialog.h"

#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "crypto.h"
#include "message_box_factory.h"
#include "password_record.h"
#include "secure_clipboard.h"

namespace yaps {

PasswordEditDialog::PasswordEditDialog(const QString& title,
                                       std::shared_ptr<PasswordRecord> record,
                                       std::unique_ptr<Crypto> crypto,
                                       std::shared_ptr<SecureClipboard> clipboard,
                                       std::shared_ptr<MessageBoxFactory> messageBoxFactory)
    : record_(std::move(record)),
      crypto_(std::move(crypto)),
      clipboard_(std::move(clipboard)),
      messageBoxFactory_(std::move(messageBoxFactory)) {
  if (!record_) {
    throw std::logic_error("PasswordRecord is null");
  }
  if (!crypto_) {
    throw std::logic_error("Crypto pointer is null");
  }
  if (!clipboard_) {
    throw std::logic_error("Clipboard is null");
  }
  nameEdit_ = new QLineEdit;
  loginEdit_ = new QLineEdit;
  passwordEdit1_ = new QLineEdit;
  passwordEdit1_->setEchoMode(QLineEdit::Password);
  passwordEdit2_ = new QLineEdit;
  passwordEdit2_->setEchoMode(QLineEdit::Password);
  initControls();

  auto okButton = new QPushButton(tr("OK"));
  auto cancelButton = new QPushButton(tr("Cancel"));
  auto generateButton = new QPushButton(tr("Generate"));
  auto copyButton = new QPushButton(tr("Copy all"));
  okButton->setDefault(true);
  connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
  connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
  connect(generateButton, SIGNAL(clicked()), this, SLOT(generatePassword()));
  connect(copyButton, SIGNAL(clicked()), this, SLOT(copyAll()));

  // layout
  auto formLayout = new QFormLayout;
  formLayout->setWidget(0, QFormLayout::LabelRole, new QLabel(tr("Name")));
  formLayout->setWidget(0, QFormLayout::FieldRole, nameEdit_);
  formLayout->setWidget(1, QFormLayout::LabelRole, new QLabel(tr("Login")));
  formLayout->setWidget(1, QFormLayout::FieldRole, loginEdit_);
  formLayout->setWidget(2, QFormLayout::LabelRole, new QLabel(tr("Password")));
  formLayout->setWidget(2, QFormLayout::FieldRole, passwordEdit1_);
  formLayout->setWidget(3, QFormLayout::LabelRole, new QLabel(tr("Confirm")));
  formLayout->setWidget(3, QFormLayout::FieldRole, passwordEdit2_);
  auto buttonLayout = new QHBoxLayout;
  buttonLayout->addStretch();
  buttonLayout->addWidget(generateButton);
  buttonLayout->addWidget(copyButton);
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(cancelButton);
  auto mainLayout = new QVBoxLayout;
  mainLayout->addLayout(formLayout);
  mainLayout->addLayout(buttonLayout);

  setLayout(mainLayout);
  setFixedHeight(sizeHint().height());
  setWindowTitle(title);
  setWindowFlags(Qt::WindowMinimizeButtonHint |
                 Qt::WindowCloseButtonHint |
                 Qt::WindowStaysOnTopHint);
}

PasswordEditDialog::~PasswordEditDialog() {}

void PasswordEditDialog::initControls() {
  if (!record_->name().isEmpty()) {
    nameEdit_->setText(record_->name());
    nameEdit_->setReadOnly(true);
  }

  QString decrypted;
  crypto_->decrypt(record_->password(), decrypted);
  int loginLength = decrypted.indexOf('\n');
  if (loginLength > 0) {
    loginEdit_->setText(decrypted.left(loginLength));
  }
  crypto_->eraseString(decrypted);
}

void PasswordEditDialog::accept() {
  // validate user input
  if (nameEdit_->text().isEmpty()) {
    messageBoxFactory_->showWarning(tr("Error"), tr("Name should not be empty."));
    return;
  }
  if (passwordEdit1_->text().isEmpty()) {
    messageBoxFactory_->showWarning(tr("Error"), tr("Password should not be empty."));
    return;
  }
  if (passwordEdit1_->text() != passwordEdit2_->text()) {
    messageBoxFactory_->showWarning(tr("Error"), tr("Passwords do not match."));
    return;
  }

  // encrypt data and close the dialog
  saveRecord();
  QDialog::accept();
}

void PasswordEditDialog::saveRecord() {
  // fill in PasswordRecord
  if (record_->name().isEmpty()) {
    record_->setName(nameEdit_->text());
  }
  QString encryptedPassword;
  if (loginEdit_->text().isEmpty()) {
    encryptedPassword = passwordEdit1_->text();
  } else {
    encryptedPassword = loginEdit_->text() + "\n" + passwordEdit1_->text();
  }
  crypto_->encrypt(encryptedPassword);
  record_->setPassword(encryptedPassword);

  // erase data in GUI controls
  QString eraser;
  eraser.fill('?', loginEdit_->text().size());
  loginEdit_->setText(eraser);
  eraser.fill('?', passwordEdit1_->text().size());
  passwordEdit1_->setText(eraser);
  passwordEdit2_->setText(eraser);
}

void PasswordEditDialog::generatePassword() {
  QString password;
  crypto_->generatePassword(password);
  passwordEdit1_->setText(password);
  passwordEdit2_->setText(password);
  crypto_->eraseString(password);
}

void PasswordEditDialog::copyAll() {
  QString login = loginEdit_->text();
  QString password1 = passwordEdit1_->text();
  QString password2 = passwordEdit2_->text();

  clipboard_->setContent({login, password1, password2});

  crypto_->eraseString(login);
  crypto_->eraseString(password1);
  crypto_->eraseString(password2);
}

}  // namespace yaps
