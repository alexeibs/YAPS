#include "PasswordsModel.h"

PasswordsModel::PasswordsModel(QObject *parent)
    : QSqlQueryModel(parent)
{
}

int PasswordsModel::nameColumnIndex() const
{
    return 1;
}

void PasswordsModel::getRecord(const QString& name, PasswordRecord& record) const
{
}

void PasswordsModel::setRecord(const PasswordRecord& record)
{
}
