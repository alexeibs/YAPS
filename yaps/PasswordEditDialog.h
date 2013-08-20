#ifndef PASSWORDEDITDIALOG_H
#define PASSWORDEDITDIALOG_H

#include <QDialog>

class QLineEdit;
struct PasswordRecord;

class PasswordEditDialog : public QDialog { Q_OBJECT
public:
    PasswordEditDialog(QWidget *parent
        , const QString& title, PasswordRecord&);
    void setNameReadOnly(bool readOnly);

public slots:
    void accept() override;

private slots:
    void commit();

private:
    PasswordRecord& m_record;
    QLineEdit* m_name;
    QLineEdit* m_password;
};

#endif // PASSWORDEDITDIALOG_H
