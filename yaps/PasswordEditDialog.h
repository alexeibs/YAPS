#ifndef PASSWORDEDITDIALOG_H
#define PASSWORDEDITDIALOG_H

#include <QDialog>

class QLineEdit;
struct PasswordRecord;

class PasswordEditDialog : public QDialog { Q_OBJECT
public:
    PasswordEditDialog(const QString& title, PasswordRecord&);
    void setNameReadOnly(bool readOnly);

public slots:
    void accept() override;

private slots:
    void commit();
    void generatePassword();

private:
    PasswordRecord& m_record;
    QLineEdit* m_name;
    QLineEdit* m_login;
    QLineEdit* m_password;
    QLineEdit* m_password2;
};

#endif // PASSWORDEDITDIALOG_H
