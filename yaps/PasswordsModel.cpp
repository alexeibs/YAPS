#include "PasswordsModel.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDateTime>
#include <QMessageBox>

static void readPasswordRecord(const QSqlRecord& sqlRecord, PasswordRecord& record)
{
    record.name = sqlRecord.value("name").toString();
    record.password = sqlRecord.value("password").toString();
    record.timestamp = sqlRecord.value("changed_at").toUInt();
}

PasswordsModel::PasswordsModel(QObject *parent)
    : QSqlQueryModel(parent)
{
    refresh();
    setHeaderData(0, Qt::Horizontal, QObject::tr("Name"));
    setHeaderData(1, Qt::Horizontal, QObject::tr("Password"));
    setHeaderData(2, Qt::Horizontal, QObject::tr("Date"));
}

int PasswordsModel::nameColumnIndex() const
{
    return 0;
}

bool PasswordsModel::isValid(const QModelIndex& index) const
{
    if (!index.isValid())
        return false;
    int row = index.row(), column = index.column();
    return row >= 0 && row < rowCount() && column >= 0 && column < columnCount();
}

QModelIndex PasswordsModel::getIndexByName(const QString& name)
{
    // binary search, implementation based on
    // http://en.cppreference.com/w/cpp/algorithm/lower_bound
    QString current;
    int first = 0, count = rowCount();
    while (count > 0) {
        int step = count / 2;
        int row = first + step;
        current = data(QSqlQueryModel::index(row, 0)).toString();
        if (current >= name) {
            count = step;
        } else {
            first = row + 1;
            count -= step + 1;
        }
    }
    current = data(QSqlQueryModel::index(first, 0)).toString();
    int row = (current == name) ? first : 0;
    return QSqlQueryModel::index(row, 0);
}

static inline int checkBounds(int n, int from, int to)
{
    if (n < from)
        n = from;
    else if (n >= to)
        n = to - 1;
    return n;
}

QModelIndex PasswordsModel::fixIndex(const QModelIndex& index)
{
    int newRow = 0, newColumn = 0;
    if (index.isValid()) {
        newRow = checkBounds(index.row(), 0, rowCount());
        newColumn = checkBounds(index.column(), 0, columnCount());
    }
    return QSqlQueryModel::index(newRow, newColumn);
}

bool PasswordsModel::getRecord(const QModelIndex& index, PasswordRecord& record) const
{
    if (!isValid(index))
        return false;
    QSqlRecord sqlRecord = QSqlQueryModel::record(index.row());
    readPasswordRecord(sqlRecord, record);
    return true;
}

bool PasswordsModel::getRecord(const QString& name, PasswordRecord& record) const
{
    QSqlQuery query;
    query.prepare("SELECT * FROM passwords WHERE name = ?");
    query.addBindValue(name);
    query.exec();
    if (query.lastError().type() != QSqlError::NoError) {
        QMessageBox::critical(0, tr("Error"), query.lastError().text());
        return false;
    }
    if (!query.first())
        return false;
    readPasswordRecord(query.record(), record);
    return true;
}

bool PasswordsModel::hasRecord(const QString& name)
{
    PasswordRecord record;
    return getRecord(name, record);
}

bool PasswordsModel::addOrSetRecord(const PasswordRecord& record)
{
    unsigned int now = QDateTime::currentDateTime().toTime_t();
    QSqlQuery query;
    query.prepare(
        "INSERT OR REPLACE INTO passwords (name, password, changed_at)"
        "VALUES (?, ?, ?)"
    );
    query.addBindValue(record.name);
    query.addBindValue(record.password);
    query.addBindValue(now);
    query.exec();
    if (query.lastError().type() != QSqlError::NoError) {
        QMessageBox::critical(0, tr("Error"), query.lastError().text());
        return false;
    }
    refresh();
    return true;
}

void PasswordsModel::removeRecord(const QModelIndex& index)
{
    if (!isValid(index))
        return;
    auto name = QSqlQueryModel::record(index.row()).value("name").toString();
    QSqlQuery query;
    query.prepare("DELETE FROM passwords WHERE name = ?");
    query.addBindValue(name);
    query.exec();
    refresh();
}

void PasswordsModel::refresh()
{
    setQuery("SELECT * FROM passwords ORDER BY name");
}
