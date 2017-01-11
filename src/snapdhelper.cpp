#include "snapdhelper.h"

#include <QThread>
#include <QLocalSocket>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QTimer>

#include <KAuthHelperSupport>
#include <QDebug>

#include "httputils.h"

using namespace KAuth;

SnapdHelper::SnapdHelper(QObject *parent) : QObject(parent)
{
}

ActionReply SnapdHelper::remove(QVariantMap args)
{

    QString snap = args["snap"].toString();

    QString path = "/v2/snaps/"+snap;
    QJsonDocument jsonRequset = QJsonDocument::fromJson("{\"action\": \"remove\"}");
    QString query = HTTPUtils::buildJSonPostRequest(path, jsonRequset);

    qDebug() << query;

    ActionReply reply;
    QByteArray rawHttpReply = sendRequest(query.toLocal8Bit());

    if (!reply.succeeded())
        return reply;


    QJsonDocument retdata = HTTPUtils::parseJSonResponse(rawHttpReply);
    QJsonObject obj = retdata.object();
    //    qDebug() << "Returned JSON: " << obj;

    if (obj.contains("change")) {
        QJsonValue changeId = obj.value("change");
        spawnChangeMonitor(changeId.toString());
    }

    return reply;
}

ActionReply SnapdHelper::install(QVariantMap args)
{
    HelperSupport::progressStep(QVariantMap());

    QString snap = args["snap"].toString();
    //     qDebug() << "Installing " << snap;

    QString path = "/v2/snaps/"+snap;
    QJsonDocument jsonRequset = QJsonDocument::fromJson("{\"action\": \"install\"}");
    QString query = HTTPUtils::buildJSonPostRequest(path, jsonRequset);

    qDebug() << query;

    ActionReply reply;
    QByteArray rawHttpReply = sendRequest(query.toLocal8Bit());

    qDebug() << rawHttpReply;
    if (!reply.succeeded())
        return reply;


    QJsonDocument retdata = HTTPUtils::parseJSonResponse(rawHttpReply);
    QJsonObject obj = retdata.object();
    //    qDebug() << "Returned JSON: " << obj;

    if (obj.contains("change")) {
        QJsonValue changeId = obj.value("change");
        spawnChangeMonitor(changeId.toString());
    }

    return reply;
}

void SnapdHelper::spawnChangeMonitor(QString changeId)
{
    QString ready("true");
    do {
        // Fetch change
        QString chageFetchRequest = HTTPUtils::buildSimpleJSonGetRequest("/v2/changes/"+changeId);
        ActionReply changeReply;
        QByteArray rawHttpReply = sendRequest(chageFetchRequest.toLocal8Bit());

        QJsonDocument response = HTTPUtils::parseJSonResponse(rawHttpReply);
        QVariantMap change = response.toVariant().toMap();
        change = change.value("result").toMap();

        ready = change.value("ready").toString();


        qDebug() << "Reporting change: " << change;
        HelperSupport::progressStep(change);

        QThread::msleep(300);
    } while (ready.compare("true") != 0);
}

QByteArray SnapdHelper::sendRequest(QByteArray request)
{
    QByteArray rawReply;
    QLocalSocket *socket = new QLocalSocket();

    socket->connectToServer("/run/snapd.socket");
    QByteArray response ;

    if (!socket->waitForConnected(1000)) {
        qDebug() << socket->errorString();

        delete socket;
        return rawReply;
    }


    //    qDebug() << "Writing !";
    socket->write(request);
    if (!socket->waitForBytesWritten(1000)) {
        qDebug() << socket->errorString();

        delete socket;
        return rawReply;
    }

    if (!socket->waitForReadyRead(1000)) {
        qDebug() << socket->errorString();

        delete socket;
        return rawReply;
    }


    rawReply = socket->readAll();

    socket->close();
    delete socket;

    return rawReply;
}


KAUTH_HELPER_MAIN("org.nomad.softwarecenter", SnapdHelper)
