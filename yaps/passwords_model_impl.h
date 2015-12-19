#pragma once

#include <QSqlQueryModel>

#include "passwords_model.h"

namespace yaps {

class PasswordsModelImpl : public QSqlQueryModel, public PasswordsModel {
  Q_OBJECT
public:
  PasswordsModelImpl();

  int nameColumnIndex() const override;
  int getIndexByName(const QString& name) const override;
  int fixIndex(int index) override;

  bool hasRecord(const QString& name) const override;
  std::shared_ptr<PasswordRecord> getRecord(int recordIndex) override;
  void addOrSetRecord(const PasswordRecord&) override;
  void removeRecord(int recordIndex) override;

private:
  // throw exception if index is not valid
  void validateIndex(int index) const;
  void reloadRecords();
};

} // namespace yaps
