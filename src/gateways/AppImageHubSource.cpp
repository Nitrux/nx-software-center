#include "AppImageHubSource.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QThread>

#include "AppImageInstallLinksRegExParser.h"

void AppImageHubSource::fetchAllApplications() {
    if (job == nullptr) {
        job = downloadManager->download(url);
        connect(job, &Download::completed, this,
                &AppImageHubSource::handleDownloadFinished);
        connect(job, &Download::stopped, this,
                &AppImageHubSource::handleDownloadStopped);
        job->start();
    }
}

void AppImageHubSource::handleDownloadFinished() {
    if (job != nullptr) {
        const QByteArray &data = job->getContent();
        appsData = parseApplicationsDataFromJson(data);

        parsers.clear();
        parsersFinished = 0;
        isAwaintingForParsers = false;
        for (const QVariantMap &appData : appsData)
            spawnApplicationReleasesParser(appData);

        isAwaintingForParsers = true;
        if (areAllApplicationReleasesParsersFinished())
                emit fetchedAllApplications(results);

        job->deleteLater();
        job = nullptr;
    }
}

void AppImageHubSource::handleDownloadStopped()
{
    emit fetchError("Unable to fetch: " + url);

    job->deleteLater();
    job = nullptr;
}

bool AppImageHubSource::areAllApplicationReleasesParsersFinished() const {
    return
            isAwaintingForParsers &&
            parsers.count() == parsersFinished;
}

void AppImageHubSource::spawnApplicationReleasesParser(const QVariantMap &appData) {
    QString link = appData.value("download_link", "").toString();

    AppImageInstallLinksRegExParser *parser = new AppImageInstallLinksRegExParser(link, downloadManager, this);
    connect(parser, &AppImageInstallLinksRegExParser::newLink, this, &AppImageHubSource::handleNewInstallLinkFound);
    connect(parser, &AppImageInstallLinksRegExParser::finished, this, &AppImageHubSource::handleParserFinished);
    connect(parser, &AppImageInstallLinksRegExParser::error, this, &AppImageHubSource::handleParserError);

    parsers << parser;
    if (link.isEmpty() || link == "") {
        qDebug() << appData.value("code_name").toString() << " has no download link";
        parsersFinished++;
    } else
        parser->parse();

}

QJsonArray AppImageHubSource::extractApplicationsArray(const QByteArray &data) const {
    QJsonDocument doc = QJsonDocument::fromJson(data);
    auto jsonRoot = doc.object();
    QJsonArray r;
    if (jsonRoot.contains("items"))
        r = jsonRoot.value("items").toArray();
    else
        qWarning() << __FUNCTION__ << " missing 'items' key in json.";
    return r;
}

QList<QVariantMap> AppImageHubSource::parseApplicationsDataFromJson(const QByteArray &data) const {
    auto applicationsArray = extractApplicationsArray(data);
    QList<QVariantMap> items;

    for (QJsonValue jsonValue : applicationsArray)
        items << extractApplicationInfo(jsonValue);

    return items;
}

QVariantMap AppImageHubSource::extractApplicationInfo(const QJsonValue &value) const {
    QJsonObject appImageDataObject = value.toObject();
    QVariantMap m;
    m["name"] = appImageDataObject.value("name").toString();
    m["code_name"] = appImageDataObject.value("name").toString().toLower().replace(" ", "");
    m["description"] = appImageDataObject.value("description").toString();
    m["licence"] = appImageDataObject.value("licence").toString();
    m["authors"] = extractApplicationAuthors(appImageDataObject);
    m["categories"] = extractApplicationCategories(appImageDataObject);
    m["screenshots"] = extractApplicationScreenshots(appImageDataObject);

    QVariantMap links = extractApplicationLinks(appImageDataObject);
    for (QVariantMap::iterator it = links.begin(); it != links.end(); ++it)
        m.insert(it.key(), it.value());

    return m;
}

QVariantMap AppImageHubSource::extractApplicationLinks(const QJsonObject &appImageDataObject) const {
    QVariantMap links;
    auto linksArray = appImageDataObject.value("links").toArray();
    for (QJsonValue linkItem : linksArray) {
        auto linkObject = linkItem.toObject();
        auto key = linkObject.value("type").toString().toLower();
        auto value = linkObject.value("url").toString();

        links.insert(key + "_link", value);
    }
    return links;
}

QStringList AppImageHubSource::extractApplicationScreenshots(const QJsonObject &appImageDataObject) const {
    QStringList screenshots;
    for (QJsonValue value :
            appImageDataObject.value("screenshots").toArray())
        screenshots << value.toString();
    return screenshots;
}

QStringList AppImageHubSource::extractApplicationCategories(const QJsonObject &appImageDataObject) const {
    QStringList categories;
    for (QJsonValue value :
            appImageDataObject.value("categotries").toArray())
        categories << value.toString();
    return categories;
}

QStringList AppImageHubSource::extractApplicationAuthors(const QJsonObject &appImageDataObject) const {
    QStringList authors;
    for (QJsonValue value : appImageDataObject.value("autors").toArray())
        authors << value.toString();
    return authors;
}

Application AppImageHubSource::buildApplication(const QVariantMap &map) const {
    QString version = guessVersion(map);
    Application a(map["code_name"].toString(), version);
    a.setName(map["code_name"].toString());
    a.setArch(QSysInfo::currentCpuArchitecture());
    a.setDescription(map["description"].toString());
    a.setLicence(map["licence"].toString());

    a.setAuthors(map["authors"].toStringList());
    a.setCategories(map["categories"].toStringList());
    a.setScreenshots(map["screenshots"].toStringList());
    a.setDownloadUrl(map["download_link"].toString());
    return a;
}

QString AppImageHubSource::guessVersion(const QVariantMap &map) const {
    QString link = map["download_link"].toString();

    QString rxString = R"((.*\/)([^\/].*)\.AppImage)";
    QRegExp rx(rxString);

    int pos = 0;
    QString s = link;
    if (rx.indexIn(link, pos) != -1)
        s = rx.cap(2);

    else
        s = link.split("/").last();

    s = s.toLower();
    // Remove arch
    s.replace(QSysInfo::currentCpuArchitecture(), "", Qt::CaseInsensitive);

    // Remove name
    QString name = map["code_name"].toString();
    s.replace(name, "", Qt::CaseInsensitive);

    s.replace("-", " ");
    s = s.simplified();
    s.replace(" ", "-");
    s.replace("_", " ");
    s = s.simplified();
    s.replace(" ", "_");
    return s;
}

void AppImageHubSource::handleParserFinished() {
    auto *parser = dynamic_cast<AppImageInstallLinksRegExParser *>(sender());
    if (parser != nullptr) {
        parsersFinished++;

        if (areAllApplicationReleasesParsersFinished())
            reportResults();

        parser->deleteLater();
    }
}

void AppImageHubSource::handleNewInstallLinkFound(const QString &link) {
    auto *parser = dynamic_cast<AppImageInstallLinksRegExParser *>(sender());
    if (parser != nullptr) {
        int i = parsers.indexOf(parser);
        QVariantMap data = appsData[i];
        data["download_link"] = link;

        results << buildApplication(data);
    } else
        qWarning() << __FUNCTION__ << "Unable to resolve signal sender";
}

void AppImageHubSource::handleParserError(const QString &error) {
    qWarning() << "Parser error: " << error;
    auto *parser = dynamic_cast<AppImageInstallLinksRegExParser *>(sender());
    if (parser != nullptr) {
        parsersFinished++;

        if (areAllApplicationReleasesParsersFinished())
            reportResults();

        parser->deleteLater();
    }
}

void AppImageHubSource::reportResults() {
    disposeParsers();

    emit fetchedAllApplications(results);
}

void AppImageHubSource::disposeParsers() {
//    for (QObject *object: parsers)
//        object->deleteLater();
    parsers.clear();
    parsersFinished = 0;
}

AppImageHubSource::AppImageHubSource(DownloadManager *downloadManager,
                                     QObject *parent)
        : Source(parent), downloadManager(downloadManager) {
    url = "https://appimage.github.io/feed.json";
}

AppImageHubSource::AppImageHubSource(const QString &url,
                                     DownloadManager *downloadManager,
                                     QObject *parent)
        : Source(parent), url(url), downloadManager(downloadManager) {

}


