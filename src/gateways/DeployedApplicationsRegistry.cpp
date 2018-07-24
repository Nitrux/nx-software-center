//
// Created by alexis on 7/20/18.
//

#include <QSet>
#include <QMap>
#include <QDir>
#include <QDebug>
#include <QJsonDocument>
#include <QCryptographicHash>

#include "DeployedApplicationsRegistry.h"
#include "../../lib/appimage-metadata-extractor/src/entities/FileMetadataExtractor.h"

DeployedApplicationsRegistry::DeployedApplicationsRegistry() {
}

void DeployedApplicationsRegistry::setApplicationsDir(const QString &applicationsDir) {
    DeployedApplicationsRegistry::applicationsDir.setPath(applicationsDir);
}

void DeployedApplicationsRegistry::setCacheDir(const QString &cacheDir) {
    DeployedApplicationsRegistry::cacheDir.setPath(cacheDir);
}

QStringList DeployedApplicationsRegistry::listApplicationFiles() {
    QStringList files;
    for (const auto &entry : applicationsDir.entryList({"*.AppImage"}, QDir::Files))
        files << applicationsDir.absoluteFilePath(entry);
    return files;
}

void DeployedApplicationsRegistry::extractFileInfo(const QString &path) {
    FileMetadataExtractor extractor(path);

    auto metadata = extractor.extractMetadata();
    const auto icon = extractor.extractIcon();

    const auto pathSha1 = getSha1(path);
    cacheDir.mkdir(pathSha1);

    auto itemCacheDir = cacheDir;
    itemCacheDir.cd(pathSha1);

    // Use a local url for the icon
    if (!icon.isEmpty())
        metadata["icon"] = "file://" + itemCacheDir.absoluteFilePath("AppImageIcon");

    // Set file path
    auto fileMetadata = metadata["file"].toMap();
    fileMetadata["path"] = path;
    metadata["file"] = fileMetadata;

    writeAppImageInfo(metadata, itemCacheDir);

    writeAppImageIcon(icon, itemCacheDir);

    QFile f(itemCacheDir.absoluteFilePath("record"));
}

QString DeployedApplicationsRegistry::getSha1(const QString &path) const {
    QCryptographicHash cryptographicHash(QCryptographicHash::Sha1);
    cryptographicHash.addData(path.toStdString().c_str());
    auto pathSha1 = cryptographicHash.result().toHex();
    return pathSha1;
}

void DeployedApplicationsRegistry::writeAppImageInfo(const QVariantMap &metadata, const QDir &outputDir) {
    auto doc = QJsonDocument::fromVariant(metadata);
    QFile output(outputDir.absoluteFilePath("AppImageInfo.json"));
    if (output.open(QIODevice::WriteOnly)) {
        output.write(doc.toJson());
        output.close();
    }
}

void DeployedApplicationsRegistry::writeAppImageIcon(QByteArray icon, const QDir &outputDir) {
    if (icon.isEmpty()) {
        qWarning() << "Empty icon file";
        return;
    } else {
        QFile f(outputDir.absoluteFilePath("AppImageIcon"));
        if (f.open(QIODevice::WriteOnly)) {
            f.write(icon);
            f.close();
        } else
            qWarning() << "Unable to write icon file";
    }
}

void DeployedApplicationsRegistry::synchronizeCache() {
    auto files = listApplicationFiles();
    extractInfoFromNewlyDeployedFiles(files);

    // THE FOLLOWING FUNCTION SHOULD BE ALWAYS EXECUTED IN THIS ORDER
    syncFilesHashCache(files);
    dropDeleteFilesCache();
    syncAppImageInfoCache();
}

void DeployedApplicationsRegistry::dropDeleteFilesCache() const {
    const QList<QString> &deployedFilesSha1 = filesHash.values();
    const auto &entries = cacheDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const auto &entry :entries) {
        if (!deployedFilesSha1.contains(entry)) {
            QDir entryDir{cacheDir.absoluteFilePath(entry)};
            entryDir.removeRecursively();
        }
    }
}

void DeployedApplicationsRegistry::syncAppImageInfoCache() {
    auto entries = cacheDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const auto &entry :entries) {
        QDir entryDir{cacheDir.absoluteFilePath(entry)};
        AppImageInfo appImageInfo = getAppImageInfoFromCache(entryDir);
        appImageInfoCache[entry] = appImageInfo;
    }
}

void DeployedApplicationsRegistry::extractInfoFromNewlyDeployedFiles(const QStringList &files) {
    for (const auto &file: files)
        if (!cacheDir.exists(getSha1(file)))
            extractFileInfo(file);
}

void DeployedApplicationsRegistry::syncFilesHashCache(const QStringList &files) {
    DeployedApplicationsRegistry::filesHash.clear();

    for (const auto &file: files)
        DeployedApplicationsRegistry::filesHash[file] = getSha1(file);
}

AppImageInfo DeployedApplicationsRegistry::getAppImageInfoFromCache(const QDir &entryDir) const {
    AppImageInfo appImageInfo;
    QFile f{entryDir.absoluteFilePath("AppImageInfo.json")};
    if (f.open(QIODevice::ReadOnly)) {
        auto data = f.readAll();
        auto jsonDocument = QJsonDocument::fromJson(data);
        auto vMap = jsonDocument.toVariant().toMap();
        appImageInfo = AppImageInfo::fromVariant(vMap);
    } else
        qWarning() << "Unable to read AppInfo from: " << f.fileName();
    return appImageInfo;
}

QList<DeployedApplicationAbstract> DeployedApplicationsRegistry::listApplications() {
    synchronizeCache();

    QList<DeployedApplicationAbstract> list;
    for (const AppImageInfo &appImageInfo: appImageInfoCache.values())
        list << getAppImageInfoAbstract(appImageInfo);

    return list;
}

DeployedApplicationAbstract
DeployedApplicationsRegistry::getAppImageInfoAbstract(const AppImageInfo &appImageInfo) const {
    DeployedApplicationAbstract abstract;
    abstract.id = appImageInfo.id;
    abstract.icon = appImageInfo.icon;
    abstract.abstract = appImageInfo.abstract;
    abstract.name = appImageInfo.name;
    abstract.fileSize = appImageInfo.file.size;
    abstract.latestReleaseVersion = appImageInfo.release.version;
    abstract.filePath = appImageInfo.file.path;
    return abstract;
}

AppImageInfo DeployedApplicationsRegistry::getApplicationInfo(const QString &filePath) {
    auto sha1 = getSha1(filePath);
    if (appImageInfoCache.contains(sha1))
        return appImageInfoCache[sha1];

    QDir itemDir = cacheDir;

    if (!itemDir.exists(sha1))
        extractFileInfo(filePath);

    itemDir.cd(sha1);
    auto item = getAppImageInfoFromCache(itemDir);

    appImageInfoCache[sha1] = item;
    filesHash[filePath] = sha1;

    return item;
}

void DeployedApplicationsRegistry::registerFile(const QString &path, const AppImageInfo &info) {
    FileMetadataExtractor extractor(path);

    auto metadata = extractor.extractMetadata();
    const auto icon = extractor.extractIcon();

    const auto pathSha1 = getSha1(path);
    cacheDir.mkdir(pathSha1);

    auto itemCacheDir = cacheDir;
    itemCacheDir.cd(pathSha1);

    // Use a local url for the icon
    if (!icon.isEmpty())
        metadata["icon"] = "file://" + itemCacheDir.absoluteFilePath("AppImageIcon");

    // Set file path
    auto fileMetadata = metadata["file"].toMap();
    fileMetadata["path"] = path;
    metadata["file"] = fileMetadata;

    auto upstreamMetadata = AppImageInfo::toVariant(info).toMap();
    const QStringList fieldsToReplace = {"id", "name", "abstract", "description", "license", "categories",
                                         "keywords", "languages", "developer", "release", "screenshots", "mimeTypes",
                                         "links"};
    for (const auto &field: fieldsToReplace)
        metadata[field] = upstreamMetadata[field];

    writeAppImageInfo(metadata, itemCacheDir);

    writeAppImageIcon(icon, itemCacheDir);

    QFile f(itemCacheDir.absoluteFilePath("record"));

}
