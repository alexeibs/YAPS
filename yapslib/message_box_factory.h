#pragma once

class QString;

namespace yaps {

struct MessageBoxFactory {
  virtual ~MessageBoxFactory() {}
  virtual void showWarning(const QString& title, const QString& warning) = 0;
  virtual void showError(const QString& title, const QString& error) = 0;
  virtual bool askYesNo(const QString& title, const QString& question) = 0;
};

}  // namespace yaps
