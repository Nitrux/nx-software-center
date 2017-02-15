#include "snapstoresnapdetailsrequest.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QSysInfo>
#include <QDebug>

#include "snapstore.h"

SnapStoreSnapDetailsRequest::SnapStoreSnapDetailsRequest(const QString &packageName, SnapStore *store) : SnapStoreRequest(store)
{
    m_packageName = packageName;
}

void SnapStoreSnapDetailsRequest::runAsync()
{
    QNetworkRequest request;
    QString url = m_snapStore->storeUrl() + "/package/" + m_packageName;

    //    request.setSslConfiguration(config);
    request.setRawHeader("Accept", "application/hal+json");

    request.setUrl(QUrl(url));

    qDebug() << url;
    qDebug() << request.rawHeaderList();

    m_reply= m_snapStore->networkAccessManager()->get(request);

    QObject::connect(m_reply, &QNetworkReply::finished, this, &SnapStoreSnapDetailsRequest::onNetworkRequestFinished);
    QObject::connect(m_reply, &QNetworkReply::downloadProgress, this, &SnapStoreRequest::onProgress);
    QObject::connect(m_reply, &QNetworkReply::sslErrors, this, &SnapStoreRequest::onSslErrors);
    QObject::connect(m_reply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), this, &SnapStoreRequest::onNetworkErrorResponse);
}

void SnapStoreSnapDetailsRequest::cancel()
{
    qDebug() << "Not implemented yet!";
}

QVariantMap SnapStoreSnapDetailsRequest::snapDetails()
{
    return m_snapDetails;
}

void SnapStoreSnapDetailsRequest::onNetworkRequestFinished()
{
    QJsonDocument document = QJsonDocument::fromJson(m_reply->readAll());
    m_reply->deleteLater();

    QJsonObject root = document.object();
    if (!root.isEmpty()) {
        m_snapDetails = root.toVariantMap();
    }
    emit(complete());
}
