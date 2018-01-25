//
// Created by alexis on 22/12/17.
//

#ifndef NOMAD_SOFTWARE_CENTER_APPIMAGEHUBSOURCE_H
#define NOMAD_SOFTWARE_CENTER_APPIMAGEHUBSOURCE_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QJsonArray>
#include <QVariantMap>

#include "entities/Source.h"
#include "entities/Application.h"
#include "gateways/DownloadManager.h"

#define DEFAULT_APPIMAGEHUB_URL "https://appimage.github.io/feed.json"

class AppImageInstallLinksRegExParser;

class AppImageHubSource : public Source {
Q_OBJECT
    QString url;

    DownloadManager *downloadManager;
    ContentDownload *job = nullptr;
    QList<Application> results;
    QList<QVariantMap> appsData;
    QList<AppImageInstallLinksRegExParser *> parsers;
    int parsersFinished;
    bool isAwaintingForParsers;
public:
    explicit AppImageHubSource(DownloadManager *downloadManager, QObject *parent = nullptr);

    explicit AppImageHubSource(const QString &url, DownloadManager *downloadManager, QObject *parent  = nullptr);

    void fetchAllApplications() override;

protected slots:

    void handleDownloadFinished();

    void handleDownloadStopped();

    void handleNewInstallLinkFound(const QString &link);

    void handleParserFinished();

    void handleParserError(const QString &error);

    QList<QVariantMap> parseApplicationsDataFromJson(const QByteArray &data) const;

    QJsonArray extractApplicationsArray(const QByteArray &data) const;

    QVariantMap extractApplicationInfo(const QJsonValue &value) const;

    QStringList extractApplicationAuthors(const QJsonObject &appImageDataObject) const;

    QStringList extractApplicationCategories(const QJsonObject &appImageDataObject) const;

    QStringList extractApplicationScreenshots(const QJsonObject &appImageDataObject) const;

    QVariantMap extractApplicationLinks(const QJsonObject &appImageDataObject) const;

    Application buildApplication(const QVariantMap &map) const;

    void spawnApplicationReleasesParser(const QVariantMap &appData);

    bool areAllApplicationReleasesParsersFinished() const;

    void reportResults();

    void disposeParsers();

    QString guessVersion(const QVariantMap &map) const;
};


#endif //NOMAD_SOFTWARE_CENTER_APPIMAGEHUBSOURCE_H
