//
// Created by alexis on 24/01/18.
//
#include <QStandardPaths>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>

#include <QDebug>

#include "Cache.h"
#include "Repository.h"
#include "ApplicationSerializer.h"

const QString Cache::getApplicationsCachePath() {
    QString path = QStandardPaths::standardLocations(QStandardPaths::CacheLocation).first();
    path += "/applications_cache/";

    // Ensure that the returned path exists
    QDir::home().mkpath(path);
    return path;
}

Cache::Cache(QObject *parent) : QObject(parent), repository(nullptr) {
    loadApplicationIdsInCache();
}

void Cache::loadApplicationIdsInCache() {
    QDir cacheDir(getApplicationsCachePath());
    for (const QString &file : cacheDir.entryList({"*.json"})) {
        QFileInfo fileInfo(cacheDir.absoluteFilePath(file));
        applicationsInCache.insert(fileInfo.completeBaseName());
    }
}

void Cache::handleInstalledApplicationsChanged(const QStringList &applicationsIds) {
    for (const QString appId: applicationsIds) {
        if (applicationsInCache.contains(appId)) {
            applicationsInCache.remove(appId);
        } else
            storeApplication(appId);
    }

    for (const QString appId: applicationsInCache) {
        removeApplication(appId);
    }

    applicationsInCache = applicationsIds.toSet();
}

void Cache::storeApplication(const QString appId) {
    Application a;
    if (repository)
        a = repository->get(appId);

    if (!a.isEmpty()) {
        QByteArray json = ApplicationSerializer::serialize(a);

        QFile f(getApplicationsCachePath() + appId + ".json");
        if (f.open(QIODevice::WriteOnly)) {
            f.write(json);
            f.close();
        }
    }
}

void Cache::removeApplication(const QString appId) {
    QFile f(getApplicationsCachePath() + appId + ".json");
    f.remove();
}

void Cache::setRepository(Repository *repository) { this->repository = repository; }

QStringList Cache::getApplicationIdsInCache() {
    return applicationsInCache.toList();
}
