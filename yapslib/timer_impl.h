#pragma once

#include <memory>

#include <QObject>

#include "expimp.h"
#include "timer.h"

class QTimer;

namespace yaps {

struct YAPSLIB_API TimerImpl : QObject, Timer {
  TimerImpl();

  // throws std::logic_error if the callback is not a valid callable object
  void start(int timeout) override;
  void stop() override;
  void setCallback(Callback callback) override;

private slots:
  // throws std::bad_function_call if the callback is not specified
  void onTimer();

private:
  Q_OBJECT
  QTimer* timer_;
  Callback callback_;
};

}  // namespace yaps
