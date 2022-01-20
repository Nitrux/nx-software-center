#include "NXSCApp.h"
#include "../nx_sc_version.h"

// libraries
#include <KLocalizedString>
#include <MauiKit/FileBrowsing/thumbnailer.h>
#include <QCommandLineParser>
#include <QDate>
#include <QIcon>
#include <QQmlContext>

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

NXSCApp::NXSCApp(int &argc, char **argv)
    : QApplication(argc, argv)
    , _qml_main(QStringLiteral("qrc:/main.qml"))
    , _taskManager(this)
    , _appsDBHelper(_appsDBHelper->getInstance())
    , _applicationsRegistry({NX::AppsPath.toLocalFile()}, _appsDBHelper->getAppsMap())
    , _applicationsRegistryModel(&_applicationsRegistry, this)
    , _updateService(this)
{
    setOrganizationName(QStringLiteral("Nitrux"));
    setWindowIcon(QIcon(":/nx-software-center.svg"));
    MauiApp::instance()->setIconName("qrc:/nx-software-center.svg");

    QObject::connect(&_updateService, &UpdateService::updateFound, &_applicationsRegistryModel, &ApplicationsRegistryModel::handleUpdateInformation);
    setKDEApplicationData();
}

NXSCApp::~NXSCApp()
{
    _bundleDirsWatcherThread.exit();
    _bundleDirsWatcherThread.wait(10);
}
void NXSCApp::setKDEApplicationData()
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
void NXSCApp::parseCommands()
{
    QCommandLineParser parser;
    parser.process(arguments());

    _aboutData.setupCommandLine(&parser);
    _aboutData.processCommandLine(&parser);
}
void NXSCApp::setupQMLEngine()
{
    QObject::connect(&_engine, &QQmlApplicationEngine::objectCreated, this, &NXSCApp::onQMLEngineObjectCreated, Qt::QueuedConnection);

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
    qmlRegisterUncreatableType<Task>("NXModels", 1, 0, "Task", "Tasks can only be created from the Task Manager");

    registerApplicationsRegistryService();
    registerUpdateService();
    setupApplicationDBUpdateCache();

    registerThumbnailer();

    _engine.load(_qml_main);
}
void NXSCApp::registerThumbnailer()
{
    auto thumbnailer = new Thumbnailer();
    _engine.addImageProvider("thumbnailer", thumbnailer);
}
void NXSCApp::registerApplicationsRegistryService()
{
    _applicationsRegistryModelProxy.setSourceModel(&_applicationsRegistryModel);
    _applicationsRegistryModelProxy.setFilterRole(ApplicationsRegistryModel::Name);
    _applicationsRegistryModelProxy.setSortRole(ApplicationsRegistryModel::XdgCategories);

    qmlRegisterUncreatableType<ApplicationsRegistryModel>("org.maui.nxsc",
                                                          1,
                                                          0,
                                                          "ApplicationsListModelRoles",
                                                          "Registry can only be accessed by the singleton");
    qmlRegisterSingletonInstance("org.maui.nxsc", 1, 0, "ApplicationsListModel", &_applicationsRegistryModelProxy);
    qmlRegisterSingletonInstance("org.maui.nxsc", 1, 0, "ApplicationsRegistry", &_applicationsRegistry);
}
void NXSCApp::onQMLEngineObjectCreated(QObject *obj, const QUrl &objUrl)
{
    // terminate application in case of errors
    if (!obj && _qml_main == objUrl)
        QCoreApplication::exit(-1);
}
void NXSCApp::setupApplicationsRegistry()
{
    qRegisterMetaType<ApplicationData>("ApplicationData");
    qRegisterMetaType<ApplicationBundle>("ApplicationBundle");
    qRegisterMetaType<ApplicationsList>("ApplicationsList");

    _bundleDirsWatcher = QPointer<BundlesDirsWatcher>(new BundlesDirsWatcher(_applicationsRegistry.getAppDirs(), {}));
    connect(_bundleDirsWatcher.data(), &BundlesDirsWatcher::bundleAdded, &_applicationsRegistry, &ApplicationsRegistry::addBundle);
    connect(_bundleDirsWatcher.data(), &BundlesDirsWatcher::bundleUpdated, &_applicationsRegistry, &ApplicationsRegistry::addBundle);
    connect(_bundleDirsWatcher.data(), &BundlesDirsWatcher::bundleRemoved, &_applicationsRegistry, &ApplicationsRegistry::removeBundleByPath);

    // run watcher in a different thread to avoid affecting UI performance
    _bundleDirsWatcher->moveToThread(&_bundleDirsWatcherThread);
    _bundleDirsWatcherThread.start();
    QMetaObject::invokeMethod(_bundleDirsWatcher.data(), &BundlesDirsWatcher::checkAllDirs, Qt::QueuedConnection);
}

void NXSCApp::registerUpdateService()
{
    qmlRegisterSingletonInstance("org.maui.nxsc", 1, 0, "UpdateService", &_updateService);
}

void NXSCApp::setupApplicationDBUpdateCache()
{
    connect(&_applicationsRegistry, &ApplicationsRegistry::applicationAdded, _appsDBHelper, &AppsDBHelper::saveOrUpdateApp);
    connect(&_applicationsRegistry, &ApplicationsRegistry::applicationUpdated, _appsDBHelper, &AppsDBHelper::saveOrUpdateApp);
    connect(&_applicationsRegistry, &ApplicationsRegistry::applicationRemoved, _appsDBHelper, &AppsDBHelper::deleteApp);
}
