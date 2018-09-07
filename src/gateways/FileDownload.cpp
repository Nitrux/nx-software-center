//
// Created by alexis on 17/01/18.
//

#include "FileDownload.h"

FileDownload::FileDownload(QString url, QString path, QObject *parent) : QObject(parent), source_url(url),
                                                                 running(false),
                                                                 progressNotificationsEnabled(false),
                                                                 file(path),
                                                                 isStopRequested(false),
                                                                 networkAccessManager(nullptr),
                                                                 speed(0), bytesRead(0), bytesReadLastTick(0) {
}


FileDownload::~FileDownload() {
    reply.clear();
}

const QString &FileDownload::getSource_url() const {
    return source_url;
}

void FileDownload::setNetworkAccessManager(QNetworkAccessManager *networkAccessManager) {
    FileDownload::networkAccessManager = networkAccessManager;
}

void FileDownload::start() {
    if (!running) {
        running = true;
        createNetworkAccessManagerIfNotExist();

        QNetworkRequest request = createRequest();

        reply = QSharedPointer<QNetworkReply>(networkAccessManager->get(request));

        QObject::connect(reply.data(), &QNetworkReply::finished, this, &FileDownload::handleFinished);

        if (progressNotificationsEnabled) {
            QObject::connect(reply.data(), &QNetworkReply::downloadProgress, this, &FileDownload::handleDownloadProgress);

            connect(&timer, &QTimer::timeout, this, &FileDownload::handleTimerTick);
            timer.start(1000);

            emit  progress(0, 0, QString("Connecting to: %1").arg(request.url().toString()));
        }

        FileDownload::file.open(QIODevice::WriteOnly);

        QObject::connect(this, &FileDownload::completed, this, &FileDownload::handleCompleted);
        QObject::connect(this, &FileDownload::stopped, this, &FileDownload::handleStopped);

        FileDownload::start();
        QObject::connect(reply.data(), &QIODevice::readyRead, this,
                         &FileDownload::handleReadyRead);
    }
}

QNetworkRequest FileDownload::createRequest() const {
    QNetworkRequest request = QNetworkRequest(source_url);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    return request;
}

void FileDownload::createNetworkAccessManagerIfNotExist() {
    if (!networkAccessManager)
        networkAccessManager = new QNetworkAccessManager(this);
}

void FileDownload::stop() {
    isStopRequested = true;
}

void FileDownload::handleDownloadProgress(qint64 bytesRead, qint64 totalBytes) {
    if (isStopRequested)
        return;

    this->bytesRead = bytesRead;
    this->totalBytes = totalBytes;

    reportProgress();
}

QString FileDownload::formatMemoryValue(float num) {
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

void FileDownload::updateDownloadSpeed() {
    qint64 diff = bytesRead - bytesReadLastTick;
    bytesReadLastTick = bytesRead;

    speed = (speed + diff) / 2;
    if (speed < 0)
        speed = 0;
    reportProgress();
}

void FileDownload::handleTimerTick() {
    if (isStopRequested) {
        reply->abort();
    } else
        updateDownloadSpeed();
}

void FileDownload::handleFinished() {
    running = false;
    if (wasCompletedProperly())
            emit completed();
    else {
        QString msg = isStopRequested ? "Stopped" : "FileDownload Error";
        emit stopped(msg);
    }
}

void FileDownload::reportProgress() {
    QString speed = formatMemoryValue(this->speed);
    QString progressValue = formatMemoryValue(this->bytesRead);
    QString progressTotal = formatMemoryValue(this->totalBytes);
    static const QString messageTemplate = "%1 of %2 --- %3/s";

    emit progress(this->bytesRead, this->totalBytes, messageTemplate.arg(progressValue, progressTotal, speed));
}

bool FileDownload::wasCompletedProperly() const {
    return reply->error() == QNetworkReply::NoError;
}


void FileDownload::setProgressNotificationsEnabled(bool progressNotificationsEnabled) {
    FileDownload::progressNotificationsEnabled = progressNotificationsEnabled;
}


bool FileDownload::isRunning() { return running; }


const QString FileDownload::getTargetPath() const {
    return file.fileName();
}

void FileDownload::handleReadyRead() {
    downloadAvailableBytes();
}

void FileDownload::downloadAvailableBytes() {
    if (file.isOpen())
        file.write(reply->readAll());
}

void FileDownload::handleCompleted() {
    downloadAvailableBytes();

    FileDownload::file.close();
}


void FileDownload::handleStopped() {
    file.remove();
}

