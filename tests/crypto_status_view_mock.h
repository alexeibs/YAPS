#pragma once

#include <gmock/gmock.h>

#include <yapslib/crypto_status.h>

namespace yapstest {

struct CryptoStatusViewMock : yaps::CryptoStatusView {
  MOCK_METHOD1(updateCryptoStatus, void(const yaps::CryptoStatus&));
};

}  // namespace
