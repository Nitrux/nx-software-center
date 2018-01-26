#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QString>
#include <QByteArray>
#include <QObject>

#include "ContentDownload.h"
#include "FileDownload.h"

class DownloadManager : public QObject {
Q_OBJECT
public:
    DownloadManager(QObject *parent) : QObject(parent) {}

    virtual ~DownloadManager() {}

    virtual FileDownload *download(const QString &url, const QString &path) = 0;

    virtual ContentDownload *download(const QString &url) = 0;
};

#endif // DOWNLOADMANAGER_H
