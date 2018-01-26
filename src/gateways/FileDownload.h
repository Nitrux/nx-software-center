//
// Created by alexis on 16/01/18.
//

#ifndef NOMAD_SOFTWARE_CENTER_FILEDOWNLOAD_H
#define NOMAD_SOFTWARE_CENTER_FILEDOWNLOAD_H

#include <QFile>
#include <QObject>
#include <QString>
#include <QNetworkReply>
#include <QNetworkAccessManager>

#include "Download.h"

class FileDownload : public Download {
Q_OBJECT
    QFile file;

public:
    FileDownload(const QString &url, const QString &target_path, QObject *parent = nullptr);

    virtual ~FileDownload();

    const QString getTarget_path() const;

public slots:

    void start() override;

protected slots:

    void handleReadyRead();

    void handleCompleted();

    void handleStopped();

private:
    void downloadAvailableBytes();
};


#endif //NOMAD_SOFTWARE_CENTER_FILEDOWNLOAD_H
