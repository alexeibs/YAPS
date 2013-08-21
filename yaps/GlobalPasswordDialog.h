#ifndef GLOBALPASSWORDDIALOG_H
#define GLOBALPASSWORDDIALOG_H

#include <QDialog>

class QLineEdit;

class GlobalPasswordDialog : public QDialog { Q_OBJECT
public:
    GlobalPasswordDialog(QString& password);

public slots:
    void accept() override;

private slots:
    void commit();

private:
    QString& m_password;
    QLineEdit* m_passwordEdit;
};

#endif // GLOBALPASSWORDDIALOG_H
