#include "passwords_model_impl.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDateTime>

#include "password_record.h"

namespace yaps {
namespace {

const int NAME_COLUMN = 0;

void checkQueryError(const QSqlQuery& query) {
  auto error = query.lastError();
  if (error.type() != QSqlError::NoError) {
    throw std::runtime_error(error.text().toStdString());
  }
}

}  // namespace

PasswordsModelImpl::PasswordsModelImpl() {
  reloadRecords();
  setHeaderData(0, Qt::Horizontal, QObject::tr("Name"));
  setHeaderData(1, Qt::Horizontal, QObject::tr("Password"));
  setHeaderData(2, Qt::Horizontal, QObject::tr("Date"));
}

int PasswordsModelImpl::nameColumnIndex() const {
  return NAME_COLUMN;
}

int PasswordsModelImpl::getIndexByName(const QString& name) const {
  // binary search, implementation based on
  // http://en.cppreference.com/w/cpp/algorithm/lower_bound
  QString current;
  int first = 0, count = rowCount();
  while (count > 0) {
    int step = count / 2;
    int row = first + step;
    current = data(QSqlQueryModel::index(row, NAME_COLUMN)).toString();
    if (current >= name) {
      count = step;
    } else {
      first = row + 1;
      count -= step + 1;
    }
  }
  current = data(QSqlQueryModel::index(first, NAME_COLUMN)).toString();
  return current == name ? first : 0;
}

int PasswordsModelImpl::fixIndex(int index) {
  if (index <= 0) {
    return 0;
  }
  int nRows = rowCount();
  return index < nRows ? index : nRows - 1;
}

void PasswordsModelImpl::validateIndex(int index) const {
  if (index < 0 || index >= rowCount()) {
    throw std::logic_error("Invalid record index");
  }
}

bool PasswordsModelImpl::hasRecord(const QString& name) const {
  QSqlQuery query;
  query.prepare("SELECT * FROM passwords WHERE name = ?");
  query.addBindValue(name);
  query.exec();
  checkQueryError(query);
  return query.size() > 0;
}

std::shared_ptr<PasswordRecord> PasswordsModelImpl::getRecord(int recordIndex) {
  validateIndex(recordIndex);
  QSqlRecord sqlRecord = QSqlQueryModel::record(recordIndex);
  return std::make_shared<PasswordRecord>(sqlRecord);
}

void PasswordsModelImpl::addOrSetRecord(const PasswordRecord& record) {
  unsigned int now = QDateTime::currentDateTime().toTime_t();
  QSqlQuery query;
  query.prepare(
    "INSERT OR REPLACE INTO passwords (name, password, changed_at)"
    "VALUES (?, ?, ?)"
  );
  query.addBindValue(record.name());
  query.addBindValue(record.password());
  query.addBindValue(now);
  query.exec();
  checkQueryError(query);
  reloadRecords();
}

void PasswordsModelImpl::removeRecord(int recordIndex) {
  validateIndex(recordIndex);
  auto name = QSqlQueryModel::record(recordIndex).value("name").toString();
  QSqlQuery query;
  query.prepare("DELETE FROM passwords WHERE name = ?");
  query.addBindValue(name);
  query.exec();
  checkQueryError(query);
  reloadRecords();
}

void PasswordsModelImpl::reloadRecords() {
  setQuery("SELECT * FROM passwords ORDER BY name");
}

}  // namespace yaps
