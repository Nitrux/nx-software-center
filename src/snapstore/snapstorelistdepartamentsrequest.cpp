#include "snapstorelistdepartamentsrequest.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include <QNetworkRequest>
#include <QNetworkReply>

#include <QFile>
#include <QDebug>

SnapStoreListDepartamentsRequest::SnapStoreListDepartamentsRequest(const QString &storeUrl, QNetworkAccessManager *networkAccessManager, QObject *parent): QObject(parent)
{
    Q_ASSERT(networkAccessManager != NULL);
    m_url = storeUrl;
    m_networkAccessManager = networkAccessManager;
}

void SnapStoreListDepartamentsRequest::start()
{
    QNetworkRequest request;
    QString url = m_url + "/departments";

//    request.setSslConfiguration(config);
    request.setRawHeader("Accept", "application/hal+json");

    request.setUrl(QUrl(url));

    qDebug() << url;

    QObject::connect(m_networkAccessManager, &QNetworkAccessManager::finished, this, &SnapStoreListDepartamentsRequest::onFinished);

    QNetworkReply* reply = m_networkAccessManager->get(request);

    QObject::connect(reply, &QNetworkReply::downloadProgress, [] (qint64 a,qint64 b) {
        qDebug() <<  "progress" << a << b;
    }  );

    QObject::connect(reply, &QNetworkReply::sslErrors, [] (QList<QSslError> sslErrors)  {
        qDebug() << sslErrors;
    } );

    QObject::connect(reply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), this, &SnapStoreListDepartamentsRequest::onNetworkErrorResponse);
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

void SnapStoreListDepartamentsRequest::onNetworkErrorResponse(QNetworkReply::NetworkError error)
{
    qDebug() << "QNetworkReply::NetworkError" << error;

//    m_errorString = error;
//    emit(errorStringChanged(m_errorString));
}

void SnapStoreListDepartamentsRequest::onFinished(QNetworkReply *reply)
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
    emit(SnapStoreListDepartamentsRequest::finished());
}
