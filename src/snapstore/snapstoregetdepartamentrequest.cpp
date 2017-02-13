#include "snapstoregetdepartamentrequest.h"


#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

SnapStoreGetDepartamentRequest::SnapStoreGetDepartamentRequest(const QString &storeUrl, const QString &slug, QNetworkAccessManager *networkAccessManager, QObject *parent) : QObject(parent)
{
    Q_ASSERT(networkAccessManager != NULL);
    Q_ASSERT(!slug.isEmpty());
    m_storeUrl = storeUrl;
    m_slug = slug;
    m_networkAccessManager = networkAccessManager;
}

void SnapStoreGetDepartamentRequest::start()
{
    QNetworkRequest request;
    QString url = m_storeUrl + "/departments/" + m_slug;

    //    request.setSslConfiguration(config);
    request.setRawHeader("Accept", "application/hal+json");

    request.setUrl(QUrl(url));

    qDebug() << url;

    QObject::connect(m_networkAccessManager, &QNetworkAccessManager::finished, this, &SnapStoreGetDepartamentRequest::onFinished);

    QNetworkReply* reply = m_networkAccessManager->get(request);

    QObject::connect(reply, &QNetworkReply::downloadProgress, [] (qint64 a,qint64 b) {
        qDebug() <<  "progress" << a << b;
    }  );

    QObject::connect(reply, &QNetworkReply::sslErrors, [] (QList<QSslError> sslErrors)  {
        qDebug() << sslErrors;
    } );

    QObject::connect(reply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), this, &SnapStoreGetDepartamentRequest::onNetworkErrorResponse);
}

void SnapStoreGetDepartamentRequest::cancel()
{
    qDebug() << "Not implemented yet!";
}

int SnapStoreGetDepartamentRequest::highlightsCount()
{
    return m_higlights.size();
}

QVariantMap SnapStoreGetDepartamentRequest::highlight(int idx)
{
    return m_higlights.at(idx);
}

int SnapStoreGetDepartamentRequest::packagesCount()
{
    return m_packages.size();
}

QVariantMap SnapStoreGetDepartamentRequest::package(int idx)
{
    return m_packages.at(idx);
}



void SnapStoreGetDepartamentRequest::onNetworkErrorResponse(QNetworkReply::NetworkError error)
{
    qDebug() << "QNetworkReply::NetworkError" << error;

//    m_errorString = error;
//    emit(errorStringChanged(m_errorString));
}

void SnapStoreGetDepartamentRequest::onFinished(QNetworkReply *reply)
{
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    QJsonObject root = document.object();
    if (root.isEmpty())
        return;

    auto _embedded = root.value("_embedded").toObject();
    auto hightlights = _embedded.value("clickindex:highlight").toArray();
    auto packages = _embedded.value("clickindex:package").toArray();

//    qDebug() << "hightlights";
    m_higlights.clear();
    for (int i = 0; i < hightlights.count(); i++) {
        auto hightlight = hightlights.at(i).toObject();
//        qDebug() << hightlight.keys();
        QVariantMap map;
        map.insert("name", hightlight.value("name").toString());
        map.insert("description", hightlight.value("description").toString());


        auto _embedded = hightlight.value("_embedded").toObject();
        auto packages = _embedded.value("clickindex:package").toArray();

        map.insert("packages", packages.toVariantList());

        m_higlights.append(map);
//        qDebug() << map;
    }
//    qDebug() << "packages";
    m_packages.clear();
    for (int i = 0; i < packages.count(); i ++) {
        auto jsonObject = packages.at(i).toObject();
        m_packages.append(jsonObject.toVariantMap());
    }

    emit(SnapStoreGetDepartamentRequest::dataReady());
}
