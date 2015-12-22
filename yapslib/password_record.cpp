#include "password_record.h"

#include <QSqlRecord>
#include <QVariant>

#include "crypto_engine.h"

namespace yaps {

PasswordRecord::PasswordRecord() {}

PasswordRecord::PasswordRecord(const QSqlRecord& record) {
  name_ = record.value("name").toString();
  password_ = record.value("password").toString();
  timestamp_ = record.value("changed_at").toUInt();
}

const QString& PasswordRecord::name() const {
  return name_;
}

// does nothing if the record already has a name
void PasswordRecord::setName(const QString& newName) {
  if (!name_.isEmpty()) {
    throw std::logic_error("PasswordRecord already has a name");
  }
  yaps::eraseString(name_);
  name_ = newName;
}

const QString& PasswordRecord::password() const {
  return password_;
}

void PasswordRecord::setPassword(const QString& newPassword) {
  yaps::eraseString(password_);
  password_ = newPassword;
}

unsigned int PasswordRecord::timestamp() const {
  return timestamp_;
}

}  // namespace yaps

