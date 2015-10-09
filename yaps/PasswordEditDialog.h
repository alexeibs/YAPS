#ifndef PASSWORDEDITDIALOG_H
#define PASSWORDEDITDIALOG_H

#include <memory>

#include <QDialog>

#include "PasswordsModel.h"

class QLineEdit;
struct PasswordRecord;

namespace yaps {
    struct Crypto;
}

class PasswordEditDialog : public QDialog { Q_OBJECT
public:
    PasswordEditDialog(const QString& title, std::unique_ptr<yaps::Crypto>);
    ~PasswordEditDialog();
    void setNameReadOnly(bool readOnly);

    void setPasswordRecord(const PasswordRecord&);
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

    std::unique_ptr<yaps::Crypto> m_crypto;
};

#endif // PASSWORDEDITDIALOG_H
