#include "snapd.h"

#include <QtWidgets/QDialog>
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
}

SnapD *SnapD::instance()
{
    if (singleton == NULL)
        singleton = new SnapD();

    return singleton;
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

//bool SnapD::login(QString username, QString password, QString third)
//{
//    QString path = "/v2/snaps/"+snap;
//    QJsonDocument jsonRequset = QJsonDocument::fromJson("{\"action\": \"install\"}");
//}

void SnapD::remove(QString snap)
{

    KAuth::Action removeAction("org.nomad.softwarecenter.remove");
    removeAction.setHelperId("org.nomad.softwarecenter");
    removeAction.addArgument("snap", snap);


    KAuth::ExecuteJob *job = removeAction.execute();
    connect(job, &KAuth::ExecuteJob::result, [=] (KJob *kjob) {
        auto job = qobject_cast<KAuth::ExecuteJob *>(kjob);
        qDebug() << job->errorString() << job->error() << job->data() << removeAction.status();

//        qDebug() << removeAction.status();
//        qDebug() << "is valid" << removeAction.isValid() << "has helper" << removeAction.hasHelper() << "Action status" << job->action().status();
//        if (reply.failed()) {
//           qDebug() << QString("KAuth returned an error code: %1").arg(reply.errorCode()) << reply.errorDescription();
//           // QMessageBox::information(this, "Error", QString("KAuth returned an error code: %1").arg(reply.errorCode()));
//        } else {
//            qDebug() << reply.data();
//        }
    });
    job->start();


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
