//
// Created by alexis on 23/12/17.
//

#ifndef NOMAD_SOFTWARE_CENTER_SIMPLEDOWNLOADTOMEMORYJOB_H
#define NOMAD_SOFTWARE_CENTER_SIMPLEDOWNLOADTOMEMORYJOB_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QNetworkReply>
#include "DownloadManager.h"

class SimpleDownloadToMemoryJob : public DownloadToMemoryJob {
Q_OBJECT

    QNetworkAccessManager *networkAccessManager;
    QNetworkRequest request;
    QNetworkReply *networkReply = nullptr;
public:
    SimpleDownloadToMemoryJob(const QNetworkRequest &request,
                              QNetworkAccessManager *networkAccessManager,
                              QObject *parent);

    void execute();

protected slots:

    void handleNetworkReply();

    void disposeNetworkReply();

    bool isAPositiveReply() const { return networkReply->error() == QNetworkReply::NoError; }
};

#endif //NOMAD_SOFTWARE_CENTER_SIMPLEDOWNLOADTOMEMORYJOB_H
