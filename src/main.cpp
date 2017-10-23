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

#include "appimage/appimage.h"
#include "appimage/appimagehubrepository.h"

static AppImageHubRepository * appImageHubRepository = nullptr;
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


static QObject *appimagehub_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    if (appImageHubRepository == nullptr)
        appImageHubRepository = new AppImageHubRepository("http://localhost:4000/feed.json");

    return appImageHubRepository;
}


int main(int argc, char *argv[])
{
    const char * uri = "org.nx.softwarecenter";

    QGuiApplication app(argc, argv);
    QCoreApplication::addLibraryPath("./");

    app.setWindowIcon(QIcon::fromTheme("nx-software-center"));
    QQmlApplicationEngine engine;

    // App Images
    qmlRegisterUncreatableType<AppImage>(uri, 1, 0, "AppImage", "Can't create");
    qmlRegisterSingletonType<AppImageHubRepository>(uri, 1, 0, "AppImageHubRepository", appimagehub_singletontype_provider);

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


