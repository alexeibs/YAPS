#pragma once

#include <memory>

#include "controller.h"
#include "expimp.h"

namespace yaps {

struct CryptoFactory;
struct MessageBoxFactory;
struct PasswordRecordEditor;
struct PasswordsModel;
struct SecureClipboard;
struct ViewState;

struct YAPSLIB_API ControllerImpl : Controller {
  ControllerImpl(std::shared_ptr<CryptoFactory>,
                 std::shared_ptr<PasswordsModel>,
                 std::shared_ptr<SecureClipboard>,
                 std::shared_ptr<MessageBoxFactory>,
                 std::shared_ptr<PasswordRecordEditor>);
  ~ControllerImpl();

  void copyToClipboard() override;
  void copyPasswordToClipboard() override;
  void copyNextItemToClipboard() override;

  void addPassword() override;
  void editPassword() override;
  void deletePassword() override;

  void clearMasterPassword() override;

  void setViewState(ViewState*);

private:
  template<typename Callback>
  void copyToClipboardImpl(Callback&& callback);

  ViewState& viewState();
  const ViewState& viewState() const;

private:
  std::shared_ptr<CryptoFactory> cryptoFactory_;
  std::shared_ptr<PasswordsModel> passwordsModel_;
  std::shared_ptr<SecureClipboard> clipboard_;
  std::shared_ptr<MessageBoxFactory> messageBoxFactory_;
  std::shared_ptr<PasswordRecordEditor> passwordRecordEditor_;
  ViewState* viewState_;
};

}  // namespace yaps
