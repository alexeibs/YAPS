#ifndef PASSWORDEDITDIALOG_H
#define PASSWORDEDITDIALOG_H

#include <memory>

#include <QDialog>

#include "PasswordsModel.h"

class QLineEdit;
struct PasswordRecord;

namespace yaps {
    struct CryptoFactory;
}

class PasswordEditDialog : public QDialog { Q_OBJECT
public:
    PasswordEditDialog(const QString& title, std::shared_ptr<yaps::CryptoFactory>);
    void setNameReadOnly(bool readOnly);

    // returns false if Crypto API is inaccessible
    bool setPasswordRecord(const PasswordRecord&);
    PasswordRecord passwordRecord() const { return m_record; }

public slots:
    void accept() override;

private slots:
    void generatePassword();
    void copyAll();

private:
    bool commit();

private:
    PasswordRecord m_record;
    QLineEdit* m_name;
    QLineEdit* m_login;
    QLineEdit* m_password;
    QLineEdit* m_password2;

    std::shared_ptr<yaps::CryptoFactory> m_cryptoFactory;
};

#endif // PASSWORDEDITDIALOG_H
