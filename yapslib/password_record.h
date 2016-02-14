#pragma once

#include <QString>

#include "expimp.h"

class QSqlRecord;

namespace yaps {

struct YAPSLIB_API PasswordRecord {
  PasswordRecord();
  PasswordRecord(const QSqlRecord&);

  const QString& name() const;
  // throws exception if the record already has a name
  void setName(const QString& newName);

  const QString& password() const;
  void setPassword(const QString& newPassword);

  unsigned int timestamp() const;

private:
  QString name_;
  QString password_;
  unsigned int timestamp_ = 0;
};

}  // namespace yaps
