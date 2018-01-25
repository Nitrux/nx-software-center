//
// Created by alexis on 25/01/18.
//

#include "CacheSource.h"

CacheSource::CacheSource(const QString &cachePath, QObject *parent) : Source(parent), cachePath(cachePath) {}

void CacheSource::fetchAllApplications() {
    QList<Application> applications;
    QDir cacheDir(cachePath);

    for (const QString &file : cacheDir.entryList({"*.json"})) {
        const QString &path = cacheDir.absoluteFilePath(file);

        QByteArray json = readFile(path);

        const Application a = ApplicationSerializer::deserialize(json);
        applications.append(a);
    }

    emit fetchedAllApplications(applications);
}

QByteArray CacheSource::readFile(const QString &path) const {
    QByteArray json;
    QFile f(path);
    if (f.open(QIODevice::ReadOnly)) {
        json = f.readAll();
        f.close();
    }
    return json;
}
