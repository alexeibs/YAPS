#pragma once

#include <functional>

namespace yaps {

struct Timer {
  typedef std::function<void()> Callback;

  virtual ~Timer() {}
  virtual void start(int timeout) = 0;
  virtual void stop() = 0;
  virtual void setCallback(Callback) = 0;
};

}  // namespace yaps
