#include "MainActions.h"

#include <QAction>
#include <QListView>
#include "mainwindow.h"
#include "PasswordsModel.h"
#include "Database.h"
#include "View.h"
#include "SecureClipboard.h"
#include "Crypto.h"
#include "PasswordEditDialog.h"

#include <QMessageBox>

Actions& Actions::instance()
{
    static Actions single;
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
        auto& crypto = Crypto::instance();
        QString decrypted;
        crypto.decrypt(record.password, decrypted);
        SecureClipboard::instance().setText(decrypted);
        crypto.erase(decrypted);
        m_mainWindow->toggleWindow();
    }
}

void Actions::addPassword()
{
    PasswordRecord record;
    PasswordEditDialog dialog(tr("New password"), record);
    if (dialog.exec() == QDialog::Rejected)
        return;
    if (m_model->hasRecord(record.name)) {
        auto userAnswer = QMessageBox::question(m_mainWindow, tr("Confirm")
            , tr("Password \"%1\" already exists. Overwrite it?").arg(record.name));
        if (userAnswer != QMessageBox::Yes)
            return;
    }
    auto lastIndex = m_view->currentIndex();
    if (m_model->addOrSetRecord(record))
        m_view->setCurrentIndex(lastIndex);
}

void Actions::editPassword()
{
    auto lastIndex = m_view->currentIndex();
    if (!m_model->isValid(lastIndex))
        return;
    PasswordRecord record;
    if (m_model->getRecord(m_view->currentIndex(), record)) {
        PasswordEditDialog dialog(tr("Edit password"), record);
        dialog.setNameReadOnly(true);
        if (dialog.exec() == QDialog::Rejected)
            return;
        if (m_model->addOrSetRecord(record))
            m_view->setCurrentIndex(lastIndex);
    }
}

void Actions::deletePassword()
{
    auto lastIndex = m_view->currentIndex();
    m_model->removeRecord(m_view->currentIndex());
    m_view->setCurrentIndex(lastIndex);
}
