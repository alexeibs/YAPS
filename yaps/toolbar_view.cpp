#include "toolbar_view.h"

#include <QAction>

#include "controller.h"

namespace yaps {

ToolbarView::ToolbarView(QWidget* actionParent,
                         Controller& controller)
    : controller_(controller),
      actionParent_(actionParent) {

  QAction* action;
  action = createAction(tr("Copy to Clipboard (Ctrl+C)"),
                        QIcon(":/icons/copy"),
                        QKeySequence(Qt::CTRL + Qt::Key_C));
  connect(action, SIGNAL(triggered()), this, SLOT(copyToClipboard()));

  action = createAction(tr("Copy password only (Ctrl+Shift+C)"),
                        QIcon(":/icons/copy_pwd"),
                        QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_C));
  connect(action, SIGNAL(triggered()), this, SLOT(copyPasswordToClipboard()));

  addSeparator();

  action = createAction(tr("Add Password (Ctrl+N)"),
                        QIcon(":/icons/add"),
                        QKeySequence(Qt::CTRL + Qt::Key_N));
  connect(action, SIGNAL(triggered()), this, SLOT(addPassword()));

  action = createAction(tr("Edit Password (F2)"),
                        QIcon(":/icons/edit"),
                        QKeySequence(Qt::Key_F2));
  connect(action, SIGNAL(triggered()), this, SLOT(editPassword()));

  action = createAction(tr("Remove Password (Ctrl+Delete)"),
                        QIcon(":/icons/delete"),
                        QKeySequence(Qt::CTRL + Qt::Key_Delete));
  connect(action, SIGNAL(triggered()), this, SLOT(deletePassword()));

  addSeparator();

  clearMasterPasswordAction_ = createAction(tr("Reset master password (F12)"),
                                            QIcon(":/icons/forget_pwd"),
                                            QKeySequence(Qt::Key_F12));
  connect(clearMasterPasswordAction_, SIGNAL(triggered()), this, SLOT(clearMasterPassword()));

  actionParent_ = nullptr;
}

ToolbarView::~ToolbarView() {}

QAction* ToolbarView::createAction(const QString& name,
                                  const QIcon& icon,
                                  const QKeySequence& shortcut) {
  auto action = new QAction(icon, name, actionParent_);
  action->setShortcut(shortcut);
  addAction(action);
  return action;
}

void ToolbarView::copyToClipboard() {
  controller_.copyToClipboard();
}

void ToolbarView::copyPasswordToClipboard() {
  controller_.copyPasswordToClipboard();
}

void ToolbarView::addPassword() {
  controller_.addPassword();
}

void ToolbarView::editPassword() {
  controller_.editPassword();
}

void ToolbarView::deletePassword() {
  controller_.deletePassword();
}

void ToolbarView::clearMasterPassword() {
  controller_.clearMasterPassword();
}

void ToolbarView::updateCryptoStatus(const CryptoStatus& cryptoStatus) {
  clearMasterPasswordAction_->setEnabled(cryptoStatus.hasMasterPassword());
}

}  // namespace yaps
