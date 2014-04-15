#ifndef PASSWORDSMODEL_H
#define PASSWORDSMODEL_H

#include <QSqlQueryModel>

struct PasswordRecord {
    QString name;
    QString password;
    unsigned int timestamp = 0;
};

class PasswordsModel : public QSqlQueryModel { Q_OBJECT
public:
    explicit PasswordsModel(QObject *parent = 0);

    int nameColumnIndex() const;
    bool isValid(const QModelIndex&) const;
    QModelIndex getIndexByName(const QString& name);
    QModelIndex fixIndex(const QModelIndex&);

    bool getRecord(const QModelIndex&, PasswordRecord&) const;
    bool getRecord(const QString& name, PasswordRecord&) const;
    bool hasRecord(const QString& name);
    bool addOrSetRecord(const PasswordRecord&);
    void removeRecord(const QModelIndex&);

private:
    void refresh();
};

#endif // PASSWORDSMODEL_H
