#include "password_prompt_impl.h"

#include "GlobalPasswordDialog.h"

namespace yaps {

void PasswordPromptImpl::getPassword(QString& password) {
  GlobalPasswordDialog(password).exec();
}

}  // namespace yaps
