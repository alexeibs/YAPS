#pragma once

namespace yaps {

struct Controller {
  virtual ~Controller() {}

  virtual void copyToClipboard() = 0;
  virtual void copyPasswordToClipboard() = 0;
  virtual void copyNextItemToClipboard() = 0;

  virtual void addPassword() = 0;
  virtual void editPassword() = 0;
  virtual void deletePassword() = 0;

  virtual void clearMasterPassword() = 0;
};

}  // namespace yaps
