//
// Created by alexis on 23/12/17.
//

#include "FakeDownloadManager.h"

#include <QDir>
#include <QFile>
#include <QSettings>
#include <QCryptographicHash>
#include <QStandardPaths>

#include "SimpleDownloadToMemoryJob.h"

DownloadToFileJob *FakeDownloadManager::downloadToFile(const QString &, const QString &) {
    return nullptr;
}

void FakeDownloadManager::writeFile(const QString &path, const QByteArray &data) const {
    QFile f(path);
    f.open(QIODevice::WriteOnly);
    f.write(data);
    f.close();
}

DownloadToMemoryJob *FakeDownloadManager::downloadToMemory(const QString &url) {
    DownloadToMemoryJob *job = nullptr;
    QString path = QCryptographicHash::hash(url.toLocal8Bit(), QCryptographicHash::Md5).toHex();
    QString cache_dir = QStandardPaths::standardLocations( QStandardPaths::CacheLocation).first();
    cache_dir = cache_dir;
    QDir d(cache_dir);
    d.mkdir("web_cache");
    path = d.absoluteFilePath("web_cache") + "/"+ path + ".cache";

    QFile f(path);
    if (f.exists())
        job = new FakeDownloadToMemoryJob(url, path, this);
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


FakeDownloadManager::FakeDownloadManager(QObject *parent) : DownloadManager(parent) {
}

FakeDownloadManager::~FakeDownloadManager() {
}

QByteArray FakeDownloadToMemoryJob::readFile(const QString &path) const {
    QFile f(path);
    f.open(QIODevice::ReadOnly);
    QByteArray data = f.readAll();
    f.close();
    return data;
}

FakeDownloadToMemoryJob::FakeDownloadToMemoryJob(const QString &url, const QString &file, QObject *parent)
        : DownloadToMemoryJob(parent),
          url(url), file(file) {
}

void FakeDownloadToMemoryJob::execute() {
    data = readFile(file);

    emit finished();
}

