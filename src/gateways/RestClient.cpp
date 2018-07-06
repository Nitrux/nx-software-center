//

#include <QNetworkRequest>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "RestClient.h"
#include "ApplicationsSearchRequest.h"
#include "GetApplicationRequest.h"

RestClient::RestClient(QString url, QObject* parent)
        :QObject(parent), api(url), networkAccessManager(new QNetworkAccessManager(this))
{

}

ApplicationsSearchRequest* RestClient::buildSearchRequest(const QString& query, const QString& category) const
{
    auto request = new ApplicationsSearchRequest();
    request->setNetworkAccessManager(networkAccessManager);
    request->setApi(api);
    request->setQuery(query);
    request->setCategory(category);

    return request;
}

GetApplicationRequest* RestClient::buildGetApplicationRequest(const QString& id) const
{
    auto request = new GetApplicationRequest();
    request->setNetworkAccessManager(networkAccessManager);
    request->setApi(api);
    request->setId(id);

    return request;
}
QNetworkAccessManager* RestClient::getNetworkAccessManager() const
{
    return networkAccessManager;
}
void RestClient::setNetworkAccessManager(QNetworkAccessManager* networkAccessManager)
{
    RestClient::networkAccessManager = networkAccessManager;
}
