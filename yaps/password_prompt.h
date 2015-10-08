#pragma once

class QString;

namespace yaps {

struct PasswordPrompt {
  virtual ~PasswordPrompt() {}
  virtual void getPassword(QString& password) = 0;
};

}  // namespace yaps
