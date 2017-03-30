#include "snapstoregetdepartamentrequest.h"


#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QEventLoop>

#include "snapstore.h"

SnapStoreGetDepartamentRequest::SnapStoreGetDepartamentRequest(const QString &slug, SnapStore *snapStore) : SnapStoreRequest(snapStore)
{
    Q_ASSERT(!slug.isEmpty());
    m_slug = slug;
}

void SnapStoreGetDepartamentRequest::runAsync()
{
    m_reply = makeRequest();

    QObject::connect(m_reply, &QNetworkReply::finished, this, &SnapStoreGetDepartamentRequest::onNetworkRequestFinished);
    QObject::connect(m_reply, &QNetworkReply::downloadProgress, this, &SnapStoreRequest::onProgress);
    QObject::connect(m_reply, &QNetworkReply::sslErrors, this, &SnapStoreRequest::onSslErrors);
    QObject::connect(m_reply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), this, &SnapStoreRequest::onNetworkErrorResponse);
}

void SnapStoreGetDepartamentRequest::runSync()
{
    m_reply = makeRequest();

    QObject::connect(m_reply, &QNetworkReply::finished, this, &SnapStoreGetDepartamentRequest::onNetworkRequestFinished);
    QObject::connect(m_reply, &QNetworkReply::downloadProgress, this, &SnapStoreRequest::onProgress);
    QObject::connect(m_reply, &QNetworkReply::sslErrors, this, &SnapStoreRequest::onSslErrors);
    QObject::connect(m_reply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), this, &SnapStoreRequest::onNetworkErrorResponse);

    QEventLoop loop;
    connect(m_reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
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


void SnapStoreGetDepartamentRequest::onNetworkRequestFinished()
{
    QJsonDocument document = QJsonDocument::fromJson(m_reply->readAll());
    m_reply->deleteLater();

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
        QVariantMap package = jsonObject.toVariantMap();

        // Only add snaps
        QString packageName = package.value("name").toString();
        QString packageSnap_id = package.value("snap_id").toString();
        QString packageContent = package.value("content").toString();
        if (packageContent.compare("application") == 0 && !packageSnap_id.isEmpty()) {
//            qDebug() << package.value("name") << "\n" << package << "\n\n";
            m_packages.append(package);
        }
    }

    emit(SnapStoreRequest::complete());
}

QNetworkReply *SnapStoreGetDepartamentRequest::makeRequest()
{
    QNetworkRequest request;
    QString path = m_snapStore->storeUrl() + "/departments/" + m_slug + "";
    QUrl url(path);

    request.setUrl(url);
    request.setRawHeader("X-Ubuntu-Confinement","strict,classic");

    qDebug() << url << " query " << url.query();

    return m_reply= m_snapStore->networkAccessManager()->get(request);
}
