#include "global_password_dialog.h"

#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

namespace yaps {

GlobalPasswordDialog::GlobalPasswordDialog(QString& password) : password_(password) {
  passwordEdit_ = new QLineEdit;
  passwordEdit_->setEchoMode(QLineEdit::Password);

  auto okButton = new QPushButton(tr("OK"));
  auto cancelButton = new QPushButton(tr("Cancel"));
  okButton->setDefault(true);

  connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
  connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
  connect(this, SIGNAL(accepted()), this, SLOT(commit()));

  auto buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(cancelButton);
  auto mainLayout = new QVBoxLayout;
  mainLayout->addWidget(passwordEdit_);
  mainLayout->addLayout(buttonLayout);

  setLayout(mainLayout);
  setWindowTitle(tr("Enter password"));
  setWindowFlags(Qt::WindowMinimizeButtonHint |
                 Qt::WindowCloseButtonHint |
                 Qt::WindowStaysOnTopHint);
  setFixedSize(sizeHint());
}

void GlobalPasswordDialog::accept() {
  if (!passwordEdit_->text().isEmpty()) {
    QDialog::accept();
  }
}

void GlobalPasswordDialog::commit() {
  password_ = passwordEdit_->text();
  QString eraser;
  eraser.fill('?', password_.size());
  passwordEdit_->setText(eraser);
}

}  // namespace yaps
