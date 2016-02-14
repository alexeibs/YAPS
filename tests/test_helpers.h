#pragma once

#include <iosfwd>
#include <memory>

#include <gmock/gmock.h>

class QString;

void PrintTo(const QString& s, ::std::ostream* os);

namespace yapstest {

template<typename T, typename... Args>
inline std::shared_ptr<T> make_nice_mock(Args&&... args) {
  return std::make_shared<::testing::NiceMock<T>>(std::forward<Args>(args)...);
}

}  // namespace yaps
