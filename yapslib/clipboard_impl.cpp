#include "clipboard_impl.h"

#include <QApplication>
#include <QClipboard>

namespace yaps {

void yaps::ClipboardImpl::clear() {
  QApplication::clipboard()->clear();
}

void yaps::ClipboardImpl::setText(const QString& text) {
  QApplication::clipboard()->setText(text);
}

}  // namespace yaps
