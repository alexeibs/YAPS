#pragma once

#include <gmock/gmock.h>

#include <yapslib/password_prompt.h>

namespace yapstest {

struct PasswordPromptMock : yaps::PasswordPrompt {
  MOCK_METHOD1(getPassword, void(QString& password));
};

}  // namespace yapstest
