#ifndef PASSWORDEDITDIALOG_H
#define PASSWORDEDITDIALOG_H

#include <QDialog>
#include "PasswordsModel.h"

class QLineEdit;
struct PasswordRecord;

class PasswordEditDialog : public QDialog { Q_OBJECT
public:
    PasswordEditDialog(const QString& title);
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
};

#endif // PASSWORDEDITDIALOG_H
