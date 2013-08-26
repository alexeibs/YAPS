#include "PasswordEditDialog.h"

#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include "PasswordsModel.h"
#include "Crypto.h"

PasswordEditDialog::PasswordEditDialog(const QString& title, PasswordRecord& record)
    : m_record(record)
{
    m_name = new QLineEdit;
    m_name->setText(m_record.name);

    m_password = new QLineEdit;
    m_password->setEchoMode(QLineEdit::Password);
    m_password2 = new QLineEdit;
    m_password2->setEchoMode(QLineEdit::Password);

    auto okButton = new QPushButton(tr("OK"));
    auto cancelButton = new QPushButton(tr("Cancel"));
    auto generateButton = new QPushButton(tr("Generate"));
    okButton->setDefault(true);
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(this, SIGNAL(accepted()), this, SLOT(commit()));
    connect(generateButton, SIGNAL(clicked()), this, SLOT(generatePassword()));

    // layout
    auto formLayout = new QFormLayout;
    formLayout->setWidget(0, QFormLayout::LabelRole, new QLabel(tr("Name")));
    formLayout->setWidget(0, QFormLayout::FieldRole, m_name);
    formLayout->setWidget(1, QFormLayout::LabelRole, new QLabel(tr("Password")));
    formLayout->setWidget(1, QFormLayout::FieldRole, m_password);
    formLayout->setWidget(2, QFormLayout::LabelRole, new QLabel(tr("Confirm")));
    formLayout->setWidget(2, QFormLayout::FieldRole, m_password2);
    auto buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(generateButton);
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    auto mainLayout = new QVBoxLayout;
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
    setFixedHeight(sizeHint().height());
    setWindowTitle(title);
    setWindowFlags(Qt::WindowCloseButtonHint);
}

void PasswordEditDialog::setNameReadOnly(bool readOnly)
{
    m_name->setReadOnly(readOnly);
}

void PasswordEditDialog::accept()
{
    if (m_name->text().isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("Name should not be empty."));
        return;
    }
    if (m_password->text().isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("Password should not be empty."));
        return;
    }
    if (m_password->text() != m_password2->text()) {
        QMessageBox::warning(this, tr("Error"), tr("Passwords do not match."));
        return;
    }
    QDialog::accept();
}

void PasswordEditDialog::commit()
{
    m_record.name = m_name->text();
    m_record.password = m_password->text();
    QString eraser;
    eraser.fill('?', m_record.password.size());
    m_password->setText(eraser);
    m_password2->setText(eraser);
    Crypto::instance().encrypt(m_record.password);
}

void PasswordEditDialog::generatePassword()
{
    QString password;
    auto& crypto = Crypto::instance();
    crypto.generatePassword(password);
    m_password->setText(password);
    m_password2->setText(password);
    crypto.erase(password);
    accept();
}
