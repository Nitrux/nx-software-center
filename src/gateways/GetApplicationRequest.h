//
// Created by alexis on 7/4/18.
//

#ifndef NX_SOFTWARE_CENTER_GETAPPLICATIONREQUEST_H
#define NX_SOFTWARE_CENTER_GETAPPLICATIONREQUEST_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <entities/Application.h>
#include <entities/ApplicationFull.h>

class GetApplicationRequest : public QObject {
Q_OBJECT
    QString api;
    QString id;
    bool running;
    QNetworkAccessManager* networkAccessManager;
    QNetworkReply* reply;
    ApplicationFull result;

protected:
    friend class RestClient;
    GetApplicationRequest();
    void setApi(const QString& api);
    void setNetworkAccessManager(QNetworkAccessManager* networkAccessManager);

public:
    QUrl getUrl();
    void setId(const QString& id);
    const ApplicationFull& getResult() const;
public slots:

    void start();
    void stop();
signals:
    void resultReady();

    void failed(const QString& reason);
protected slots:
    void handleRequestFinished();
protected:
    ApplicationFull parseResponse(const QVariantMap& map) const;
    QList<ApplicationFull::Release> parseReleasesList(const QVariantList& vReleases) const;
};

#endif //NX_SOFTWARE_CENTER_GETAPPLICATIONREQUEST_H
