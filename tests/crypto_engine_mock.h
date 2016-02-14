#pragma once

#include <QString>

#include <gmock/gmock.h>

#include <yapslib/crypto_engine.h>

namespace yapstest {

struct CryptoEngineMock : yaps::CryptoEngine {
  MOCK_METHOD3(encrypt, void(const QString& password, const QString& input, QString& output));
  MOCK_METHOD3(decrypt, void(const QString& password, const QString& input, QString& output));
  MOCK_METHOD1(generatePassword, void(QString& password));
  MOCK_METHOD1(eraseString, void(QString& stringToErase));
};

}  // namespace yapstest
