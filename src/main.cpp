#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QDebug>
#include <QIcon>
#include <QtQml>
#include <QQmlEngine>
#include <QLocalSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QQmlNetworkAccessManagerFactory>

#include <KAuth/KAuthExecuteJob>

#include <Snapd/App>
#include "snapdclientkauthwrapper.h"
#include "snapdsettings.h"

#include "snapstore/customnetworkaccessmanagerfactory.h"
#include "snapstore/snapstore.h"
#include "snapstore/snapstorerequest.h"
#include "snapstore/snapstorelistdepartamentsrequest.h"
#include "snapstore/snapstoregetdepartamentrequest.h"
#include "snapstore/snapstoresnapdetailsrequest.h"

#include "entities/simplefileregistry.h"

#include "gateways/appimagehubrepository.h"
#include "gateways/kf5downloadmanager.h"

#include "ui/searchviewcontroller.h"
#include "ui/taskscontroller.h"
#include "ui/taskcontroller.h"
#include "ui/registrycontroller.h"


static SnapdSettings * snapdSettings;
static SnapStore * snapStore;

static QObject *snapdkauthwrapper_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return new SnapdClientKAuthWrapper();
}

static QObject *snapdsetings_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return snapdSettings;
}

static QObject *snapstore_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return snapStore;
}

Registry * registry = nullptr;
AppImageHubRepository *repository = nullptr;
KF5DownloadManager *downloadManager = nullptr;

SearchViewController *searchviewcontroller = nullptr;
TasksController * tasksController = nullptr;
RegistryController * registryController = nullptr;

static QObject *searchviewcontroller_singletontype_provider(QQmlEngine *engine, QJSEngine *)
{
    if (searchviewcontroller == nullptr)
    {
        Q_ASSERT(repository != nullptr);
        QList<Repository*> repositoryList(QList<Repository*> {repository});
        searchviewcontroller = new SearchViewController(registry, repositoryList, engine);
    }

    return dynamic_cast<QObject*>(searchviewcontroller);
}

static QObject *taskscontroller_singletontype_provider(QQmlEngine *engine, QJSEngine *)
{
    if (tasksController == nullptr)
    {
        Q_ASSERT(repository != nullptr);
        QList<Repository*> repositoryList(QList<Repository*> {repository});
        tasksController = new TasksController(repositoryList, registry, downloadManager, engine);
    }

    return dynamic_cast<QObject*>(tasksController);
}

static QObject *registrycontroller_singletontype_provider(QQmlEngine *engine, QJSEngine *)
{
    if (registryController == nullptr)
    {
        Q_ASSERT(registry != nullptr);
        registryController = new RegistryController(registry, engine);
    }

    return dynamic_cast<QObject*>(registryController);
}

int main(int argc, char *argv[])
{
    const char * uri = "org.nx.softwarecenter";

    QGuiApplication app(argc, argv);
    QCoreApplication::addLibraryPath("./");

    QCoreApplication::setOrganizationName("NXOS");
    QCoreApplication::setOrganizationDomain("nxos.org");
    QCoreApplication::setApplicationName("nx-software-center");

    app.setWindowIcon(QIcon::fromTheme("nx-software-center"));
    QQmlApplicationEngine engine;

    // Init view controllers
    downloadManager = new KF5DownloadManager();
    registry = new SimpleFileRegistry();

    repository = new AppImageHubRepository("https://appimage.github.io/feed.json");
    repository->updateCache();

    qRegisterMetaType<TaskController::TaskState>("TaskState");
    qmlRegisterUncreatableType<TaskController>(uri, 1, 0, "Task", "Task can only be created by the TasksController");
    qmlRegisterSingletonType<SearchViewController>(uri, 1, 0, "SearchViewController", searchviewcontroller_singletontype_provider);
    qmlRegisterSingletonType<TasksController>(uri, 1, 0, "TasksController", taskscontroller_singletontype_provider);
    qmlRegisterSingletonType<RegistryController>(uri, 1, 0, "RegistryController", registrycontroller_singletontype_provider);

    // Snaps

    snapdSettings = new SnapdSettings();
    snapdSettings->load();

    CustomNetworkAccessManagerFactory networkAccessManagerFactory(snapdSettings);

    snapStore = new SnapStore(snapdSettings);
    snapStore->setNetworkAccessManagerFactory(&networkAccessManagerFactory);

    // HACK TO LOAD THE PLUGIN FROM ITS CUSTOM INSTALL PATH
    // FIXME: well, fix the glib-snapd-qt plugin install path
    engine.addImportPath(QStringLiteral("/usr/lib/qt5/qml/"));
    qDebug() << engine.importPathList();

    qmlRegisterSingletonType<SnapdClientKAuthWrapper>(uri, 1, 0, "SnapdRootClient", snapdkauthwrapper_singletontype_provider);
    qmlRegisterSingletonType<SnapdSettings>(uri, 1, 0, "SnapdSettings", snapdsetings_singletontype_provider);
    qmlRegisterUncreatableType<KAuth::ExecuteJob> (uri, 1, 0, "ExecuteJob", "Can't create");
    qmlRegisterUncreatableType<QSnapdApp>(uri, 1, 0, "QSnapdApp", "Can't create");


    qmlRegisterSingletonType<SnapStore>(uri, 1, 0, "SnapStore", snapstore_singletontype_provider);
    //    qmlRegisterUncreatableType<SnapStoreRequest> (uri, 1, 0,"SnapStoreRequest", "Can't create");
    qmlRegisterUncreatableType<SnapStoreListDepartamentsRequest>(uri, 1, 0, "SnapStoreListDepartmentsRequest", "Can't create");
    qmlRegisterUncreatableType<SnapStoreGetDepartamentRequest>(uri, 1, 0, "SnapStoreGetDepartamentSnapsRequest", "Can't create");
    qmlRegisterUncreatableType<SnapStoreSnapDetailsRequest>(uri, 1, 0, "SnapStoreSnapDetailsRequest", "Can't create");


    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    engine.setNetworkAccessManagerFactory(&networkAccessManagerFactory);

    return app.exec();
}


