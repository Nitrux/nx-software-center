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

const QString Cache::getApplicationsCachePath() {
    QString path = QStandardPaths::standardLocations(QStandardPaths::CacheLocation).first();
    path += "/applications_cache/";

    // Ensure that the returned path exists
    QDir::home().mkpath(path);
    return path;
}

Cache::Cache(Repository *repository, QObject *parent) : QObject(parent), repository(repository) {

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
    const Application &a = repository->get(appId);
    QByteArray json;
    if (!a.getId().isEmpty()) {
        QJsonObject o;
        o.insert("id", a.getId());
        o.insert("codeName", a.getCodeName());
        o.insert("version", a.getVersion());

        QJsonDocument document(o);
        json = document.toJson(QJsonDocument::Compact);
    }

    QFile f(getApplicationsCachePath() + appId + ".json");
    if (f.open(QIODevice::WriteOnly)) {
        f.write(json);
        f.close();
    }
}

void Cache::removeApplication(const QString appId) {
    QFile f(getApplicationsCachePath() + appId + ".json");
    f.remove();
}
