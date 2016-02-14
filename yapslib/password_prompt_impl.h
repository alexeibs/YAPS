#pragma once

#include "expimp.h"
#include "password_prompt.h"

namespace yaps {

struct YAPSLIB_API PasswordPromptImpl : PasswordPrompt {
  void getPassword(QString& password) override;
};

}  // namespace yaps
