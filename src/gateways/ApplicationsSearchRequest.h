//
// Created by alexis on 7/4/18.
//

#ifndef NX_SOFTWARE_CENTER_APPLICATIONSSEARCH_H
#define NX_SOFTWARE_CENTER_APPLICATIONSSEARCH_H

#include <QtNetwork/QNetworkAccessManager>
#include <entities/ApplicationAbstract.h>
#include "ApplicationsRepositorySearch.h"

class ApplicationsSearchRequest : public ApplicationsRepositorySearch {
Q_OBJECT
    QString api;
    bool running;
    QNetworkAccessManager *networkAccessManager;
    QNetworkReply *reply;
protected:
    friend class ApplicationRepositoryRestClient;

    ApplicationsSearchRequest();
    virtual ~ApplicationsSearchRequest();

    void setNetworkAccessManager(QNetworkAccessManager *networkAccessManager);

    void setApi(const QString &api);

public:
    void start() override;

    void stop() override;

    QUrl getUrl() const;

protected slots:

    void handleReplyFinished();

protected:
    void trySetRunning();

    QList<ApplicationAbstract> parseResponse(const QByteArray &response) const;

    ApplicationAbstract getAbstract(const QVariantMap &vMap) const;
};

#endif //NX_SOFTWARE_CENTER_APPLICATIONSSEARCH_H
