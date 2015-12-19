#pragma once

#include <memory>

#include <QDialog>

class QLineEdit;

namespace yaps {

struct Crypto;
struct PasswordRecord;
struct SecureClipboard;

class PasswordEditDialog : public QDialog {
  Q_OBJECT
public:
  PasswordEditDialog(const QString& title,
                     std::shared_ptr<PasswordRecord>,
                     std::unique_ptr<Crypto>,
                     std::shared_ptr<SecureClipboard>);
  ~PasswordEditDialog();

public slots:
  void accept() override;

private slots:
  void generatePassword();
  void copyAll();

private:
  void initControls();
  void saveRecord();

private:
  QLineEdit* nameEdit_;
  QLineEdit* loginEdit_;
  QLineEdit* passwordEdit1_;
  QLineEdit* passwordEdit2_;

  std::shared_ptr<PasswordRecord> record_;
  std::unique_ptr<Crypto> crypto_;
  std::shared_ptr<SecureClipboard> clipboard_;
};

}  // namespace yaps
