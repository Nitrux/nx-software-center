//
// Created by alexis on 23/12/17.
//

#include "CachedDownloadManager.h"

#include <QDir>
#include <QFile>
#include <QSettings>
#include <QCryptographicHash>
#include <QStandardPaths>

#include "SimpleDownloadToMemoryJob.h"

DownloadToFileJob *CachedDownloadManager::downloadToFile(const QString &, const QString &) {
    return nullptr;
}

void CachedDownloadManager::writeFile(const QString &path, const QByteArray &data) const {
    QFile f(path);
    f.open(QIODevice::WriteOnly);
    f.write(data);
    f.close();
}

DownloadToMemoryJob *CachedDownloadManager::downloadToMemory(const QString &url) {
    DownloadToMemoryJob *job = nullptr;
    QString path = QCryptographicHash::hash(url.toLocal8Bit(), QCryptographicHash::Md5).toHex();
    QString cache_dir = QStandardPaths::standardLocations( QStandardPaths::CacheLocation).first();
    cache_dir = cache_dir;
    QDir d(cache_dir);
    d.mkdir("web_cache");
    path = d.absoluteFilePath("web_cache") + "/"+ path + ".cache";

    QFile f(path);
    if (f.exists())
        job = new CachedDownloadToMemoryJob(url, path, this);
    else {
        QNetworkRequest request = QNetworkRequest(url);
        request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

        job = new SimpleDownloadToMemoryJob(request, &networkAccessManager, this);
        connect(job, &DownloadToMemoryJob::finished, [=]() {
            const QByteArray &data = job->getData();

            qDebug() << url << " " << path << " " << data.isEmpty();
            writeFile(path, data);
        });
    }
    return job;
}


CachedDownloadManager::CachedDownloadManager(QObject *parent) : DownloadManager(parent) {
}

CachedDownloadManager::~CachedDownloadManager() {
}

QByteArray CachedDownloadToMemoryJob::readFile(const QString &path) const {
    QFile f(path);
    f.open(QIODevice::ReadOnly);
    QByteArray data = f.readAll();
    f.close();
    return data;
}

CachedDownloadToMemoryJob::CachedDownloadToMemoryJob(const QString &url, const QString &file, QObject *parent)
        : DownloadToMemoryJob(parent),
          url(url), file(file) {
}

void CachedDownloadToMemoryJob::execute() {
    data = readFile(file);

    emit finished();
}

