#include "password_record_editor_impl.h"
#include "password_edit_dialog.h"

namespace yaps {

PasswordRecordEditorImpl::PasswordRecordEditorImpl(std::shared_ptr<SecureClipboard> clipboard,
                                                   std::shared_ptr<MessageBoxFactory> factory)
    : clipboard_(move(clipboard)),
      messageBoxFactory_(move(factory)) {
}

PasswordRecordEditorImpl::~PasswordRecordEditorImpl() {}

bool PasswordRecordEditorImpl::edit(std::shared_ptr<PasswordRecord> record,
                                    std::unique_ptr<Crypto> crypto,
                                    const QString& title) {
  PasswordEditDialog dialog(title, move(record), move(crypto), clipboard_, messageBoxFactory_);
  return dialog.exec() != QDialog::Rejected;
}

}  // namespace yaps
