//
// Created by alexis on 23/12/17.
//

#ifndef NOMAD_SOFTWARE_CENTER_SIMPLEDOWNLOADMANAGER_H
#define NOMAD_SOFTWARE_CENTER_SIMPLEDOWNLOADMANAGER_H

#include <QNetworkRequest>
#include "DownloadManager.h"

class QNetworkReply;
class QNetworkAccessManager;
class SimpleDownloadManager : public DownloadManager {
    Q_OBJECT
    QNetworkAccessManager *networkAccessManager;
    QNetworkReply *networkReply;

public:
    SimpleDownloadManager(QNetworkAccessManager *networkAccessManager,
                          QObject *parent);

    FileDownload *download(const QString &url, const QString &path) override;

    ContentDownload *download(const QString &url) override;

protected:
    QNetworkRequest createFollowRedirectRequest(const QString &url) const;
};


#endif //NOMAD_SOFTWARE_CENTER_SIMPLEDOWNLOADMANAGER_H
