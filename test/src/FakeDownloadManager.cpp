//
// Created by alexis on 23/12/17.
//

#include "FakeDownloadManager.h"

#include <QSettings>

void FakeDownloadManager::writeFile(const QString &path, const QByteArray &data) const {
    QFile f(path);
    f.open(QIODevice::WriteOnly);
    f.write(data);
    f.close();
}

FakeDownloadManager::~FakeDownloadManager() {
}

FileDownload *FakeDownloadManager::download(const QString &/*url*/, const QString &path) {
    FileDownload *f = new FileDownload("file://" TEST_DATA_DIR "echo-x86_64-8.25.AppImage", path, this);
    f->setProgressNotificationsEnabled(true);

    return f;
}

ContentDownload *FakeDownloadManager::download(const QString &url) {
    ContentDownload *job = nullptr;
    QString path = QCryptographicHash::hash(url.toLocal8Bit(), QCryptographicHash::Md5).toHex();
    path = QString(TEST_DATA_DIR "/webCache/") + path + ".cache";

    QFile f(path);
    if (f.exists())
        job = new ContentDownload("file://" + path, this);
    else {
        QNetworkRequest request = QNetworkRequest(url);
        request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

        job = new ContentDownload(url, this);
        connect(job, &ContentDownload::completed, [=]() {
            const QByteArray &data = job->getContent();
            writeFile(path, data);
        });
    }

    return job;
}

FakeDownloadManager::FakeDownloadManager(QNetworkAccessManager *networkAccessManager, QObject *parent)
        : DownloadManager(parent), networkAccessManager(networkAccessManager) {
}
