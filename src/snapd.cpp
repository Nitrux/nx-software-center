#include "snapd.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include <QLocalSocket>

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

QVariantList SnapD::list()
{
    QString query = httpUtils.buildSimpleJSonGetRequest("/v2/snaps");

    QVariantList result;
    QLocalSocket *socket = new QLocalSocket();

    socket->connectToServer("/run/snapd.socket");
    QByteArray response ;

    if (socket->waitForConnected(1000))
        qDebug("Connected to socket!");
    else
        return result;


    qDebug() << "Writing !";
    socket->write(query.toLocal8Bit());
    if (socket->waitForBytesWritten(1000))
        qDebug("Wrote!");
    else
        return result;


    if (socket->waitForReadyRead(1000))
        qDebug("Response ready!");
    else
        return result;

    QByteArray rawResponse = socket->readAll();

    socket->close();
    delete socket;


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
