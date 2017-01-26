#include "snapdhelper.h"

#include <QSettings>
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

    // Check for cancel requests
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [timer, request]() {
        if (HelperSupport::isStopped()) {
            qDebug() << "Stop request recived";
            request->cancel();
            timer->deleteLater();
        }
    });

    timer->start(200);

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

    // Check for cancel requests
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [timer, request]() {
        if (HelperSupport::isStopped()) {
            qDebug() << "Stop request recived";
            request->cancel();
            timer->deleteLater();
        }
    });
    timer->start(200);

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

    // Check for cancel requests
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [timer, request]() {
        if (HelperSupport::isStopped()) {
            qDebug() << "Stop request recived";
            request->cancel();
            timer->deleteLater();
        }

    });
    timer->start(200);

    request->deleteLater();
    return reply;
}

ActionReply SnapdHelper::enable(QVariantMap args)
{
    QString snap = args["snap"].toString();
    ActionReply reply;

    auto request = m_qsnapdClient.enable(snap);
    request->runSync();
    if (request->error() != QSnapdRequest::NoError) {
        reply.setErrorCode( ActionReply::BackendError );
        reply.setErrorDescription(request->errorString());
    }

    // Check for cancel requests
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [timer, request]() {
        if (HelperSupport::isStopped()) {
            qDebug() << "Stop request recived";
            request->cancel();
            timer->deleteLater();
        }
    });
    timer->start(200);


    request->deleteLater();
    return reply;
}

ActionReply SnapdHelper::refresh(QVariantMap args)
{
    HelperSupport::progressStep(QVariantMap());

    QString snap = args["snap"].toString();
    QString channel = args.value("channel").toString();

    ActionReply reply;

    auto request = m_qsnapdClient.refresh(snap, channel);

    // Check for cancel requests
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [timer, request]() {
        if (HelperSupport::isStopped()) {
            qDebug() << "Stop request recived";
            request->cancel();
            timer->stop();
        }
        qDebug() << "tick" << HelperSupport::isStopped();

    });
    timer->start(200);
    request->runSync();
    timer->stop();

    if (request->error() != QSnapdRequest::NoError) {
        reply.setErrorCode( ActionReply::BackendError );
        reply.setErrorDescription(request->errorString());
    }

    timer->deleteLater();
    request->deleteLater();
    return reply;
}

ActionReply SnapdHelper::applysettings(QVariantMap args)
{
    HelperSupport::progressStep(QVariantMap());

    QString store = args["store"].toString();
    QString storeUrl =  args["storeUrl"].toString();

    bool useProxy = args["useProxy"].toBool();
    QString httpProxy =  args["httpProxy"].toString();
    QString httpsProxy =  args["httpsProxy"].toString();
    QString noProxy =  args["noProxy"].toString();

    ActionReply reply;

    QSettings enviroment("/etc/environment");
    for (QString key: enviroment.allKeys())
        qDebug() << key << enviroment.value(key);

    return reply;
}


KAUTH_HELPER_MAIN("org.nomad.softwarecenter", SnapdHelper)
