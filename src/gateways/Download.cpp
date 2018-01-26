//
// Created by alexis on 17/01/18.
//

#include "FileDownload.h"
#include "Download.h"

Download::Download(QString url, QObject *parent) : QObject(parent), source_url(url),
                                                   running(false),
                                                   progressNotificationsEnabled(false),
                                                   isStopRequested(false),
                                                   networkAccessManager(nullptr),
                                                   speed(0), bytesRead(0), bytesReadLastTick(0) {
}


Download::~Download() {
    reply.clear();
}

const QString &Download::getSource_url() const {
    return source_url;
}

QNetworkAccessManager *Download::getNetworkAccessManager() const {
    return networkAccessManager;
}

void Download::setNetworkAccessManager(QNetworkAccessManager *networkAccessManager) {
    Download::networkAccessManager = networkAccessManager;
}

void Download::start() {
    if (!running) {
        running = true;
        createNetworkAccessManagerIfNotExist();

        QNetworkRequest request = createRequest();

        reply = QSharedPointer<QNetworkReply>(networkAccessManager->get(request));

        QObject::connect(reply.data(), &QNetworkReply::finished, this, &Download::handleFinished);

        if (progressNotificationsEnabled) {
            QObject::connect(reply.data(), &QNetworkReply::downloadProgress, this, &Download::handleDownloadProgress);

            connect(&timer, &QTimer::timeout, this, &Download::handleTimerTick);
            timer.start(1000);

            emit  progress(0, 0, QString("Connecting to: %1").arg(request.url().toString()));
        }
    }
}

QNetworkRequest Download::createRequest() const {
    QNetworkRequest request = QNetworkRequest(source_url);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    return request;
}

void Download::createNetworkAccessManagerIfNotExist() {
    if (!networkAccessManager)
        networkAccessManager = new QNetworkAccessManager(this);
}

void Download::stop() {
    isStopRequested = true;
}

void Download::handleDownloadProgress(qint64 bytesRead, qint64 totalBytes) {
    if (isStopRequested)
        return;

    this->bytesRead = bytesRead;
    this->totalBytes = totalBytes;

    reportProgress();
}

QString Download::formatMemoryValue(float num) {
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

void Download::updateDownloadSpeed() {
    qint64 diff = bytesRead - bytesReadLastTick;
    bytesReadLastTick = bytesRead;

    speed = (speed + diff) / 2;
    if (speed < 0)
        speed = 0;
    reportProgress();
}

void Download::handleTimerTick() {
    if (isStopRequested) {
        reply->abort();
    } else
        updateDownloadSpeed();
}

void Download::handleFinished() {
    running = false;
    if (wasCompletedProperly())
            emit completed();
    else {
        QString msg = isStopRequested ? "Stopped" : "Download Error";
        emit stopped(msg);
    }
}

void Download::reportProgress() {
    QString speed = formatMemoryValue(this->speed);
    QString progressValue = formatMemoryValue(this->bytesRead);
    QString progressTotal = formatMemoryValue(this->totalBytes);
    static const QString messageTemplate = "%1 of %2 --- %3/s";

    emit progress(this->bytesRead, this->totalBytes, messageTemplate.arg(progressValue, progressTotal, speed));
}

bool Download::wasCompletedProperly() const {
    return reply->error() == QNetworkReply::NoError;
}


void Download::setProgressNotificationsEnabled(bool progressNotificationsEnabled) {
    Download::progressNotificationsEnabled = progressNotificationsEnabled;
}


bool Download::isRunning() { return running; }
