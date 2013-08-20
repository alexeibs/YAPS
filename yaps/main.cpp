#include <QApplication>
#include <QScopedPointer>
#include <QSharedMemory>
#include <QAbstractItemView>

#include "mainwindow.h"
#include "MainActions.h"
#include "Database.h"

#define YAPS_ID "YAPS-6d049d5f-2a4a-4910-8713-249dacbbd700"

static QSharedMemory* createSharedMemory();

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    QScopedPointer<QSharedMemory> singleAppGuard(createSharedMemory());
    if (!singleAppGuard)
        return 0;
    application.setApplicationName("YAPS");
    application.setWindowIcon(QIcon(":/icons/app"));

    if (!setupDatabase())
        return 1;

    MainWindow mainWindow; // MainWindow should be created before first call Actions::instance

    auto& actions = Actions::instance();
    actions.initialize();

    mainWindow.setMainWidget(actions.view());
    mainWindow.toggleWindow();

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
