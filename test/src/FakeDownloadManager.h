//
// Created by alexis on 23/12/17.
//

#ifndef NOMAD_SOFTWARE_CENTER_FAKEDOWNLOADMANAGER_H
#define NOMAD_SOFTWARE_CENTER_FAKEDOWNLOADMANAGER_H

#include <QtCore/QMap>
#include <QtNetwork/QNetworkAccessManager>
#include "gateways/DownloadManager.h"


class FakeDownloadManager : public DownloadManager {
Q_OBJECT
    QNetworkAccessManager *networkAccessManager;
    QMap<QString, QString> cache;

public:
    FakeDownloadManager(QNetworkAccessManager *networkAccessManager, QObject *parent = nullptr);

    virtual ~FakeDownloadManager();

    FileDownload *download(const QString &url, const QString &path) override;

    ContentDownload *download(const QString &url) override;

private:
    void writeFile(const QString &path, const QByteArray &data) const;

    void writeCacheIndex();

    void readCacheIndex();

    QByteArray readFile(const QString &path) const;
};


#endif //NOMAD_SOFTWARE_CENTER_FAKEDOWNLOADMANAGER_H
