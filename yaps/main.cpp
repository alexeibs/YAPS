#include <QApplication>
#include <QScopedPointer>
#include <QSharedMemory>
#include <QFile>
#include <QTranslator>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonObject>

#include "yapslib/mainwindow.h"
#include "yapslib/Database.h"

#include "yapslib/clipboard_impl.h"
#include "yapslib/controller_impl.h"
#include "yapslib/crypto_engine_impl.h"
#include "yapslib/crypto_factory_impl.h"
#include "yapslib/message_box_factory_impl.h"
#include "yapslib/passwords_model_impl.h"
#include "yapslib/password_prompt_impl.h"
#include "yapslib/password_record_editor_impl.h"
#include "yapslib/secure_clipboard_impl.h"
#include "yapslib/timer_impl.h"

#define YAPS_ID "YAPS-6d049d5f-2a4a-4910-8713-249dacbbd700"

static QSharedMemory* createSharedMemory();
static void setupTranslations();
static QJsonDocument loadConfig();
static QString getDatabasePath(const QJsonDocument&);

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    Q_INIT_RESOURCE(main);
    QScopedPointer<QSharedMemory> singleAppGuard(createSharedMemory());
    if (!singleAppGuard)
        return 0;
    application.setApplicationName("YAPS");
    application.setWindowIcon(QIcon(":/icons/app"));
    setupTranslations();

    QJsonDocument config = loadConfig();
    if (!setupDatabase(getDatabasePath(config)))
        return 1;

    auto passwordsModel = std::make_shared<yaps::PasswordsModelImpl>();
    auto cryptoFactory = std::make_shared<yaps::CryptoFactoryImpl>(
        std::make_shared<yaps::CryptoEngineImpl>(),
        std::make_shared<yaps::PasswordPromptImpl>(),
        std::make_shared<yaps::TimerImpl>()
    );
    auto clipboard = std::make_shared<yaps::SecureClipboardImpl>(
        std::make_shared<yaps::ClipboardImpl>(),
        std::make_shared<yaps::TimerImpl>(),
        std::make_shared<yaps::TimerImpl>()
    );
    auto messageBoxFactory = std::make_shared<yaps::MessageBoxFactoryImpl>();
    auto controller = std::make_shared<yaps::ControllerImpl>(
        cryptoFactory,
        passwordsModel,
        clipboard,
        messageBoxFactory,
        std::make_shared<yaps::PasswordRecordEditorImpl>(clipboard, messageBoxFactory)
    );
    MainWindow mainWindow(passwordsModel.get(), controller);

    messageBoxFactory->setParentWindow(&mainWindow);
    controller->setViewState(&mainWindow);
    cryptoFactory->setCryptoStatusView(mainWindow.cryptoStatusView());

    passwordsModel.reset();
    cryptoFactory.reset();
    controller.reset();

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
