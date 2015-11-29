#pragma once

#include <memory>

#include "secure_clipboard.h"

namespace yaps {

struct Timer;

struct SecureClipboardImpl : SecureClipboard {
  SecureClipboardImpl(std::shared_ptr<Timer> clearTimer, std::shared_ptr<Timer> nextTimer);

  void setContent(QStringList content) override;
  void copyNextItem() override;

private:
  void clearByTimer();
  void copyNextItemByTimer();
  void copyNextItemImpl();

  std::shared_ptr<Timer> clearTimer_;
  std::shared_ptr<Timer> nextTimer_;
  QStringList content_;
  bool needToClearClipboard_ = false;
};

}  // namespace yaps
