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

class SimpleDownloadToFileJob : public DownloadToFileJob {
    Q_OBJECT

    QNetworkAccessManager *networkAccessManager;
    QNetworkRequest request;
    QNetworkReply *reply = nullptr;
    QFile file;
    bool aborted;
public:
    SimpleDownloadToFileJob(const QNetworkRequest &request, const QString path,
                              QNetworkAccessManager *networkAccessManager,
                              QObject *parent);

    void execute() override;

protected slots:
    void handleBytesAvailable();

    void handleDownloadProgress(qint64 bytesRead, qint64 totalBytes);

    void handleFinished();

    void disposeNetworkReply();

    bool isAPositiveReply() const { return reply->error() == QNetworkReply::NoError; }
};


#endif //NOMAD_SOFTWARE_CENTER_SIMPLEDOWNLOADTOFILEJOB_H
