#pragma once

namespace yaps {

struct CryptoStatus {
  virtual ~CryptoStatus() {}
  virtual bool hasMasterPassword() const = 0;
};

struct CryptoStatusView {
  virtual ~CryptoStatusView() {}
  virtual void updateCryptoStatus(const CryptoStatus&) = 0;
};

}  // namespace yaps
