#include "PasswordEditDialog.h"

#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

#include "crypto.h"
#include "crypto_engine.h"
#include "PasswordsModel.h"
#include "SecureClipboard.h"

PasswordEditDialog::PasswordEditDialog(const QString& title,
                                       std::shared_ptr<yaps::CryptoFactory> cryptoFactory)
{
    m_cryptoFactory = std::move(cryptoFactory);
    m_name = new QLineEdit;
    m_login = new QLineEdit;
    m_password = new QLineEdit;
    m_password->setEchoMode(QLineEdit::Password);
    m_password2 = new QLineEdit;
    m_password2->setEchoMode(QLineEdit::Password);

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
    formLayout->setWidget(0, QFormLayout::FieldRole, m_name);
    formLayout->setWidget(1, QFormLayout::LabelRole, new QLabel(tr("Login")));
    formLayout->setWidget(1, QFormLayout::FieldRole, m_login);
    formLayout->setWidget(2, QFormLayout::LabelRole, new QLabel(tr("Password")));
    formLayout->setWidget(2, QFormLayout::FieldRole, m_password);
    formLayout->setWidget(3, QFormLayout::LabelRole, new QLabel(tr("Confirm")));
    formLayout->setWidget(3, QFormLayout::FieldRole, m_password2);
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
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint);
}

bool PasswordEditDialog::setPasswordRecord(const PasswordRecord& record)
{
    auto crypto = m_cryptoFactory->getCrypto();
    if (!crypto)
        return false;

    m_record = record;
    m_name->setText(m_record.name);

    QString decrypted;
    crypto->decrypt(record.password, decrypted);
    int loginLength = decrypted.indexOf('\n');
    if (loginLength > 0)
        m_login->setText(decrypted.left(loginLength));
    yaps::eraseString(decrypted);

    return true;
}

void PasswordEditDialog::setNameReadOnly(bool readOnly)
{
    m_name->setReadOnly(readOnly);
}

void PasswordEditDialog::accept()
{
    // validate user input
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

    // encrypt data and close the dialog
    if (commit())
        QDialog::accept();
    else
        QDialog::reject();
}

bool PasswordEditDialog::commit()
{
    auto crypto = m_cryptoFactory->getCrypto();
    if (!crypto)
        return false;

    // fill in PasswordRecord
    m_record.name = m_name->text();
    if (m_login->text().isEmpty())
        m_record.password = m_password->text();
    else
        m_record.password = m_login->text() + "\n" + m_password->text();

    // erase data in GUI controls
    QString eraser;
    eraser.fill('?', m_login->text().size());
    m_login->setText(eraser);
    eraser.fill('?', m_password->text().size());
    m_password->setText(eraser);
    m_password2->setText(eraser);

    // encrypt PasswordRecord
    crypto->encrypt(m_record.password);

    return true;
}

void PasswordEditDialog::generatePassword()
{
    auto crypto = m_cryptoFactory->getCrypto();
    if (!crypto)
        return;

    QString password;
    crypto->generatePassword(password);
    m_password->setText(password);
    m_password2->setText(password);
    yaps::eraseString(password);
}

void PasswordEditDialog::copyAll()
{
    QString login = m_login->text();
    QString password1 = m_password->text();
    QString password2 = m_password2->text();
    QString all = login + "\n" + password1 + "\n" + password2;

    SecureClipboard::instance().setContent(all);\

    yaps::eraseString(login);
    yaps::eraseString(password1);
    yaps::eraseString(password2);
    yaps::eraseString(all);
}
