#include "NXSCApp.h"
#include "../nx_sc_version.h"

// libraries
#include <KI18n/KLocalizedString>
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
#include "models/categoriesmodel.h"
#include "models/storemodel.h"
#include "nx.h"

NXSCApp::NXSCApp(int &argc, char **argv)
    : QApplication(argc, argv)
    , _qml_main(QStringLiteral("qrc:/main.qml"))
    , _applicationsRegistry({NX::AppsPath.toLocalFile()})
    , _applicationsRegistryModel(this)
    , _updateService(this)
    , _installService(NX::AppsPath.toLocalFile(), this)
    , _deleteService(2, this)
    , _cacheService(QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).filePath(NX::appDBName))
    , _tasksListModel(this)
    , _bundleDirsWatcher(nullptr /* will be moved to another thread later */)
{
    setOrganizationName(QStringLiteral("Nitrux"));
    setWindowIcon(QIcon(":/nx-software-center.svg"));

    QObject::connect(&_updateService,
                     &UpdateService::applicationUpdateDataChanged,
                     &_applicationsRegistryModel,
                     &ApplicationsRegistryModel::handleUpdateInformation);
    QObject::connect(&_updateService, &UpdateService::progressNotification, &_tasksListModel, &TasksListModel::handleTaskUpdate);
    QObject::connect(&_updateService, &UpdateService::progressNotification, &_applicationsRegistryModel, &ApplicationsRegistryModel::handleTaskUpdate);

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
    MauiApp::instance()->setIconName("qrc:/nx-software-center.svg");
}
void NXSCApp::parseCommands()
{
    QCommandLineParser parser;
    parser.process(arguments());

    _aboutData.setupCommandLine(&parser);
    _aboutData.processCommandLine(&parser);
}
void NXSCApp::setup()
{
    QObject::connect(&_engine, &QQmlApplicationEngine::objectCreated, this, &NXSCApp::onQMLEngineObjectCreated, Qt::QueuedConnection);

    qRegisterMetaType<Application>("ApplicationData");
    qRegisterMetaType<ApplicationBundle>("ApplicationBundle");
    qRegisterMetaType<ApplicationsList>("ApplicationsList");

    qmlRegisterType<App>("NXModels", 1, 0, "App");
    qmlRegisterAnonymousType<PlingStoreApplicationData>("NXModels", 1);
    qmlRegisterType<Category>("NXModels", 1, 0, "Category");
    qmlRegisterType<StoreModel>("NXModels", 1, 0, "Store");
    qmlRegisterType<CategoriesModel>("NXModels", 1, 0, "Categories");

    // register services
    qmlRegisterSingletonInstance("org.maui.nxsc", 1, 0, "TasksModel", &_tasksListModel);

    qmlRegisterSingletonInstance("org.maui.nxsc", 1, 0, "LaunchService", &_launchService);

    qmlRegisterSingletonInstance("org.maui.nxsc", 1, 0, "InstallService", &_installService);
    QObject::connect(&_installService, &InstallService::progressNotification, &_tasksListModel, &TasksListModel::handleTaskUpdate);
    QObject::connect(&_installService, &InstallService::progressNotification, &_applicationsRegistryModel, &ApplicationsRegistryModel::handleTaskUpdate);
    QObject::connect(&_applicationsRegistry, &ApplicationsRegistry::applicationAdded, &_installService, &InstallService::handleApplicationAdded);
    QObject::connect(&_applicationsRegistry, &ApplicationsRegistry::applicationUpdated, &_installService, &InstallService::handleApplicationUpdated);
    QObject::connect(&_applicationsRegistry, &ApplicationsRegistry::applicationRemoved, &_installService, &InstallService::handleApplicationRemoved);

    qmlRegisterSingletonInstance("org.maui.nxsc", 1, 0, "DeleteService", &_deleteService);
    QObject::connect(&_applicationsRegistry, &ApplicationsRegistry::applicationUpdated, &_deleteService, &DeleteService::onApplicationUpdated);

    _engine.rootContext()->setContextObject(new KLocalizedContext(&_engine));

    registerUpdateService();
    setupCacheService();
    setupBundlesDirsWatcher();
    registerApplicationsRegistryService();

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
    _applicationsRegistryModel.setRegistry(&_applicationsRegistry);

    _applicationsRegistryModelProxy.setSourceModel(&_applicationsRegistryModel);
    _applicationsRegistryModelProxy.setFilterRole(ApplicationsRegistryModel::Name);
    _applicationsRegistryModelProxy.setSortRole(ApplicationsRegistryModel::XdgCategories);

    qmlRegisterUncreatableType<ApplicationsRegistryModel>("org.maui.nxsc", 1, 0, "ApplicationModelRole", "Registry can only be accessed by the singleton");
    qmlRegisterSingletonInstance("org.maui.nxsc", 1, 0, "ApplicationsListModel", &_applicationsRegistryModelProxy);
    qmlRegisterSingletonInstance("org.maui.nxsc", 1, 0, "ApplicationsRegistry", &_applicationsRegistry);
}
void NXSCApp::onQMLEngineObjectCreated(QObject *obj, const QUrl &objUrl)
{
    // terminate application in case of errors
    if (!obj && _qml_main == objUrl)
        QCoreApplication::exit(-1);
}

void NXSCApp::setupBundlesDirsWatcher()
{
    connect(&_bundleDirsWatcher, &BundlesDirsWatcher::bundleAdded, &_applicationsRegistry, &ApplicationsRegistry::addBundle);
    connect(&_bundleDirsWatcher, &BundlesDirsWatcher::bundleUpdated, &_applicationsRegistry, &ApplicationsRegistry::addBundle);
    connect(&_bundleDirsWatcher, &BundlesDirsWatcher::bundleRemoved, &_applicationsRegistry, &ApplicationsRegistry::removeBundleByPath);

    _bundleDirsWatcher.watchPaths({NX::AppsPath.toLocalFile()});

    // run watcher in a different thread to avoid affecting UI performance
    _bundleDirsWatcher.moveToThread(&_bundleDirsWatcherThread);
    _bundleDirsWatcherThread.start();

    QMetaObject::invokeMethod(&_bundleDirsWatcher, &BundlesDirsWatcher::checkAllDirs, Qt::QueuedConnection);
}

void NXSCApp::registerUpdateService()
{
    qmlRegisterSingletonInstance("org.maui.nxsc", 1, 0, "UpdateService", &_updateService);
}

void NXSCApp::setupCacheService()
{
    connect(&_applicationsRegistry, &ApplicationsRegistry::applicationAdded, &_cacheService, &CacheService::saveApplication);
    connect(&_applicationsRegistry, &ApplicationsRegistry::applicationUpdated, &_cacheService, &CacheService::saveApplication);
    connect(&_applicationsRegistry, &ApplicationsRegistry::applicationRemoved, &_cacheService, &CacheService::removeApplication);

    // init registry cache
    auto applicationsCache = _cacheService.listApplications();
    _applicationsRegistry.setApplications(applicationsCache);

    // init dir watcher cache
    QMap<QString, QDateTime> fileCache;
    for (const auto &app : applicationsCache) {
        for (const auto &bundle : app.getBundles()) {
            fileCache[bundle.path] = bundle.lastModified;
        }
    }
    _bundleDirsWatcher.setFileCache(fileCache);
}
