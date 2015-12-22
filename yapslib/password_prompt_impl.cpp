#include "password_prompt_impl.h"

#include "global_password_dialog.h"

namespace yaps {

void PasswordPromptImpl::getPassword(QString& password) {
  GlobalPasswordDialog(password).exec();
}

}  // namespace yaps
