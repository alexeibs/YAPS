#pragma once

#include <gmock/gmock.h>

#include <yapslib/timer.h>

namespace yapstest {

struct TimerMock : yaps::Timer {
  MOCK_METHOD1(start, void(int timeout));
  MOCK_METHOD0(stop, void());
  MOCK_METHOD1(setCallback, void(yaps::Timer::Callback));
};

}  // namespace yapstest
