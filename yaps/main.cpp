#include <QApplication>
#include <QScopedPointer>
#include <QSharedMemory>

#include "mainwindow.h"
#include "MainActions.h"

#include <QTextEdit>

#define YAPS_ID "YAPS-6d049d5f-2a4a-4910-8713-249dacbbd700"

static QSharedMemory* createSharedMemory();
static void setupMainWindow(MainWindow& mainWindow);

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    QScopedPointer<QSharedMemory> singleAppGuard(createSharedMemory());
    if (!singleAppGuard)
        return 0;
    application.setApplicationName("YAPS");
    application.setWindowIcon(QIcon(":/icons/app"));

    MainWindow mainWindow;
    setupMainWindow(mainWindow);

    return application.exec();
}

QSharedMemory *createSharedMemory()
{
    QScopedPointer<QSharedMemory> shared(new QSharedMemory(YAPS_ID));
    if (shared->attach())
        return nullptr;
    if (!shared->create(1))
        return nullptr;
    return shared.take();
}

void setupMainWindow(MainWindow& mainWindow)
{
    auto& actions = Actions::instance();
    actions.addAction(QObject::tr("Add Password"), QIcon(":/icons/add"));
    actions.addAction(QObject::tr("Edit Password"), QIcon(":/icons/edit"));
    actions.addAction(QObject::tr("Remove Password"), QIcon(":/icons/delete"));

    mainWindow.setMainWidget(new QTextEdit(&mainWindow));
    mainWindow.toggleWindow();
}
