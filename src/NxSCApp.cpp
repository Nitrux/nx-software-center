#include "NxSCApp.h"
#include "../nx_sc_version.h"

// libraries
#include <KLocalizedString>
#include <QCommandLineParser>
#include <QDate>
#include <QIcon>
#include <QQmlContext>
#include <thumbnailer.h>

// local
#include "ResponseDTO/category.h"
#include "mauiapp.h"
#include "models/ApplicationsRegistryModel.h"
#include "models/app.h"
#include "models/appsmodel.h"
#include "models/categoriesmodel.h"
#include "models/package.h"
#include "models/progressmanager.h"
#include "models/storemodel.h"
#include "nx.h"

NxSCApp::NxSCApp(int &argc, char **argv)
    : QGuiApplication(argc, argv)
    , _qml_main(QStringLiteral("qrc:/main.qml"))
    , _taskManager(this)
    , _applicationsRegistry({NX::AppsPath.toLocalFile()})
    , _applicationsRegistryModel(&_applicationsRegistry, this)
{
    setAttribute(Qt::AA_EnableHighDpiScaling);
    setAttribute(Qt::AA_UseHighDpiPixmaps, true);

    setOrganizationName(QStringLiteral("Nitrux"));
    setWindowIcon(QIcon(":/nx-software-center.svg"));
    MauiApp::instance()->setIconName("qrc:/nx-software-center.svg");

    setKDEApplicationData();
}

void NxSCApp::setKDEApplicationData()
{
    KLocalizedString::setApplicationDomain("nx-software-center");
    _aboutData.setComponentName(QStringLiteral("nx-software-center"));
    _aboutData.setDisplayName(i18n("NX Software Center"));
    _aboutData.setVersion(NX_SC_VERSION_STRING);
    _aboutData.setShortDescription(i18n("Browse and install AppImages."));
    _aboutData.setLicense(KAboutLicense::LGPL_V3);
    _aboutData.setCopyrightStatement(i18n("© 2019-%1 Nitrux Development Team", QString::number(QDate::currentDate().year())));
    _aboutData.setOtherText(QString(GIT_BRANCH) + "/" + QString(GIT_COMMIT_HASH));

    _aboutData.addAuthor(i18n("Camilo Higuita"), i18n("Developer"), QStringLiteral("milo.h@aol.com"));
    _aboutData.addAuthor(i18n("Anupam Basak"), i18n("Developer"), QStringLiteral("anupam.basak27@gmail.com"));
    _aboutData.setHomepage("https://nxos.org");
    _aboutData.setProductName("nitrux/nx-software-center");
    _aboutData.setBugAddress("https://github.com/nitrux/issues");
    _aboutData.setOrganizationDomain("org.nx.softwarecenter");
    _aboutData.setProgramLogo(windowIcon());

    KAboutData::setApplicationData(_aboutData);
}
void NxSCApp::parseCommands()
{
    QCommandLineParser parser;
    parser.process(arguments());

    _aboutData.setupCommandLine(&parser);
    _aboutData.processCommandLine(&parser);
}
void NxSCApp::setupQMLEngine()
{
    QObject::connect(&_engine, &QQmlApplicationEngine::objectCreated, this, &NxSCApp::onQMLEngineObjectCreated, Qt::QueuedConnection);

    qmlRegisterType<App>("NXModels", 1, 0, "App");
    qmlRegisterType<AppsModel>("NXModels", 1, 0, "Apps");
    qmlRegisterType<ProgressManager>("NXModels", 1, 0, "ProgressManager");
    qmlRegisterAnonymousType<Package>("NXModels", 1);
    qmlRegisterAnonymousType<Application>("NXModels", 1);
    qmlRegisterType<Category>("NXModels", 1, 0, "Category");
    qmlRegisterType<StoreModel>("NXModels", 1, 0, "Store");
    qmlRegisterType<CategoriesModel>("NXModels", 1, 0, "Categories");

    QQmlContext *rootContext = _engine.rootContext();
    rootContext->setContextProperty("taskManagerCtx", &_taskManager);
    rootContext->setContextProperty("applicationsRegistry", &_applicationsRegistryModel);

    qmlRegisterUncreatableType<Task>("NXModels", 1, 0, "Task", "Tasks can only be created from the Task Manager");

    auto thumbnailer = new Thumbnailer();
    _engine.addImageProvider("thumbnailer", thumbnailer);

    _engine.load(_qml_main);
}
void NxSCApp::onQMLEngineObjectCreated(QObject *obj, const QUrl &objUrl)
{
    // terminate application in case of errors
    if (!obj && _qml_main == objUrl)
        QCoreApplication::exit(-1);
}
void NxSCApp::setupApplicationsRegistry()
{
    qRegisterMetaType<ApplicationData>("ApplicationData");
    qRegisterMetaType<ApplicationBundle>("ApplicationBundle");

    _bundleDirsWatcher = QPointer<BundlesDirsWatcher>(new BundlesDirsWatcher(_applicationsRegistry.getAppDirs()));
    connect(_bundleDirsWatcher.data(), &BundlesDirsWatcher::bundleAdded, &_applicationsRegistry, &ApplicationsRegistry::addBundle);
    connect(_bundleDirsWatcher.data(), &BundlesDirsWatcher::bundleUpdated, &_applicationsRegistry, &ApplicationsRegistry::addBundle);
    connect(_bundleDirsWatcher.data(), &BundlesDirsWatcher::bundleRemoved, &_applicationsRegistry, &ApplicationsRegistry::removeBundleByPath);

    // run watcher in a different thread to avoid affecting UI performance
    _bundleDirsWatcher->moveToThread(&_bundleDirsWatcherThread);
    _bundleDirsWatcherThread.start();
    QMetaObject::invokeMethod(_bundleDirsWatcher.data(), &BundlesDirsWatcher::checkAllDirs, Qt::QueuedConnection);
}
NxSCApp::~NxSCApp()
{
    _bundleDirsWatcherThread.exit();
    _bundleDirsWatcherThread.wait(10);
}
