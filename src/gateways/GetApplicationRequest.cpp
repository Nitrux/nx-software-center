//
// Created by alexis on 7/4/18.
//

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <entities/ApplicationFull.h>
#include "GetApplicationRequest.h"
#include "RestClient.h"
GetApplicationRequest::GetApplicationRequest()
        :QObject(), running(false), networkAccessManager(nullptr), reply(nullptr) { }
void GetApplicationRequest::setApi(const QString& api)
{
    GetApplicationRequest::api = api;
}
void GetApplicationRequest::setId(const QString& id)
{
    GetApplicationRequest::id = id;
}
void GetApplicationRequest::setNetworkAccessManager(QNetworkAccessManager* networkAccessManager)
{
    GetApplicationRequest::networkAccessManager = networkAccessManager;
}
const ApplicationFull& GetApplicationRequest::getResult() const
{
    return result;
}
void GetApplicationRequest::start()
{
    if (running)
        throw std::runtime_error("Get Application request already started.");

    running = true;
    QNetworkRequest request(getUrl());
    reply = networkAccessManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &GetApplicationRequest::handleRequestFinished);
}
QUrl GetApplicationRequest::getUrl()
{
    QUrl url = api+"/applications/"+id;
    url.setQuery(R"(filter={"include":[{"releases":{"files":{}}}]})");
    return url;
}
void GetApplicationRequest::handleRequestFinished()
{
    disconnect(reply, &QNetworkReply::finished, this, &GetApplicationRequest::handleRequestFinished);
    if (reply->error()==QNetworkReply::NoError) {
        auto response = reply->readAll();
        auto jsonDoc = QJsonDocument::fromJson(response);
        if (jsonDoc.isObject()) {
            auto obj = jsonDoc.object();
            auto vMap = obj.toVariantMap();
            result = parseResponse(vMap);
            emit resultReady();
        }
        else
            qWarning() << "Unexpected response: " << jsonDoc;
    }
    else
            emit failed(reply->errorString());

    reply->deleteLater();
    reply = nullptr;
    running = false;
}
ApplicationFull GetApplicationRequest::parseResponse(const QVariantMap& map) const
{
    ApplicationFull a;
    a.id = map["id"].toString();
    a.name = ApplicationFull::LocalizedQString::fromVariant(map["name"]);
    a.icon = map["icon"].toString();
    a.abstract = ApplicationFull::LocalizedQString::fromVariant(map["abstract"]);
    a.description = ApplicationFull::LocalizedQString::fromVariant(map["description"]);
    a.license = ApplicationFull::License::fromVariant(map["license"]);
    a.categories = map["categories"].toStringList();
    a.keywords = map["keywords"].toStringList();
    a.languages = map["languages"].toStringList();
    a.developer = ApplicationFull::Developer::fromVariant(map["developer"]);

    QVariantList vReleases = map["releases"].toList();
    a.releases = parseReleasesList(vReleases);

    QList<ApplicationFull::RemoteImage> screenshots;
    for (const auto& v: map["screenshots"].toList()) {
        screenshots << ApplicationFull::RemoteImage::fromVariant(v);
    }
    a.screenshots = screenshots;

    a.mimeTypes = map["mime-types"].toStringList();

    QMap<QString, QString> links;
    auto linksVMap = map["links"].toMap();
    for (const auto& k: linksVMap.keys())
        links[k] = linksVMap[k].toString();

    a.links = links;
    return a;
}
QList<ApplicationFull::Release> GetApplicationRequest::parseReleasesList(const QVariantList& vReleases) const
{
    QList<ApplicationFull::Release> releases;
    for (const auto& v: vReleases) {
        auto r = ApplicationFull::Release::fromVariant(v);

        QList<ApplicationFull::File> files;
        QVariantList vFiles;
        for (const auto& vF: vFiles)
            files << ApplicationFull::File::fromVariant(vF);

        r.files = files;
        releases << r;
    }
    return releases;
}
void GetApplicationRequest::stop()
{
    if (reply)
        reply->abort();
}
