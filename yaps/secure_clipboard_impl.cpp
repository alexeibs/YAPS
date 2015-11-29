#include "secure_clipboard_impl.h"

#include "clipboard.h"
#include "crypto_engine.h"
#include "timer.h"

namespace yaps {

const int CLEAR_TIMEOUT = 5000;
const int NEXT_TIMEOUT = 50;

SecureClipboardImpl::SecureClipboardImpl(std::shared_ptr<Clipboard> clipboard,
                                         std::shared_ptr<Timer> clearTimer,
                                         std::shared_ptr<Timer> nextTimer)
    : clipboard_(move(clipboard)),
      clearTimer_(move(clearTimer)),
      nextTimer_(move(nextTimer)) {

  clearTimer_->setCallback([this] {
    clearByTimer();
  });
  nextTimer_->setCallback([this] {
    copyNextItemByTimer();
  });
}

void SecureClipboardImpl::setContent(QStringList content) {
  nextTimer_->stop();
  clearTimer_->stop();
  if (!content_.empty()) {
    clearByTimer();
  }
  content_ = std::move(content);
  if (!content_.empty()) {
    copyNextItemImpl();
  }
}

void SecureClipboardImpl::copyNextItem() {
  clearTimer_->stop();
  nextTimer_->start(NEXT_TIMEOUT);
}

void SecureClipboardImpl::copyNextItemByTimer() {
  if (content_.empty()) {
    clipboard_->clear();
  } else {
    copyNextItemImpl();
  }
}

void SecureClipboardImpl::copyNextItemImpl() {
  clipboard_->setText(content_.first());
  yaps::eraseString(content_.first());
  content_.removeFirst();
  clearTimer_->start(CLEAR_TIMEOUT);
}

void SecureClipboardImpl::clearByTimer() {
  clipboard_->clear();
  for (auto& item : content_) {
    eraseString(item);
  }
  content_.clear();
}

}  // namespace yaps
