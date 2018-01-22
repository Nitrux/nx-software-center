#include "SimpleDownloadManager.h"


SimpleDownloadManager::SimpleDownloadManager(QNetworkAccessManager *networkAccessManager, QObject *parent)
        : DownloadManager(parent),
          networkAccessManager(networkAccessManager),
          networkReply(nullptr) {}


FileDownload *SimpleDownloadManager::download(const QString &url, const QString &path) {
    FileDownload *job = new FileDownload(url, path);

    return job;
}

ContentDownload *SimpleDownloadManager::download(const QString &url) {
    ContentDownload *job = new ContentDownload(url);

    return job;
}
