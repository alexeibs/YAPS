#include "timer_impl.h"

#include <QTimer>

namespace yaps {

TimerImpl::TimerImpl() {
  timer_ = new QTimer(this);
  timer_->setSingleShot(true);
  connect(timer_, SIGNAL(timeout()), this, SLOT(onTimer()));
}

void TimerImpl::start(int timeout) {
  if (!callback_) {
    throw std::logic_error("Timer callback is not set");
  }
  timer_->start(timeout);
}

void TimerImpl::stop() {
  timer_->stop();
}

void TimerImpl::setCallback(Callback callback) {
  callback_ = move(callback);
}

void TimerImpl::onTimer() {
  callback_();
}

}  // namespace yaps
