#pragma once

namespace yaps {

struct CryptoStatus {
  virtual ~CryptoStatus() {}
  virtual void clearPassword() = 0;
  virtual bool hasPassword() const = 0;
};

struct CryptoStatusView {
  virtual ~CryptoStatusView() {}
  virtual void updateCryptoStatus() = 0;
};

}  // namespace yaps
