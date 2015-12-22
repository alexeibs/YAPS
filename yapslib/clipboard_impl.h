#pragma once

#include "clipboard.h"

namespace yaps {

struct ClipboardImpl : Clipboard {
  void clear() override;
  void setText(const QString& text) override;
};

}  // namespace yaps
