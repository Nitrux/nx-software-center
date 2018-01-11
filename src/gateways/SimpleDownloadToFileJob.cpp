//
// Created by alexis on 26/12/17.
//

#include "SimpleDownloadToFileJob.h"
#include <QTimer>

SimpleDownloadToFileJob::SimpleDownloadToFileJob(const QNetworkRequest &request,
                                                 const QString path,
                                                 QNetworkAccessManager *networkAccessManager,
                                                 QObject *parent)
        : DownloadToFileJob(path, parent),
          networkAccessManager(networkAccessManager),
          request(request), file(path), aborted(false), bytesReadLastTick(0) {

}

void SimpleDownloadToFileJob::execute() {
    file.open(QIODevice::WriteOnly);
    reply = networkAccessManager->get(request);

    QObject::connect(reply, &QNetworkReply::finished, this,
                     &SimpleDownloadToFileJob::handleFinished);
    QObject::connect(reply, &QNetworkReply::readyRead, this,
                     &SimpleDownloadToFileJob::handleReadyRead);
    QObject::connect(reply, &QNetworkReply::downloadProgress, this,
                     &SimpleDownloadToFileJob::handleDownloadProgress);

    emit  progress(0, 0, QString("Connecting to: %1").arg(request.url().toString()));


    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SimpleDownloadToFileJob::handleTimerTick);
    timer->start(1000);
}

void SimpleDownloadToFileJob::executeFromQObjectThread() {
    execute();
}

void SimpleDownloadToFileJob::handleReadyRead() {
    if (file.isOpen())
        file.write(reply->readAll());
}

void SimpleDownloadToFileJob::handleDownloadProgress(qint64 bytesRead, qint64 totalBytes) {
    if (aborted)
        return;

    this->bytesRead = bytesRead;
    this->totalBytes = totalBytes;

    reportProgress();
}

void SimpleDownloadToFileJob::reportProgress() {
    QString speed = size_human(this->speed);
    QString progressValue = size_human(this->bytesRead);
    QString progressTotal = size_human(this->totalBytes);
    static const QString messageTemplate = "%1 of %2 where downloaded. Speed %3/s";emit

    emit progress(this->bytesRead, this->totalBytes, messageTemplate.arg(progressValue, progressTotal, speed));
}

void SimpleDownloadToFileJob::handleFinished() {
    handleReadyRead();

    if (isAPositiveReply()) {
        emit finished();
        file.close();
    } else {
        emit error("Download failed.");
        file.remove();
    }

    QMetaObject::invokeMethod(timer, "stop");
    disposeNetworkReply();
}

void SimpleDownloadToFileJob::disposeNetworkReply() {
    reply->deleteLater();
    reply = nullptr;
}

QString SimpleDownloadToFileJob::size_human(float num) const {
    QStringList list;
    list << "KiB" << "MiB" << "GiB" << "TiB";

    QStringListIterator i(list);
    QString unit("bytes");

    while (num >= 1024.0 && i.hasNext()) {
        unit = i.next();
        num /= 1024.0;
    }
    return QString().setNum(num, 'f', 2) + " " + unit;
}

void SimpleDownloadToFileJob::handleTimerTick() {
    qint64 diff = bytesRead - bytesReadLastTick;
    bytesReadLastTick = bytesRead;

    speed = (speed + diff) / 2;

    reportProgress();
}
