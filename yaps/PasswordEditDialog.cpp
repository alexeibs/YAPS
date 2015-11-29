#include "PasswordEditDialog.h"

#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

#include "crypto.h"
#include "passwords_model.h"
#include "secure_clipboard.h"

using yaps::PasswordRecord;

PasswordEditDialog::PasswordEditDialog(const QString& title,
                                       std::unique_ptr<yaps::Crypto> crypto,
                                       std::shared_ptr<yaps::SecureClipboard> clipboard)
{
    if (!crypto) {
        throw std::logic_error("Crypto pointer is null");
    }
    if (!clipboard) {
        throw std::logic_error("Clipboard is null");
    }
    m_crypto = std::move(crypto);
    m_clipboard = std::move(clipboard);
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

PasswordEditDialog::~PasswordEditDialog()
{
}

void PasswordEditDialog::setPasswordRecord(const PasswordRecord& record)
{
    m_record = record;
    m_name->setText(m_record.name);

    QString decrypted;
    m_crypto->decrypt(record.password, decrypted);
    int loginLength = decrypted.indexOf('\n');
    if (loginLength > 0)
        m_login->setText(decrypted.left(loginLength));
    m_crypto->eraseString(decrypted);
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
    m_crypto->encrypt(m_record.password);

    return true;
}

void PasswordEditDialog::generatePassword()
{
    QString password;
    m_crypto->generatePassword(password);
    m_password->setText(password);
    m_password2->setText(password);
    m_crypto->eraseString(password);
}

void PasswordEditDialog::copyAll()
{
    QString login = m_login->text();
    QString password1 = m_password->text();
    QString password2 = m_password2->text();

    m_clipboard->setContent({login, password1, password2});

    m_crypto->eraseString(login);
    m_crypto->eraseString(password1);
    m_crypto->eraseString(password2);
}
