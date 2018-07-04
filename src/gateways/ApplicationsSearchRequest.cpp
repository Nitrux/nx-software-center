//
// Created by alexis on 7/4/18.
//

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>

#include <iostream>
#include <entities/ApplicationFull.h>
#include <entities/ApplicationAbstract.h>
#include "RestClient.h"
#include "ApplicationsSearchRequest.h"

ApplicationsSearchRequest::ApplicationsSearchRequest()
        :running(false), networkAccessManager(nullptr), reply(nullptr) { }

void ApplicationsSearchRequest::setQuery(const QString& query)
{
    ApplicationsSearchRequest::query = query;
}

void ApplicationsSearchRequest::setCategory(const QString& category)
{
    ApplicationsSearchRequest::category = category;
}

void ApplicationsSearchRequest::setNetworkAccessManager(QNetworkAccessManager* networkAccessManager)
{
    ApplicationsSearchRequest::networkAccessManager = networkAccessManager;
}
const QList<ApplicationAbstract>& ApplicationsSearchRequest::getResults() const
{
    return results;
}
QUrl ApplicationsSearchRequest::getUrl() const
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
void ApplicationsSearchRequest::setApi(const QString& api)
{
    ApplicationsSearchRequest::api = api;
}

void ApplicationsSearchRequest::start()
{
    trySetRunning();

    results.clear();
    QUrl url = getUrl();

    QNetworkRequest request(url);
    reply = networkAccessManager->get(request);
    connect(reply, &QNetworkReply::finished, this,
            &ApplicationsSearchRequest::handleReplyFinished);
}

void ApplicationsSearchRequest::trySetRunning()
{
    if (running)
        throw std::runtime_error("ApplicationSearch already started.");

    else
        running = true;
}
void ApplicationsSearchRequest::stop()
{
    if (reply)
        reply->abort();
}

void ApplicationsSearchRequest::handleReplyFinished()
{
    disconnect(reply, &QNetworkReply::finished, this,
            &ApplicationsSearchRequest::handleReplyFinished);

    if (reply==nullptr) {
        emit failed("Internal error.");
        return;
    }

    if (reply->error()==QNetworkReply::NoError) {
        auto response = reply->readAll();
        results = parseResponse(response);

        emit resultsReady();
    }
    else
            emit failed(reply->errorString());

    reply->deleteLater();
    reply = nullptr;
    running = false;
}
QList<ApplicationAbstract> ApplicationsSearchRequest::parseResponse(const QByteArray& response) const
{
    QList<ApplicationAbstract> newResults;
    auto jsonDoc = QJsonDocument::fromJson(response);
    if (jsonDoc.isArray()) {
        auto array = jsonDoc.array();
        for (const auto& v: array) {
            if (v.isObject()) {
                auto obj = v.toObject();
                auto vMap = obj.toVariantMap();

                newResults << getAbstract(vMap);
            }
            else
                qWarning() << "Unexpected response " << v;
        }
    }
    else
        qWarning() << "Unexpected response: " << jsonDoc;
    return newResults;
}
ApplicationAbstract ApplicationsSearchRequest::getAbstract(const QVariantMap& vMap) const
{
    ApplicationAbstract a;
    a.id = vMap["id"].toString();
    a.icon = vMap["icon"].toString();
    a.name = ApplicationFull::LocalizedQString::fromVariant(vMap["name"]);
    a.abstract = ApplicationFull::LocalizedQString::fromVariant(vMap["name"]);

    return a;
}
