#include "snapdhelper.h"

#include <QLocalSocket>
#include <QByteArray>
#include <QJsonDocument>

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
    qDebug() << "Uninstalling " << snap;

    QString path = "/v2/snaps/"+snap;
    QJsonDocument jsonRequset = QJsonDocument::fromJson("{\"action\": \"remove\"}");
    QString query = HTTPUtils::buildJSonPostRequest(path, jsonRequset);

    qDebug() << query;

    ActionReply reply;
    QByteArray rawHttpReply = sendRequest(query.toLocal8Bit(), reply);

    if (!reply.succeeded())
        return reply;


    QVariantMap retdata;
    retdata["contents"] = HTTPUtils::parseJSonResponse(rawHttpReply);

    reply.setData(retdata);
    return reply;
}

QByteArray SnapdHelper::sendRequest(QByteArray request, ActionReply reply)
{
    QByteArray rawReply;
    QLocalSocket *socket = new QLocalSocket();

    socket->connectToServer("/run/snapd.socket");
    QByteArray response ;

    if (socket->waitForConnected(1000))
        qDebug("Connected to socket!");
    else {
        reply = ActionReply::HelperErrorReply();
        reply.setErrorDescription(socket->errorString());

        delete socket;
        return rawReply;
    }


    qDebug() << "Writing !";
    socket->write(request);
    if (socket->waitForBytesWritten(5000))
        qDebug("Wrote!");
    else {
        reply = ActionReply::HelperErrorReply();
        reply.setErrorDescription(socket->errorString());

        delete socket;
        return rawReply;
    }

    if (socket->waitForReadyRead(5000))
        qDebug("Response ready!");
    else {
        reply = ActionReply::HelperErrorReply();
        reply.setErrorDescription(socket->errorString());

        delete socket;
        return rawReply;
    }


    rawReply = socket->readAll();

    socket->close();
    delete socket;

    return rawReply;
}


KAUTH_HELPER_MAIN("org.nomad.softwarecenter", SnapdHelper)
