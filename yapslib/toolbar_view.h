#pragma once

#include <memory>

#include <QToolBar>

#include "crypto_status.h"

namespace yaps {

struct Controller;

class ToolbarView : public QToolBar, public CryptoStatusView {
  Q_OBJECT
public:
  ToolbarView(QWidget* actionParent, Controller&);
  ~ToolbarView();

  // CryptoStatusView interface
  void updateCryptoStatus(const CryptoStatus&) override;

private slots:
  void copyToClipboard();
  void copyPasswordToClipboard();

  void addPassword();
  void editPassword();
  void deletePassword();

  void clearMasterPassword();

private:
  QAction* createAction(const QString& name, const QIcon& icon, const QKeySequence& shortcut);

  Controller& controller_;
  QAction* clearMasterPasswordAction_ = nullptr;

  // should be used only in constructor
  QWidget* actionParent_ = nullptr;
};

}  // namespace yaps
