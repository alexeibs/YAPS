#pragma once

class QString;

namespace yaps {

struct Clipboard {
  virtual ~Clipboard() {}

  virtual void clear() = 0;
  virtual void setText(const QString& text) = 0;
};

}  // namespace yaps
