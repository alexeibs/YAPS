#pragma once

#include <memory>

#include <QString>

class QSqlRecord;

namespace yaps {

struct PasswordRecord;

struct PasswordsModel {
  virtual ~PasswordsModel() {}

  virtual int nameColumnIndex() const = 0;
  virtual int getIndexByName(const QString& name) const = 0;
  // returns the closest valid index
  virtual int fixIndex(int index) = 0;

  virtual bool hasRecord(const QString& name) const = 0;
  virtual std::shared_ptr<PasswordRecord> getRecord(int recordIndex) = 0;
  virtual void addOrSetRecord(const PasswordRecord&) = 0;
  virtual void removeRecord(int recordIndex) = 0;
};

}  // namespace yaps
