//
// Created by alexis on 26/12/17.
//

#ifndef NOMAD_SOFTWARE_CENTER_SIMPLEDOWNLOADTOFILEJOB_H
#define NOMAD_SOFTWARE_CENTER_SIMPLEDOWNLOADTOFILEJOB_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>

#include "DownloadManager.h"
#include <QTimer>

class SimpleDownloadToFileJob : public DownloadToFileJob {
    Q_OBJECT

    QNetworkAccessManager *networkAccessManager;
    QNetworkRequest request;
    QNetworkReply *reply = nullptr;
    QFile file;
    bool aborted;

    float speed;
    qint64 totalBytes;
    qint64 bytesRead;
    qint64 bytesReadLastTick;
    QTimer *timer;

public:
    SimpleDownloadToFileJob(const QNetworkRequest &request, const QString path,
                              QNetworkAccessManager *networkAccessManager,
                              QObject *parent = nullptr);

    void execute() override;

protected slots:
    void executeFromQObjectThread();

    void handleReadyRead();

    void handleDownloadProgress(qint64 bytesRead, qint64 totalBytes);

    void handleFinished();

    void disposeNetworkReply();

    bool isAPositiveReply() const { return reply->error() == QNetworkReply::NoError; }

    void handleTimerTick();

private:
    QString size_human(float num) const;

    void reportProgress();
    void updateDownloadSpeed();
};


#endif //NOMAD_SOFTWARE_CENTER_SIMPLEDOWNLOADTOFILEJOB_H
