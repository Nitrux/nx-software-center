//
// Created by alexis on 7/2/18.
//

#ifndef NX_SOFTWARE_CENTER_EXPLORER_H
#define NX_SOFTWARE_CENTER_EXPLORER_H

#include <QList>
#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QNetworkReply>

#include "ApplicationRepository.h"

class ApplicationRepositoryRestClient : public QObject, public ApplicationRepository {
Q_OBJECT
    QString api;
    QNetworkAccessManager *networkAccessManager;
public:
    explicit ApplicationRepositoryRestClient(QString url, QObject *parent = nullptr);

    ApplicationsRepositorySearch *buildSearchRequest(const QString &query = "", const QString &category = "") const;

    ApplicationGetRequest *buildGetApplicationRequest(const QString &id) const;

    FileDownload * buildFileDownloadRequest(QString url, QString path) override;

    void setNetworkAccessManager(QNetworkAccessManager *networkAccessManager);
};

#endif //NX_SOFTWARE_CENTER_EXPLORER_H
