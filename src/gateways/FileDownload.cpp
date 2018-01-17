//
// Created by alexis on 16/01/18.
//

#include "FileDownload.h"

FileDownload::FileDownload(const QString &url, const QString &target_path, QObject *parent)
        : Download(url, parent), file(target_path) {

}

FileDownload::~FileDownload() {
}

const QString FileDownload::getTarget_path() const {
    return file.fileName();
}

void FileDownload::start() {
    FileDownload::file.open(QIODevice::WriteOnly);

    Download::start();
    QObject::connect(reply, &QIODevice::readyRead, this,
                     &FileDownload::handleReadyRead);
}

void FileDownload::handleReadyRead() {
    downloadAvailableBytes();
}

void FileDownload::downloadAvailableBytes() {
    if (file.isOpen())
        file.write(reply->readAll());
}

void FileDownload::handleFinished() {
    downloadAvailableBytes();

    FileDownload::file.close();
    if (!wasCompletedProperly())
        FileDownload::file.remove();

    Download::handleFinished();
}

