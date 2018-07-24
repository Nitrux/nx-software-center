//

#include <QNetworkRequest>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "Explorer.h"

Explorer::Explorer(QString url, QObject* parent)
        :QObject(parent), busy(false), api(url), networkAccessManager(new QNetworkAccessManager(this))
{

}
bool Explorer::isBusy() const
{
    return busy;
}
void Explorer::search(const QString& query, const QString& category)
{
    trySetBusy();

    QUrl url = buildSearchQueryUrl(query, category);

    QNetworkRequest request(url);
    networkAccessManager->get(request);
    connect(networkAccessManager, &QNetworkAccessManager::finished, this, &Explorer::handleSearchRequestResults);

}
QUrl Explorer::buildSearchQueryUrl(const QString& query, const QString& category) const
{
    QString urlQuery;
    if (!query.isEmpty())
        urlQuery += "query="+query;

    if (!category.isEmpty()) {
        if (!urlQuery.isEmpty())
            urlQuery += "&";
        urlQuery += "category="+category;
    }

    QUrl url = api+"/applications/search";
    url.setQuery(urlQuery);
    return url;
}
void Explorer::trySetBusy()
{
    if (busy)
        throw ExplorerBusy();

    setBusy(true);
}
void Explorer::setBusy(bool busy)
{
    Explorer::busy = busy;
    emit isBusyChanged(busy);
}
void Explorer::handleSearchRequestResults(QNetworkReply* reply)
{
    disconnect(networkAccessManager, &QNetworkAccessManager::finished, this, &Explorer::handleSearchRequestResults);

    if (reply->error()!=QNetworkReply::NoError) {
        emit failure(reply->errorString());
        emit searchCompleted(QList<QVariantMap>());
    }
    else {
        auto response = reply->readAll();
        auto jsonDoc = QJsonDocument::fromJson(response);
        QList<QVariantMap> apps;
        if (jsonDoc.isArray()) {
            auto array = jsonDoc.array();
            for (const QJsonValue& v: array) {
                if (v.isObject()) {
                    auto obj = v.toObject();
                    apps << obj.toVariantMap();
                }
                else
                    qWarning() << "Unexpected response " << v;
            }
        }
        else
            qWarning() << "Unexpected response: " << jsonDoc;

        emit searchCompleted(apps);
    }

    reply->deleteLater();
    setBusy(false);
}
void Explorer::getApplication(const QString& id)
{
    trySetBusy();

    QUrl url = buildGetApplicationUrl(id);

    QNetworkRequest request(url);
    networkAccessManager->get(request);
    connect(networkAccessManager, &QNetworkAccessManager::finished, this, &Explorer::handleGetApplicationResult);
}
QUrl Explorer::buildGetApplicationUrl(const QString& id) const
{
    QUrl url = api+"/applications/"+id;
    url.setQuery(R"(filter={"include":[{"releases":{"files":{}}}]})");
    return url;
}
void Explorer::handleGetApplicationResult(QNetworkReply* reply)
{
    disconnect(networkAccessManager, &QNetworkAccessManager::finished, this, &Explorer::handleGetApplicationResult);

    if (reply->error()==QNetworkReply::NoError) {
        auto response = reply->readAll();
        auto jsonDoc = QJsonDocument::fromJson(response);
        if (jsonDoc.isObject()) {
            auto obj = jsonDoc.object();
            emit getApplicationCompleted(obj.toVariantMap());
            qInfo() << obj;
        }
        else
            qWarning() << "Unexpected response: " << jsonDoc;
    }
    else {
        emit failure(reply->errorString());
        emit getApplicationCompleted(QVariantMap());
    }

    reply->deleteLater();
    setBusy(false);
}

ExplorerBusy::ExplorerBusy(const std::string& __arg)
        :runtime_error(__arg) { }
