#pragma once

#include "clipboard.h"
#include "expimp.h"

namespace yaps {

struct YAPSLIB_API ClipboardImpl : Clipboard {
  void clear() override;
  void setText(const QString& text) override;
};

}  // namespace yaps
