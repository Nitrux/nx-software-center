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

#include <Snapd/Client>
#include <Snapd/Request>


using namespace KAuth;

SnapdHelper::SnapdHelper(QObject *parent) : QObject(parent), m_qsnapdClient(parent)
{
    auto request = m_qsnapdClient.connect();
    request->runSync();
    qDebug() << "Connecting to snapd, errors: " << request->errorString();
    request->deleteLater();
}

ActionReply SnapdHelper::remove(QVariantMap args)
{
    QString snap = args["snap"].toString();
    ActionReply reply;

    auto request = m_qsnapdClient.remove(snap);
    request->runSync();
    if (request->error() != QSnapdRequest::NoError) {
        reply.setErrorCode( ActionReply::BackendError );
        reply.setErrorDescription(request->errorString());
    }

    request->deleteLater();
    return reply;
}

ActionReply SnapdHelper::install(QVariantMap args)
{
    HelperSupport::progressStep(QVariantMap());

    QString snap = args["snap"].toString();
    QString channel = args.value("channel").toString();

    ActionReply reply;

    auto request = m_qsnapdClient.install(snap, channel);
    request->runSync();
    if (request->error() != QSnapdRequest::NoError) {
        reply.setErrorCode( ActionReply::BackendError );
        reply.setErrorDescription(request->errorString());
    }

    request->deleteLater();
    return reply;
}

ActionReply SnapdHelper::disable(QVariantMap args)
{
    QString snap = args["snap"].toString();
    ActionReply reply;

    auto request = m_qsnapdClient.disable(snap);
    request->runSync();
    if (request->error() != QSnapdRequest::NoError) {
        reply.setErrorCode( ActionReply::BackendError );
        reply.setErrorDescription(request->errorString());
    }

    request->deleteLater();
    return reply;
}

ActionReply SnapdHelper::enable(QVariantMap args)
{
    QString snap = args["snap"].toString();
    ActionReply reply;

    qDebug() << "Ok let's do it!";

    auto request = m_qsnapdClient.enable(snap);
    request->runSync();
    if (request->error() != QSnapdRequest::NoError) {
        reply.setErrorCode( ActionReply::BackendError );
        reply.setErrorDescription(request->errorString());
    }

    request->deleteLater();
    return reply;
}


KAUTH_HELPER_MAIN("org.nomad.softwarecenter", SnapdHelper)
