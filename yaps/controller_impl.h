#pragma once

#include <memory>

#include "controller.h"

namespace yaps {

struct CryptoFactory;
struct PasswordsModel;
struct ViewState;
struct SecureClipboard;
struct PasswordRecordEditor;

struct ControllerImpl : Controller {
  ControllerImpl(std::shared_ptr<CryptoFactory>,
                 std::shared_ptr<PasswordsModel>,
                 std::shared_ptr<SecureClipboard>,
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
  std::shared_ptr<PasswordRecordEditor> passwordRecordEditor_;
  ViewState* viewState_;
};

}  // namespace yaps
