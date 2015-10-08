#pragma once

#include "password_prompt.h"

namespace yaps {

struct PasswordPromptImpl : PasswordPrompt {
  void getPassword(QString& password) override;
};

}  // namespace yaps
