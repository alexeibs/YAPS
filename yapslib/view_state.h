#pragma once

namespace yaps {

struct ViewState {
  virtual ~ViewState() {}

  virtual int currentRecordIndex() const = 0;
  virtual void setCurrentRecordIndex(int index) = 0;

  virtual void toggleMainWindow() = 0;
};

}  // namespace yaps
