#include "snapstorelistdepartamentsrequest.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QSysInfo>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QFile>
#include <QDebug>

#include "snapstore.h"

SnapStoreListDepartamentsRequest::SnapStoreListDepartamentsRequest(SnapStore *snapStore):
    SnapStoreRequest(snapStore)
{
}

SnapStoreListDepartamentsRequest::~SnapStoreListDepartamentsRequest()
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
    request.setRawHeader("X-Ubuntu-Architecture", QSysInfo::currentCpuArchitecture().toLocal8Bit());

    request.setUrl(QUrl(url));

    qDebug() << "get" << url;

    QNetworkAccessManager * networkAccessManager = m_snapStore->networkAccessManager();



    m_reply = networkAccessManager->get(request);

    QObject::connect(m_reply, &QNetworkReply::finished, this, &SnapStoreListDepartamentsRequest::onRequestFinished);
    QObject::connect(m_reply, &QNetworkReply::downloadProgress, this, &SnapStoreRequest::onProgress);
    QObject::connect(m_reply, &QNetworkReply::sslErrors, this, &SnapStoreRequest::onSslErrors);
    QObject::connect(m_reply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), this, &SnapStoreRequest::onNetworkErrorResponse);
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

void SnapStoreListDepartamentsRequest::onRequestFinished()
{
    QJsonDocument document = QJsonDocument::fromJson(m_reply->readAll());
    m_reply->deleteLater();

    QJsonObject root = document.object();
    if (!root.isEmpty()) {
        auto _embedded = root.value("_embedded").toObject();
        auto departMentsArray = _embedded.value("clickindex:department").toArray();

        for (auto department : departMentsArray) {
            QJsonObject departmentObject = department.toObject();

            QVariantMap map;
            map.insert("slug", departmentObject.value("slug").toString());
            map.insert("name", departmentObject.value("name").toString());
            m_result.append(map);

        }
    }

    //    qDebug() << m_result;
    emit(complete());
}
