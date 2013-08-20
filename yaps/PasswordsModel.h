#ifndef PASSWORDSMODEL_H
#define PASSWORDSMODEL_H

#include <QSqlQueryModel>

struct PasswordRecord {
    int id;
    QString name;
    QString password;
    unsigned int timestamp;
};

class PasswordsModel : public QSqlQueryModel { Q_OBJECT
public:
    explicit PasswordsModel(QObject *parent = 0);

    int nameColumnIndex() const;
    void getRecord(const QString& name, PasswordRecord&) const;
    void setRecord(const PasswordRecord&);
};

#endif // PASSWORDSMODEL_H
