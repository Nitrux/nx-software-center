#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QDebug>
#include <QLocalSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QQmlNetworkAccessManagerFactory>

#include <KAuth/KAuthExecuteJob>

#include <Snapd/App>
#include "snapdclientkauthwrapper.h"
#include "snapdsettings.h"

#include "snapstore/snapstore.h"
#include "snapstore/snapstorerequest.h"
#include "snapstore/snapstorelistdepartamentsrequest.h"
#include "snapstore/snapstoregetdepartamentrequest.h"
#include "snapstore/snapstoresnapdetailsrequest.h"

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

class MyNetworkAccessManagerFactory : public QQmlNetworkAccessManagerFactory
{
public:
    MyNetworkAccessManagerFactory(SnapdSettings *settings) : QQmlNetworkAccessManagerFactory() {
        m_settings = settings;
    }
    ~MyNetworkAccessManagerFactory() {
    }

    virtual QNetworkAccessManager *create(QObject *parent) {
        QNetworkAccessManager *nam = new QNetworkAccessManager(parent);
        if (m_settings->useProxy()) {
            QString proxyHost = m_settings->httpProxy();
            int32_t proxyPort = m_settings->httpProxyPort();

            qDebug() << "Created QNetworkAccessManager using proxy" << (proxyHost + ":" + QString::number(proxyPort));
            QNetworkProxy proxy(QNetworkProxy::HttpProxy, proxyHost, proxyPort);
            nam->setProxy(proxy);
        }

        return nam;
    }

private:
    SnapdSettings * m_settings;
};


int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);
    QCoreApplication::addLibraryPath("./");

    QQmlApplicationEngine engine;

    snapdSettings = new SnapdSettings();
    snapdSettings->load();


    snapStore = new SnapStore(snapdSettings);

    // HACK TO LOAD THE PLUGIN FROM ITS CUSTOM INSTALL PATH
    // FIXME: well, fix the glib-snapd-qt plugin install path
    engine.addImportPath(QStringLiteral("/usr/lib/qt5/qml/"));
    qDebug() << engine.importPathList();



    const char * uri = "org.nx.softwarecenter";

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
    engine.setNetworkAccessManagerFactory(new MyNetworkAccessManagerFactory(snapdSettings));
    return app.exec();

}


