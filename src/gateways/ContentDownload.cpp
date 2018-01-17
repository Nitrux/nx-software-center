//
// Created by alexis on 17/01/18.
//

#include "ContentDownload.h"

ContentDownload::ContentDownload(const QString &url, QObject *parent) : Download(url, parent) {}

void ContentDownload::handleFinished() {
    content = reply->readAll();

    Download::handleFinished();
}

const QByteArray &ContentDownload::getContent() const {
    return content;
}
