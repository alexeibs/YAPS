#pragma once

#include <QString>

namespace yaps {

struct PasswordRecord {
  QString name;
  QString password;
  unsigned int timestamp = 0;
};

struct PasswordsModel {
  virtual ~PasswordsModel() {}

  virtual int nameColumnIndex() const = 0;
  virtual int getIndexByName(const QString& name) const = 0;
  // returns the closest valid index
  virtual int fixIndex(int index) = 0;

  virtual bool hasRecord(const QString& name) const = 0;
  virtual void getRecord(int recordIndex, PasswordRecord&) = 0;
  virtual void addOrSetRecord(const PasswordRecord&) = 0;
  virtual void removeRecord(int recordIndex) = 0;
};

}  // namespace yaps
