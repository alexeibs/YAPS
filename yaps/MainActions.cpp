#include "MainActions.h"

#include <QAction>
#include "mainwindow.h"

Actions& Actions::instance()
{
    static Actions single;
    return single;
}

QAction* Actions::addAction(const QString& name, const QIcon& icon)
{
    if (!m_mainWindow)
        return nullptr;
    auto action = new QAction(icon, name, m_mainWindow);
    m_mainWindow->addActionIntoToolbar(action);
    return action;
}

void Actions::setMainWindow(MainWindow* window)
{
    m_mainWindow = window;
}

Actions::Actions()
    : m_mainWindow(nullptr)
{
}
