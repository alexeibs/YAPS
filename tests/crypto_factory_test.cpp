#include <yapslib/crypto_factory_impl.h>

#include "test_helpers.h"

#include "crypto_engine_mock.h"
#include "crypto_status_view_mock.h"
#include "password_prompt_mock.h"
#include "timer_mock.h"

namespace yapstest {
namespace {

struct TestEnvironment {
  std::shared_ptr<CryptoEngineMock> cryptoEngine;
  std::shared_ptr<PasswordPromptMock> passwordPrompt;
  std::shared_ptr<TimerMock> timer;
  std::shared_ptr<CryptoStatusViewMock> statusView;

  TestEnvironment() {
    cryptoEngine = make_nice_mock<CryptoEngineMock>();
    passwordPrompt = make_nice_mock<PasswordPromptMock>();
    timer = make_nice_mock<TimerMock>();
    statusView = make_nice_mock<CryptoStatusViewMock>();
  }

  std::shared_ptr<yaps::CryptoFactoryImpl> createFactory(bool setupStatusView) {
    auto factory = std::make_shared<yaps::CryptoFactoryImpl>(cryptoEngine, passwordPrompt, timer);
    if (setupStatusView) {
      factory->setCryptoStatusView(statusView.get());
    }
    return factory;
  }
};

}  // anonymous namespace

using ::testing::_;
using ::testing::Return;
using ::testing::SetArgReferee;

TEST(CryptoFactoryImpl, constructor_destructor) {
  TestEnvironment env;

  EXPECT_CALL(*env.timer, setCallback(_))
      .WillOnce(Return());

  EXPECT_CALL(*env.timer, stop())
      .WillOnce(Return());

  EXPECT_CALL(*env.cryptoEngine, eraseString(_))
      .WillOnce(Return());

  env.createFactory(false);
}

TEST(CryptoFactoryImpl, setCryptoStatusView) {
  TestEnvironment env;

  EXPECT_CALL(*env.statusView, updateCryptoStatus(_))
      .Times(1)
      .WillOnce(Return());

  auto factory = env.createFactory(false);

  factory->setCryptoStatusView(env.statusView.get());
  factory->setCryptoStatusView(nullptr);
}

TEST(CryptoFactoryImpl, hasMasterPassword) {
  TestEnvironment env;

  ON_CALL(*env.passwordPrompt, getPassword(_))
      .WillByDefault(SetArgReferee<0>("testpwd"));

  auto factory = env.createFactory(false);
  EXPECT_FALSE(factory->hasMasterPassword());

  factory->getCrypto();
  EXPECT_TRUE(factory->hasMasterPassword());
}

TEST(CryptoFactoryImpl, clearMasterPassword_unlocked) {
  TestEnvironment env;

  ON_CALL(*env.passwordPrompt, getPassword(_))
      .WillByDefault(SetArgReferee<0>("testpwd"));

  EXPECT_CALL(*env.timer, stop())
      .Times(2)
      .WillRepeatedly(Return());

  EXPECT_CALL(*env.cryptoEngine, eraseString(_))
      .WillOnce(SetArgReferee<0>(""));

  QString expectedPassword = "testpwd";
  EXPECT_CALL(*env.cryptoEngine, eraseString(expectedPassword))
      .WillOnce(SetArgReferee<0>(""));

  auto factory = env.createFactory(false);

  factory->getCrypto();
  factory->clearMasterPassword();
  EXPECT_FALSE(factory->hasMasterPassword());
}

TEST(CryptoFactoryImpl, clearMasterPassword_locked) {
  TestEnvironment env;

  ON_CALL(*env.passwordPrompt, getPassword(_))
      .WillByDefault(SetArgReferee<0>("testpwd"));

  EXPECT_CALL(*env.timer, stop())
      .WillOnce(Return());

  EXPECT_CALL(*env.cryptoEngine, eraseString(_))
      .WillOnce(SetArgReferee<0>(""));

  EXPECT_CALL(*env.timer, start(yaps::CryptoFactoryImpl::EXPIRATION_INTERVAL))
      .WillOnce(Return());

  EXPECT_CALL(*env.timer, start(yaps::CryptoFactoryImpl::RESCHEDULE_INTERVAL))
      .WillOnce(Return());

  auto factory = env.createFactory(false);

  auto crypto = factory->getCrypto();
  factory->clearMasterPassword();
  EXPECT_TRUE(factory->hasMasterPassword());
}

TEST(CryptoFactoryImpl, getCrypto) {
  TestEnvironment env;

  EXPECT_CALL(*env.passwordPrompt, getPassword(_))
      .Times(2)
      .WillOnce(SetArgReferee<0>(""))
      .WillOnce(SetArgReferee<0>("testpwd"));

  EXPECT_CALL(*env.timer, start(yaps::CryptoFactoryImpl::EXPIRATION_INTERVAL))
      .Times(2)
      .WillRepeatedly(Return());

  EXPECT_CALL(*env.cryptoEngine, encrypt(QString{"testpwd"}, QString{"Secret"}, _))
      .Times(1)
      .WillOnce(SetArgReferee<2>("Secret_encrypted"));

  EXPECT_CALL(*env.statusView, updateCryptoStatus(_))
      .Times(2)
      .WillRepeatedly(Return());

  auto factory = env.createFactory(true);
  auto crypto = factory->getCrypto();
  EXPECT_EQ(nullptr, crypto.get());

  crypto = factory->getCrypto();
  EXPECT_NE(nullptr, crypto.get());

  QString secret = {"Secret"};
  crypto->encrypt(secret);
  EXPECT_EQ("Secret_encrypted", secret);

  crypto.reset();

  crypto = factory->getCrypto();
  EXPECT_NE(nullptr, crypto.get());
}

TEST(CryptoFactoryImpl, lockPassword_unlockPassword) {
  TestEnvironment env;

  EXPECT_CALL(*env.timer, start(yaps::CryptoFactoryImpl::RESCHEDULE_INTERVAL))
      .WillOnce(Return());

  auto factory = env.createFactory(false);
  factory->clearMasterPassword();

  EXPECT_EQ("", factory->lockPassword());
  factory->clearMasterPassword();

  EXPECT_THROW({
    factory->lockPassword();
  }, std::logic_error);

  factory->unlockPassword();
  factory->clearMasterPassword();

  EXPECT_THROW({
    factory->unlockPassword();
  }, std::logic_error);
}

}  // namespace
