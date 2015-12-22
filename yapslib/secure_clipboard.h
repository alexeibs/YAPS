#pragma once

#include <QStringList>

namespace yaps {

struct SecureClipboard {
  virtual ~SecureClipboard() {}

  virtual void setContent(QStringList content) = 0;
  virtual void copyNextItem() = 0;
};

}  // namespace yaps
