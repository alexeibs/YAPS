#include "message_box_factory_impl.h"

#include <QMessageBox>

namespace yaps {

void MessageBoxFactoryImpl::setParentWindow(QWidget* parent) {
  parent_ = parent;
}

void MessageBoxFactoryImpl::showWarning(const QString& title, const QString& warning) {
  QMessageBox::warning(parent_, title, warning);
}

void MessageBoxFactoryImpl::showError(const QString& title, const QString& error) {
  QMessageBox::critical(parent_, title, error);
}

bool MessageBoxFactoryImpl::askYesNo(const QString& title, const QString& question) {
  auto answer = QMessageBox::question(parent_, title, question);
  return answer == QMessageBox::Yes;
}

}  // namespace yaps
