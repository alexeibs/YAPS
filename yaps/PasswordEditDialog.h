#ifndef PASSWORDEDITDIALOG_H
#define PASSWORDEDITDIALOG_H

#include <memory>

#include <QDialog>

#include "passwords_model.h"

class QLineEdit;

namespace yaps {
    struct Crypto;
    struct SecureClipboard;
}

class PasswordEditDialog : public QDialog { Q_OBJECT
public:
    PasswordEditDialog(const QString& title,
                       std::unique_ptr<yaps::Crypto>,
                       std::shared_ptr<yaps::SecureClipboard>);
    ~PasswordEditDialog();
    void setNameReadOnly(bool readOnly);

    void setPasswordRecord(const yaps::PasswordRecord&);
    yaps::PasswordRecord passwordRecord() const { return m_record; }

public slots:
    void accept() override;

private slots:
    void generatePassword();
    void copyAll();

private:
    bool commit();

private:
    yaps::PasswordRecord m_record;
    QLineEdit* m_name;
    QLineEdit* m_login;
    QLineEdit* m_password;
    QLineEdit* m_password2;

    std::unique_ptr<yaps::Crypto> m_crypto;
    std::shared_ptr<yaps::SecureClipboard> m_clipboard;
};

#endif // PASSWORDEDITDIALOG_H
