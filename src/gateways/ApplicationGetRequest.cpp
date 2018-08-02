//
// Created by alexis on 7/4/18.
//

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <entities/Application.h>
#include <iostream>
#include "ApplicationGetRequest.h"
#include "ApplicationRepositoryRestClient.h"

ApplicationGetRequest::ApplicationGetRequest()
        : running(false), networkAccessManager(nullptr), reply(nullptr) {}

void ApplicationGetRequest::setApi(const QString &api) {
    ApplicationGetRequest::api = api;
}

void ApplicationGetRequest::setNetworkAccessManager(QNetworkAccessManager *networkAccessManager) {
    ApplicationGetRequest::networkAccessManager = networkAccessManager;
}

void ApplicationGetRequest::start() {
    if (running)
        throw std::runtime_error("Get Application request already started.");

    running = true;
    QNetworkRequest request(getUrl());
    reply = networkAccessManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &ApplicationGetRequest::handleRequestFinished);
}

QUrl ApplicationGetRequest::getUrl() {
    QUrl url = api + "/applications/" + id;
    url.setQuery(R"(filter={"include":[{"releases":{"files":{}}}]})");
    return url;
}

void ApplicationGetRequest::handleRequestFinished() {
    disconnect(reply, &QNetworkReply::finished, this, &ApplicationGetRequest::handleRequestFinished);
    if (reply->error() == QNetworkReply::NoError) {
        auto response = reply->readAll();
        auto jsonDoc = QJsonDocument::fromJson(response);
        if (jsonDoc.isObject()) {
            auto obj = jsonDoc.object();
            auto vMap = obj.toVariantMap();
            application = parseResponse(vMap);
            emit completed();
        } else
            qWarning() << "Unexpected response: " << jsonDoc;
    } else
            emit failed(reply->errorString());

    reply->deleteLater();
    reply = nullptr;
    running = false;
}

Application ApplicationGetRequest::parseResponse(const QVariantMap &map) const {
    Application a;
    a.id = map["id"].toString();
    a.name = Application::LocalizedQString::fromVariant(map["name"]);
    a.icon = map["icon"].toString();
    a.abstract = Application::LocalizedQString::fromVariant(map["abstract"]);
    a.description = Application::LocalizedQString::fromVariant(map["description"]);
    a.license = Application::License::fromVariant(map["license"]);
    a.categories = map["categories"].toStringList();
    a.keywords = map["keywords"].toStringList();
    a.languages = map["languages"].toStringList();
    a.developer = Application::Developer::fromVariant(map["developer"]);

    QVariantList vReleases = map["releases"].toList();
    a.releases = parseReleasesList(vReleases);

    QList<Application::RemoteImage> screenshots;
    for (const auto &v: map["screenshots"].toList()) {
        screenshots << Application::RemoteImage::fromVariant(v);
    }
    a.screenshots = screenshots;

    a.mimeTypes = map["mime-types"].toStringList();

    QMap<QString, QString> links;
    auto linksVMap = map["links"].toMap();
    for (const auto &k: linksVMap.keys())
        links[k] = linksVMap[k].toString();

    a.links = links;
    return a;
}

QList<Application::Release> ApplicationGetRequest::parseReleasesList(const QVariantList &vReleases) const {
    QList<Application::Release> releases;
    for (const auto &v: vReleases) {
        auto map = v.toMap();
        auto r = Application::Release::fromVariant(map);
        QList<Application::File> files;
        const QVariantList &vFiles = map["files"].toList();
        for (const auto &vF: vFiles)
            files << Application::File::fromVariant(vF);

        r.files = files;
        releases << r;
    }
    return releases;
}

void ApplicationGetRequest::stop() {
    if (reply)
        reply->abort();
}
