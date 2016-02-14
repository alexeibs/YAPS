#include <yapslib/crypto_engine_impl.h>

#include <algorithm>

#include <QString>

#include "test_helpers.h"

namespace yapstest {

const int ITERATIONS = 20;

const QString passwordChars = {
  "-0123456789"
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ_"
  "abcdefghijklmnopqrstuvwxyz"
};


TEST(CryptoEngineImpl, encrypt_decrypt) {
  yaps::CryptoEngineImpl cryptoEngine;

  const QString testPasswords[] = {
    {"secret_password"},
    {"123"}
  };

  const QString testStrings[] = {
    {""},
    {"42"},
    {"test"},
    {"testtest"},
    {"very long string, very long string, very long string"},
    {"Lots of people working in cryptography have no deep concern with real application issues. They are trying to discover things clever enough to write papers about."},
    {"I thought cryptography was a technique that did not require your trusting other people-that if you encrypted your files, you would have the control to make the choice as to whether you would surrender your files."},
    {"Cryptography is the essential building block of independence for organisations on the Internet, just like armies are the essential building blocks of states, because otherwise one state just takes over another."},
    {"The magic words are squeamish ossifrage"}
  };

  QString encrypted, decrypted;
  for (auto& testPassword : testPasswords) {
    for (auto& testString : testStrings) {
      cryptoEngine.encrypt(testPassword, testString, encrypted);
      EXPECT_LE(256, encrypted.size());
      cryptoEngine.decrypt(testPassword, encrypted, decrypted);
      EXPECT_EQ(testString, decrypted);
    }
  }
}

TEST(CryptoEngineImpl, generatePassword) {
  yaps::CryptoEngineImpl cryptoEngine;

  EXPECT_EQ(64, passwordChars.size());
  for (size_t i = 1, imax = passwordChars.size(); i < imax; ++i) {
    EXPECT_LT(passwordChars[i - 1], passwordChars[i]);
  }

  for (int i = 0; i < ITERATIONS; ++i) {
    QString password;
    cryptoEngine.generatePassword(password);
    EXPECT_EQ(16, password.size());
    for (auto& ch : password) {
      EXPECT_TRUE(std::binary_search(passwordChars.begin(), passwordChars.end(), ch));
    }
  }
}

TEST(CryptoEngineImpl, eraseString) {
  yaps::CryptoEngineImpl cryptoEngine;

  QString stringToErase = {"Secret"};
  cryptoEngine.eraseString(stringToErase);
  EXPECT_EQ("", stringToErase);
}

}  // namespace yapstest
