//
// Created by alexis on 23/12/17.
//

#include "SimpleDownloadToMemoryJob.h"

SimpleDownloadToMemoryJob::SimpleDownloadToMemoryJob(const QNetworkRequest &request,
                                                     QNetworkAccessManager *networkAccessManager,
                                                     QObject *parent) : DownloadToMemoryJob(parent),
                                                                        networkAccessManager(networkAccessManager),
                                                                        request(request) {}


void SimpleDownloadToMemoryJob::handleNetworkReply() {
    if (networkReply == nullptr) {
        qWarning() << __FUNCTION__ << " called without a network reply.";
        return;
    }

    qDebug() << "Reply"
             << networkReply->header(QNetworkRequest::ContentLengthHeader)
             << networkReply->header(QNetworkRequest::ContentTypeHeader);

    if (isAPositiveReply()) {
        data = networkReply->readAll();
        emit finished();
    } else
            emit error("Network error: " + networkReply->errorString());

    disposeNetworkReply();

}

void SimpleDownloadToMemoryJob::execute() {
    networkReply = networkAccessManager->get(request);
    QObject::connect(networkReply, &QNetworkReply::finished, this,
                     &SimpleDownloadToMemoryJob::handleNetworkReply);
}

void SimpleDownloadToMemoryJob::disposeNetworkReply() {
    networkReply->deleteLater();
    networkReply = nullptr;

    networkReply->deleteLater();
}
