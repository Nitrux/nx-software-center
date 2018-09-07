//
// Created by alexis on 7/4/18.
//

#ifndef NX_SOFTWARE_CENTER_GETAPPLICATIONREQUEST_H
#define NX_SOFTWARE_CENTER_GETAPPLICATIONREQUEST_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <entities/Application.h>
#include <gateways/ApplicationRepositoryGet.h>

class ApplicationGetRequest : public ApplicationRepositoryGet {
Q_OBJECT
    QString api;
    bool running;
    QNetworkAccessManager* networkAccessManager;
    QNetworkReply* reply;

protected:
    friend class ApplicationRepositoryRestClient;
    ApplicationGetRequest();
    void setApi(const QString& api);
    void setNetworkAccessManager(QNetworkAccessManager* networkAccessManager);

public:
    QUrl getUrl();
public slots:

    void start() override;
    void stop() override;

protected slots:
    void handleRequestFinished();
protected:
    Application parseResponse(const QVariantMap& map) const;
    QList<Application::Release> parseReleasesList(const QVariantList& vReleases) const;
};

#endif //NX_SOFTWARE_CENTER_GETAPPLICATIONREQUEST_H
