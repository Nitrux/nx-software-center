//
// Created by alexis on 23/12/17.
//

#include "CachedDownloadManager.h"

#include <QDir>
#include <QThread>
#include <QCoreApplication>
#include <QSettings>
#include <QStandardPaths>

CachedDownloadManager::CachedDownloadManager(QNetworkAccessManager *networkAccessManager, QObject *parent)
        : DownloadManager(parent), networkAccessManager(networkAccessManager) {
}

CachedDownloadManager::~CachedDownloadManager() {
}

FileDownload *CachedDownloadManager::download(const QString &url, const QString &path) {
    qDebug() << "FileDownload from: " << url;
    QString cacheFilePath = getCachePath(url);
    qDebug() << "Cache file path: " << cacheFilePath;
    QFile f(cacheFilePath);

    FileDownload *job;
    if (f.exists()) {
        job = new FileDownload("file://" + cacheFilePath, path);
    } else {
        job = new FileDownload(url, path);
        connect(job, &Download::completed, [=]() {
            // Copy that file in cache

            QFile downloadedFile(path);
            downloadedFile.copy(path);
        });
    }

    return job;
}

QString CachedDownloadManager::getCachePath(const QString &url) const {
    QString cacheFilePath = QCryptographicHash::hash(url.toLocal8Bit(), QCryptographicHash::Md5).toHex();
    QString cache_dir = QStandardPaths::standardLocations(QStandardPaths::CacheLocation).first();
    cache_dir = cache_dir + "/web_cache";
    QDir d(cache_dir);
    d.mkpath(cache_dir);
    cacheFilePath = cache_dir + "/" + cacheFilePath + ".cache";
    return cacheFilePath;
}

ContentDownload *CachedDownloadManager::download(const QString &url) {
    qDebug() << "ContentDownload from: " << url;
    QString cacheFilePath = getCachePath(url);

    QFile f(cacheFilePath);
    ContentDownload *job;
    if (f.exists()) {
        qDebug() << "Using cache file: " << cacheFilePath;
        job = new ContentDownload("file://" + cacheFilePath);
    } else {
        job = new ContentDownload(url);
        connect(job, &Download::completed, [=]() {
            // Save result into a cache file
            writeFile(cacheFilePath, job->getContent());
        });
    }

    return job;
}

void CachedDownloadManager::writeFile(const QString &path, const QByteArray &data) const {
    QFile f(path);
    f.open(QIODevice::WriteOnly);
    f.write(data);
    f.close();
}
