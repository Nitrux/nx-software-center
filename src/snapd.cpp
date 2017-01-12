#include "snapd.h"

#include <QTimer>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include <QLocalSocket>

#include <KAuth>
#include <kauthaction.h>

#include "httputils.h"

SnapD *SnapD::singleton = NULL;

SnapD::SnapD(QObject *parent) : QObject(parent)
{
    m_changes = new Changes();
    m_local = new LocalSnaps();
    m_store = new SnapStore();

}


SnapD *SnapD::instance()
{
    if (singleton == NULL)
        singleton = new SnapD();

    return singleton;
}

Changes *SnapD::changes() const
{
    return m_changes;
}

LocalSnaps *SnapD::local() const
{
    return m_local;
}

SnapStore *SnapD::store() const
{
    return m_store;
}


QVariantList SnapD::snaps()
{
    QString query = httpUtils.buildSimpleJSonGetRequest("/v2/snaps");

    QVariantList result;
    QByteArray rawResponse = localQuery(query.toLocal8Bit());


    QJsonDocument jsonResponse = httpUtils.parseJSonResponse(rawResponse);
    QJsonObject object = jsonResponse.object();
    if (object.contains("result")) {
        QJsonValue resultObject = object.value("result");
        if (resultObject.isArray()) {
            QJsonArray resultArray = resultObject.toArray();
            return resultArray.toVariantList();
        }
    }

    return result;
}

QVariantList SnapD::find(QString query)
{
    QString path = QString("/v2/find?q=%1").arg(query);
    QString request = httpUtils.buildSimpleJSonGetRequest(path);

    QVariantList result;
    QByteArray rawResponse = localQuery(request.toLocal8Bit());


    QJsonDocument jsonResponse = httpUtils.parseJSonResponse(rawResponse);
    QJsonObject object = jsonResponse.object();
    if (object.contains("result")) {
        QJsonValue resultObject = object.value("result");
        if (resultObject.isArray()) {
            QJsonArray resultArray = resultObject.toArray();
            return resultArray.toVariantList();
        }
    }

    return result;
}

//bool SnapD::login(QString username, QString password, QString third)
//{
//    QString path = "/v2/snaps/"+snap;
//    QJsonDocument jsonRequset = QJsonDocument::fromJson("{\"action\": \"install\"}");
//}

KAuth::ExecuteJob *SnapD::remove(QString snap)
{

    KAuth::Action removeAction("org.nomad.softwarecenter.remove");
    removeAction.setHelperId("org.nomad.softwarecenter");
    removeAction.addArgument("snap", snap);

    KAuth::ExecuteJob *job = removeAction.execute();
    connect(job, &KAuth::ExecuteJob::newData, m_changes, &Changes::updateChanges);
    connect(job, &KAuth::ExecuteJob::result, [=] (KJob *kjob) {
        auto job = qobject_cast<KAuth::ExecuteJob *>(kjob);
        qDebug() << QString("Uninstall snap %1 finished, errors ? ").arg(snap) << job->errorString() << job->error();
        job->deleteLater();
    });

    job->start();
    return job;
}

KAuth::ExecuteJob *SnapD::install(QString snap)
{
    KAuth::Action installAction("org.nomad.softwarecenter.install");
    installAction.setHelperId("org.nomad.softwarecenter");
    installAction.addArgument("snap", snap);


    KAuth::ExecuteJob *job = installAction.execute();
    connect(job, &KAuth::ExecuteJob::newData, m_changes, &Changes::updateChanges);
    connect(job, &KAuth::ExecuteJob::result, [=] (KJob *kjob) {
        auto job = qobject_cast<KAuth::ExecuteJob *>(kjob);
        qDebug() << job->errorString() << job->error() << job->data() << installAction.status();

    });
    job->start();
    return job;
}

QByteArray SnapD::localQuery(QByteArray query)
{

    QByteArray result;
    QLocalSocket *socket = new QLocalSocket();

    socket->connectToServer("/run/snapd.socket");
    QByteArray response ;

    if (!socket->waitForConnected(1000))
        return result;


    socket->write(query);
    if (!socket->waitForBytesWritten(5000))
        return result;


    if (!socket->waitForReadyRead(5000))
        return result;

    result = socket->readAll();

    socket->close();
    delete socket;

    return result;
}
