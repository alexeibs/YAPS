#pragma once

#include <gmock/gmock.h>

#include <yapslib/crypto.h>

namespace yapstest {

struct CryptoMock : yaps::Crypto {
  MOCK_METHOD1(encrypt, void(QString& text));
  MOCK_METHOD2(decrypt, void(const QString& input, QString& output));
  MOCK_METHOD1(generatePassword, void(QString& password));
  MOCK_METHOD1(eraseString, void(QString& stringToErase));
};

}  // namespace yapstest
