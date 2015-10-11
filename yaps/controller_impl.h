#pragma once

#include <memory>

#include "controller.h"

class PasswordsModel;

namespace yaps {

struct CryptoFactory;
struct CryptoStatus;
struct ViewState;

struct ControllerImpl : Controller {
  ControllerImpl(std::shared_ptr<yaps::CryptoFactory>,
                 std::shared_ptr<yaps::CryptoStatus>,
                 PasswordsModel&);
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
  std::shared_ptr<yaps::CryptoFactory> cryptoFactory_;
  std::shared_ptr<yaps::CryptoStatus> cryptoStatus_;
  PasswordsModel& passwordsModel_;
  ViewState* viewState_;
};

}  // namespace yaps
