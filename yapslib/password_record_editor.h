#pragma once

#include <memory>

#include "crypto.h"

class QString;

namespace yaps {

struct PasswordRecord;

struct PasswordRecordEditor {
  virtual ~PasswordRecordEditor() {}
  // returns false if the record is not changed
  virtual bool edit(std::shared_ptr<PasswordRecord>,
                    std::unique_ptr<Crypto>,
                    const QString& title) = 0;
};

}  // namespace yaps
