#pragma once

class QString;

namespace yaps {

struct PasswordLock {
  virtual ~PasswordLock() {}

  // returns the master password and
  // protects it from being cleared until unlockPassword is called
  virtual const QString& lockPassword() = 0;
  virtual void unlockPassword() = 0;
};

}  // namespace yaps
