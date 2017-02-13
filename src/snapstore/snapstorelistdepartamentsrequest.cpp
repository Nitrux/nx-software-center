#include "snapstorelistdepartamentsrequest.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include <QNetworkRequest>
#include <QNetworkReply>

#include <QFile>
#include <QDebug>

#include "snapstore.h"

SnapStoreListDepartamentsRequest::SnapStoreListDepartamentsRequest(SnapStore *snapStore):
    SnapStoreRequest(snapStore)
{
}

void SnapStoreListDepartamentsRequest::runAsync()
{
    QNetworkRequest request;
    QString url = m_snapStore->storeUrl() + "/departments";
    m_error = 0;
    m_errorString = "";

//    request.setSslConfiguration(config);
    request.setRawHeader("Accept", "application/hal+json");

    request.setUrl(QUrl(url));

    qDebug() << "get" << url;

    QNetworkAccessManager * networkAccessManager = m_snapStore->networkAccessManager();

    QObject::connect(networkAccessManager, &QNetworkAccessManager::finished, this, &SnapStoreListDepartamentsRequest::onRequestFinished);

    QNetworkReply* reply = networkAccessManager->get(request);

    QObject::connect(reply, &QNetworkReply::downloadProgress, this, &SnapStoreRequest::onProgress);
    QObject::connect(reply, &QNetworkReply::sslErrors, this, &SnapStoreRequest::onSslErrors);
    QObject::connect(reply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), this, &SnapStoreRequest::onNetworkErrorResponse);
}

void SnapStoreListDepartamentsRequest::cancel()
{
    qDebug() << __func__ << "not implemented yet.";
}

int SnapStoreListDepartamentsRequest::departamentCount() const
{
    return m_result.size();
}

QVariantMap SnapStoreListDepartamentsRequest::departament(int idx) const
{
    return m_result.at(idx);
}

void SnapStoreListDepartamentsRequest::onRequestFinished(QNetworkReply *reply)
{
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    QJsonObject root = document.object();
    if (root.isEmpty())
        return;

    auto _embedded = root.value("_embedded").toObject();
    auto departMentsArray = _embedded.value("clickindex:department").toArray();

    for (auto department : departMentsArray) {
        QJsonObject departmentObject = department.toObject();

        QVariantMap map;
        map.insert("slug", departmentObject.value("slug").toString());
        map.insert("name", departmentObject.value("name").toString());
        m_result.append(map);

    }
//    qDebug() << m_result;
    emit(complete());
}
