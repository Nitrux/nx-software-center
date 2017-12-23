//
// Created by alexis on 23/12/17.
//

#ifndef NOMAD_SOFTWARE_CENTER_FAKEDOWNLOADMANAGER_H
#define NOMAD_SOFTWARE_CENTER_FAKEDOWNLOADMANAGER_H

#include <QtCore/QMap>
#include <QtNetwork/QNetworkAccessManager>
#include "DownloadManager.h"

class FakeDownloadToMemoryJob : public DownloadToMemoryJob {
Q_OBJECT

    QString url;
    QString file;
public:
    FakeDownloadToMemoryJob(const QString &qString, const QString &file, QObject *parent);

    void execute() override;

private:
    QByteArray readFile(const QString &path) const;
};

class FakeDownloadManager : public DownloadManager {
Q_OBJECT
    QNetworkAccessManager networkAccessManager;
    QMap<QString, QString> cache;

public:
    FakeDownloadManager(QObject *parent = nullptr);

    virtual ~FakeDownloadManager();

    DownloadToFileJob *downloadToFile(const QString &url, const QString &path) override;

    DownloadToMemoryJob *downloadToMemory(const QString &url) override;

private:
    void writeFile(const QString &path, const QByteArray &data) const;

    void writeCacheIndex();

    void readCacheIndex();
};


#endif //NOMAD_SOFTWARE_CENTER_FAKEDOWNLOADMANAGER_H
