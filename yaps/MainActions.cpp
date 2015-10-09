#include "MainActions.h"

#include <QAction>
#include <QListView>
#include <QMessageBox>

#include "crypto.h"

#include "mainwindow.h"
#include "PasswordsModel.h"
#include "Database.h"
#include "View.h"
#include "SecureClipboard.h"
#include "PasswordEditDialog.h"

std::shared_ptr<Actions> Actions::instance()
{
    static std::shared_ptr<Actions> single(new Actions);
    return single;
}

Actions::Actions()
    : m_mainWindow(nullptr)
    , m_model(nullptr)
    , m_view(nullptr)
{
}

void Actions::setMainWindow(MainWindow* window)
{
    m_mainWindow = window;
}

QAbstractItemModel* Actions::model()
{
    return m_model;
}

QAbstractItemView* Actions::view()
{
    return m_view;
}

void Actions::initialize()
{
    if (!m_mainWindow)
        throw std::runtime_error("Main window does not exists");

    m_model = makeModel(this);
    m_view = makeView(m_model, m_model->nameColumnIndex());
    m_view->setCurrentIndex(m_model->index(0, 0));
    connect(m_view, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(copyToClipboard()));

    // setup signals & slots
    QAction* action;
    action = createAction(tr("Copy to Clipboard (Ctrl+C)"), QIcon(":/icons/copy"));
    action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
    connect(action, SIGNAL(triggered()), this, SLOT(copyToClipboard()));

    action = createAction(tr("Copy password only (Ctrl+Shift+C)"), QIcon(":/icons/copy_pwd"));
    action->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_C));
    connect(action, SIGNAL(triggered()), this, SLOT(copyPasswordToClipboard()));

    m_mainWindow->addActionIntoToolbar(nullptr);

    action = createAction(tr("Add Password (Ctrl+N)"), QIcon(":/icons/add"));
    action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    connect(action, SIGNAL(triggered()), this, SLOT(addPassword()));

    action = createAction(tr("Edit Password (F2)"), QIcon(":/icons/edit"));
    action->setShortcut(QKeySequence(Qt::Key_F2));
    connect(action, SIGNAL(triggered()), this, SLOT(editPassword()));

    action = createAction(tr("Remove Password (Ctrl+Delete)"), QIcon(":/icons/delete"));
    action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Delete));
    connect(action, SIGNAL(triggered()), this, SLOT(deletePassword()));

    m_mainWindow->addActionIntoToolbar(nullptr);

    m_expireAction = createAction(tr("Reset master password (F12)"), QIcon(":/icons/forget_pwd"));
    action->setShortcut(QKeySequence(Qt::Key_F12));
    connect(m_expireAction, SIGNAL(triggered()), this, SLOT(clearGlobalPassword()));
}

QAction* Actions::createAction(const QString& name, const QIcon& icon)
{
    auto action = new QAction(icon, name, m_mainWindow);
    m_mainWindow->addActionIntoToolbar(action);
    return action;
}

void Actions::copyToClipboard()
{
    PasswordRecord record;
    if (m_model->getRecord(m_view->currentIndex(), record)) {
        auto crypto = m_cryptoFactory->getCrypto();
        if (!crypto)
            return;
        QString decrypted;
        crypto->decrypt(record.password, decrypted);
        SecureClipboard::instance().setContent(decrypted);
        crypto->eraseString(decrypted);
        m_mainWindow->toggleWindow();
    }
}

void Actions::copyPasswordToClipboard()
{
    PasswordRecord record;
    if (m_model->getRecord(m_view->currentIndex(), record)) {
        auto crypto = m_cryptoFactory->getCrypto();
        if (!crypto)
            return;
        QString decrypted;
        crypto->decrypt(record.password, decrypted);
        QString password = decrypted.mid(decrypted.lastIndexOf('\n') + 1);
        SecureClipboard::instance().setContent(password);
        crypto->eraseString(decrypted);
        crypto->eraseString(password);
        m_mainWindow->toggleWindow();
    }
}

void Actions::clipboardPasted()
{
    SecureClipboard::instance().contentPasted();
}

void Actions::addPassword()
{
    auto crypto = m_cryptoFactory->getCrypto();
    if (!crypto) // check if Crypto API is accessible
        return;
    PasswordRecord record;
    PasswordEditDialog dialog(tr("New password"), move(crypto));
    dialog.setPasswordRecord(record);
    if (dialog.exec() == QDialog::Rejected)
        return;
    record = dialog.passwordRecord();
    if (m_model->hasRecord(record.name)) {
        auto userAnswer = QMessageBox::question(m_mainWindow, tr("Confirm")
            , tr("Password \"%1\" already exists. Overwrite it?").arg(record.name));
        if (userAnswer != QMessageBox::Yes)
            return;
    }
    if (m_model->addOrSetRecord(record))
        m_view->setCurrentIndex(m_model->getIndexByName(record.name));
}

void Actions::editPassword()
{
    if (!m_model->isValid(m_view->currentIndex()))
        return;
    PasswordRecord record;
    if (m_model->getRecord(m_view->currentIndex(), record)) {
        auto crypto = m_cryptoFactory->getCrypto();
        if (!crypto) // check if Crypto API is accessible
            return;
        PasswordEditDialog dialog(tr("Edit password"), move(crypto));
        dialog.setPasswordRecord(record);
        dialog.setNameReadOnly(true);
        if (dialog.exec() == QDialog::Rejected)
            return;
        record = dialog.passwordRecord();
        if (m_model->addOrSetRecord(record))
            m_view->setCurrentIndex(m_model->getIndexByName(record.name));
    }
}

void Actions::deletePassword()
{
    auto currentIndex = m_view->currentIndex();
    m_model->removeRecord(currentIndex);
    m_view->setCurrentIndex(m_model->fixIndex(currentIndex));
}

void Actions::clearGlobalPassword()
{
    m_cryptoStatus->clearPassword();
}

void Actions::setCryptoStatus(std::shared_ptr<yaps::CryptoStatus> cryptoStatus)
{
    m_cryptoStatus = move(cryptoStatus);
}

void Actions::updateCryptoStatus()
{
    m_expireAction->setEnabled(m_cryptoStatus->hasPassword());
}

void Actions::setCryptoFactory(std::shared_ptr<yaps::CryptoFactory> cryptoFactory)
{
    m_cryptoFactory = move(cryptoFactory);
}
