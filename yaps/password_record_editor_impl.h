#pragma once

#include "password_record_editor.h"

namespace yaps {

struct SecureClipboard;

struct PasswordRecordEditorImpl : PasswordRecordEditor {
  PasswordRecordEditorImpl(std::shared_ptr<SecureClipboard>);
  ~PasswordRecordEditorImpl();

  bool edit(std::shared_ptr<PasswordRecord>,
            std::unique_ptr<Crypto>,
            const QString& title) override;

private:
  std::shared_ptr<SecureClipboard> clipboard_;
};

}  // namespace yaps
