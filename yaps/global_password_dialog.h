#pragma once

#include <QDialog>

class QLineEdit;

namespace yaps {

class GlobalPasswordDialog : public QDialog {
  Q_OBJECT
public:
  GlobalPasswordDialog(QString& password);

public slots:
  void accept() override;

private slots:
  void commit();

private:
  QString& password_;
  QLineEdit* passwordEdit_;
};

}  // namespace yaps
