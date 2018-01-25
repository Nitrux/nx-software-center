//
// Created by alexis on 22/12/17.
//

#ifndef NOMAD_SOFTWARE_CENTER_APPLICATIONRELEASESPARSER_H
#define NOMAD_SOFTWARE_CENTER_APPLICATIONRELEASESPARSER_H

#include <QObject>
#include <QtCore/QMap>
#include <QtNetwork/QNetworkAccessManager>
#include "entities/Application.h"
#include "gateways/DownloadManager.h"

class AppImageInstallLinksRegExParser : public QObject {
Q_OBJECT
    QString url;
    DownloadManager *downloadManager;
    ContentDownload * job = nullptr;
public:
    AppImageInstallLinksRegExParser(const QString &url, DownloadManager *downloadManager,
                                    QObject *parent = nullptr);

    void parse();

signals:

    void finished();

    void error(const QString &error);

    void newLink(const QString &link);

protected slots:

    void handleDownloadFinished();

    QStringList parseAppimageLinks(const QString &str) const;

    QStringList resolveRelativeLinks(const QStringList &list) const;
};


#endif //NOMAD_SOFTWARE_CENTER_APPLICATIONRELEASESPARSER_H
