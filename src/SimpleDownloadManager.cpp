//
// Created by alexis on 23/12/17.
//

#include "SimpleDownloadManager.h"
#include "SimpleDownloadToMemoryJob.h"


SimpleDownloadManager::SimpleDownloadManager(QNetworkAccessManager *networkAccessManager, QObject *parent)
        : DownloadManager(parent),
          networkAccessManager(networkAccessManager),
          networkReply(nullptr) {}


QNetworkRequest SimpleDownloadManager::createFollowRedirectRequest(const QString &url) const {
    QNetworkRequest request = QNetworkRequest(url);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    return request;
}

DownloadToFileJob *SimpleDownloadManager::downloadToFile(const QString &, const QString &) {
    qWarning() << __FUNCTION__ << " is not implemented yet!";
    return nullptr;
}

DownloadToMemoryJob *SimpleDownloadManager::downloadToMemory(const QString &url) {
    QNetworkRequest request = createFollowRedirectRequest(url);

    SimpleDownloadToMemoryJob *job = new SimpleDownloadToMemoryJob(request, networkAccessManager, this);
    job->execute();
    return job;
}

