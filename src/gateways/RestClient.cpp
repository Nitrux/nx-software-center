//

#include <QNetworkRequest>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "RestClient.h"
#include "ApplicationsSearchRequest.h"

RestClient::RestClient(QString url, QObject* parent)
        :QObject(parent), busy(false), api(url), networkAccessManager(new QNetworkAccessManager(this))
{

}
bool RestClient::isBusy() const
{
    return busy;
}
void RestClient::search(const QString& query, const QString& category)
{
    trySetBusy();

    QUrl url = buildSearchQueryUrl(query, category);

    QNetworkRequest request(url);
    networkAccessManager->get(request);
    connect(networkAccessManager, &QNetworkAccessManager::finished, this, &RestClient::handleSearchRequestResults);

}
QUrl RestClient::buildSearchQueryUrl(const QString& query, const QString& category) const
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
void RestClient::trySetBusy()
{
    if (busy)
        return;
//        throw RestClientBusy();

    setBusy(true);
}
void RestClient::setBusy(bool busy)
{
    RestClient::busy = busy;
    emit isBusyChanged(busy);
}
void RestClient::handleSearchRequestResults(QNetworkReply* reply)
{
    disconnect(networkAccessManager, &QNetworkAccessManager::finished, this, &RestClient::handleSearchRequestResults);

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
void RestClient::getApplication(const QString& id)
{
    trySetBusy();

    QUrl url = buildGetApplicationUrl(id);

    QNetworkRequest request(url);
    networkAccessManager->get(request);
    connect(networkAccessManager, &QNetworkAccessManager::finished, this, &RestClient::handleGetApplicationResult);
}
QUrl RestClient::buildGetApplicationUrl(const QString& id) const
{
    QUrl url = api+"/applications/"+id;
    url.setQuery(R"(filter={"include":[{"releases":{"files":{}}}]})");
    return url;
}
void RestClient::handleGetApplicationResult(QNetworkReply* reply)
{
    disconnect(networkAccessManager, &QNetworkAccessManager::finished, this, &RestClient::handleGetApplicationResult);

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

ApplicationsSearchRequest* RestClient::buildSearchRequest(const QString& query, const QString& category)
{
    auto request = new ApplicationsSearchRequest();
    request->setNetworkAccessManager(networkAccessManager);
    request->setApi(api);
    request->setQuery(query);
    request->setCategory(category);

    return request;
}

RestClientBusy::RestClientBusy(const std::string& __arg)
        :runtime_error(__arg) { }
