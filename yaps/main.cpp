#include <QApplication>
#include <QScopedPointer>
#include <QSharedMemory>
#include <QAbstractItemView>
#include <QFile>
#include <QTranslator>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonObject>

#include "mainwindow.h"
#include "MainActions.h"
#include "Database.h"
//#include "Crypto.h"

#include "crypto_engine_impl.h"
#include "crypto_factory_impl.h"
#include "password_prompt_impl.h"
#include "timer_impl.h"

#define YAPS_ID "YAPS-6d049d5f-2a4a-4910-8713-249dacbbd700"

static QSharedMemory* createSharedMemory();
static void setupTranslations();
static QJsonDocument loadConfig();
static QString getDatabasePath(const QJsonDocument&);

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    QScopedPointer<QSharedMemory> singleAppGuard(createSharedMemory());
    if (!singleAppGuard)
        return 0;
    application.setApplicationName("YAPS");
    application.setWindowIcon(QIcon(":/icons/app"));
    setupTranslations();

    QJsonDocument config = loadConfig();
    if (!setupDatabase(getDatabasePath(config)))
        return 1;

    MainWindow mainWindow; // MainWindow should be created before first call Actions::instance

    auto actions = Actions::instance();
    auto cryptoFactory = std::make_shared<yaps::CryptoFactoryImpl>(
        std::make_shared<yaps::CryptoEngineImpl>(),
        std::make_shared<yaps::PasswordPromptImpl>(),
        std::make_shared<yaps::TimerImpl>(),
        actions
    );

    actions->initialize();
    actions->setCryptoFactory(cryptoFactory);
    actions->setCryptoStatus(cryptoFactory);

    mainWindow.setMainWidget(actions->view());
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

static QJsonDocument loadConfig()
{
    QJsonDocument config;

    QFile configFile("yaps.json");
    if (configFile.open(QIODevice::ReadOnly))
        config = QJsonDocument::fromJson(configFile.readAll());

    return config;
}

static QString getDatabasePath(const QJsonDocument& config)
{
    QString path = "yaps.db";
    if (!config.isObject())
        return path;

    auto root = config.object();
    if (root.contains("database"))
        path = root.value("database").toString(path);

    return path;
}
