#include <QApplication>
#include <QScopedPointer>
#include <QSharedMemory>
#include <QAbstractItemView>
#include <QFile>
#include <QTranslator>

#include "mainwindow.h"
#include "MainActions.h"
#include "Database.h"
#include "Crypto.h"

#define YAPS_ID "YAPS-6d049d5f-2a4a-4910-8713-249dacbbd700"

static QSharedMemory* createSharedMemory();
static void setupTranslations();

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    QScopedPointer<QSharedMemory> singleAppGuard(createSharedMemory());
    if (!singleAppGuard)
        return 0;
    application.setApplicationName("YAPS");
    application.setWindowIcon(QIcon(":/icons/app"));
    setupTranslations();

    if (!setupDatabase())
        return 1;
    if (!Crypto::instance().getGlobalPassword())
        return 2;

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

static void loadTranslation(const QString& translation)
{
    if (QFile::exists(translation)) {
        QTranslator* translator = new QTranslator;
        translator->load(translation);
        QApplication::installTranslator(translator);
    }
}

void setupTranslations()
{
    QString locale = QLocale::system().name();
    loadTranslation(":/tr/" + QLocale::system().name() + ".qm");
    loadTranslation(":/tr/qtbase_" + QLocale::system().name() + ".qm");
}
