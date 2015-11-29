#include "secure_clipboard_impl.h"

#include <QApplication>
#include <QClipboard>

#include "crypto_engine.h"
#include "timer.h"

namespace yaps {

const int CLEAR_TIMEOUT = 5000;
const int NEXT_TIMEOUT = 50;

SecureClipboardImpl::SecureClipboardImpl(std::shared_ptr<Timer> clearTimer,
                                         std::shared_ptr<Timer> nextTimer)
    : clearTimer_(move(clearTimer)),
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
    QApplication::clipboard()->clear();
  } else {
    copyNextItemImpl();
  }
}

void SecureClipboardImpl::copyNextItemImpl() {
  QApplication::clipboard()->setText(content_.first());
  yaps::eraseString(content_.first());
  content_.removeFirst();
  clearTimer_->start(CLEAR_TIMEOUT);
}

void SecureClipboardImpl::clearByTimer() {
  QApplication::clipboard()->clear();
  for (auto& item : content_) {
    eraseString(item);
  }
  content_.clear();
}

}  // namespace yaps
