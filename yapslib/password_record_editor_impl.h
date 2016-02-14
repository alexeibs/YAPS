#pragma once

#include "expimp.h"
#include "password_record_editor.h"

namespace yaps {

struct MessageBoxFactory;
struct SecureClipboard;

struct YAPSLIB_API PasswordRecordEditorImpl : PasswordRecordEditor {
  PasswordRecordEditorImpl(std::shared_ptr<SecureClipboard>, std::shared_ptr<MessageBoxFactory>);
  ~PasswordRecordEditorImpl();

  bool edit(std::shared_ptr<PasswordRecord>,
            std::unique_ptr<Crypto>,
            const QString& title) override;

private:
  std::shared_ptr<SecureClipboard> clipboard_;
  std::shared_ptr<MessageBoxFactory> messageBoxFactory_;
};

}  // namespace yaps
