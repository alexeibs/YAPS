#pragma once

#include <memory>

#include "expimp.h"
#include "secure_clipboard.h"

namespace yaps {

struct Clipboard;
struct Timer;

struct YAPSLIB_API SecureClipboardImpl : SecureClipboard {
  SecureClipboardImpl(std::shared_ptr<Clipboard>,
                      std::shared_ptr<Timer> clearTimer,
                      std::shared_ptr<Timer> nextTimer);

  void setContent(QStringList content) override;
  void copyNextItem() override;

private:
  void clearByTimer();
  void copyNextItemByTimer();
  void copyNextItemImpl();

  std::shared_ptr<Clipboard> clipboard_;
  std::shared_ptr<Timer> clearTimer_;
  std::shared_ptr<Timer> nextTimer_;
  QStringList content_;
  bool needToClearClipboard_ = false;
};

}  // namespace yaps
