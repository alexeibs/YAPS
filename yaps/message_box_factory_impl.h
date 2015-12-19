#pragma once

#include "message_box_factory.h"

class QWidget;

namespace yaps {

struct MessageBoxFactoryImpl : MessageBoxFactory {
  void setParentWindow(QWidget* parent);

  void showWarning(const QString& title, const QString& warning) override;
  void showError(const QString& title, const QString& error) override;
  bool askYesNo(const QString& title, const QString& question) override;

private:
  QWidget* parent_;
};


}  // namespace yaps