//
// Created by alexis on 26/12/17.
//

#include "SimpleDownloadToFileJob.h"


SimpleDownloadToFileJob::SimpleDownloadToFileJob(const QNetworkRequest &request,
                                                 const QString path,
                                                 QNetworkAccessManager *networkAccessManager,
                                                 QObject *parent)
        : DownloadToFileJob(path, parent),
          networkAccessManager(networkAccessManager),
          request(request), file(path), aborted(false) {

}

void SimpleDownloadToFileJob::execute() {
    file.open(QIODevice::WriteOnly);
    reply = networkAccessManager->get(request);
    QObject::connect(reply, &QNetworkReply::finished, this,
                     &SimpleDownloadToFileJob::handleFinished);
    QObject::connect(reply, &QNetworkReply::bytesAvailable, this,
                     &SimpleDownloadToFileJob::handleBytesAvailable);
    QObject::connect(reply, &QNetworkReply::downloadProgress, this,
                     &SimpleDownloadToFileJob::handleDownloadProgress);
}

void SimpleDownloadToFileJob::handleBytesAvailable() {
    if (file.isOpen())
        file.write(reply->readAll());
}

void SimpleDownloadToFileJob::handleDownloadProgress(qint64 bytesRead, qint64 totalBytes) {
    if (aborted)
        return;

    emit  progress(bytesRead, totalBytes, QString("Downloading: %1").arg(path));
}

void SimpleDownloadToFileJob::handleFinished() {
    if (isAPositiveReply()) {
        emit finished();
        file.close();
    } else {
        emit error("Download failed.");
        file.remove();
    }

    disposeNetworkReply();
}

void SimpleDownloadToFileJob::disposeNetworkReply() {
    reply->deleteLater();
    reply = nullptr;
}
