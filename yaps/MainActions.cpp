#include "MainActions.h"

#include <QAction>
#include <QListView>
#include "mainwindow.h"
#include "PasswordsModel.h"
#include "Database.h"
#include "View.h"
#include "SecureClipboard.h"

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
    m_view = makeView(m_mainWindow, m_model, m_model->nameColumnIndex());

    // setup signals & slots
    QAction* action;
    action = createAction(QObject::tr("Copy to Clipboard"), QIcon(":/icons/copy"));
    connect(action, SIGNAL(triggered()), this, SLOT(copyToClipboard()));
    m_mainWindow->addActionIntoToolbar(nullptr);

    action = createAction(QObject::tr("Add Password"), QIcon(":/icons/add"));
    connect(action, SIGNAL(triggered()), this, SLOT(addPassword()));

    action = createAction(QObject::tr("Edit Password"), QIcon(":/icons/edit"));
    connect(action, SIGNAL(triggered()), this, SLOT(editPassword()));

    action = createAction(QObject::tr("Remove Password"), QIcon(":/icons/delete"));
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
    QMessageBox::information(0, "info", "copy");
    SecureClipboard::instance().setText("Testing SecureClipboard...");
}

void Actions::addPassword()
{
    QMessageBox::information(0, "info", "add");
}

void Actions::editPassword()
{
    QMessageBox::information(0, "info", "edit");
}

void Actions::deletePassword()
{
    QMessageBox::information(0, "info", "delete");
}
