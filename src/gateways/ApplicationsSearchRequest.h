//
// Created by alexis on 7/4/18.
//

#ifndef NX_SOFTWARE_CENTER_APPLICATIONSSEARCH_H
#define NX_SOFTWARE_CENTER_APPLICATIONSSEARCH_H

#include <QtNetwork/QNetworkAccessManager>
#include <entities/ApplicationAbstract.h>

class ApplicationsSearchRequest : public QObject {
Q_OBJECT
    QString api;
    QString query;
    QString category;
    bool running;
    QNetworkAccessManager* networkAccessManager;
    QNetworkReply* reply;
    QList<ApplicationAbstract> results;
protected:
    friend class RestClient;
    ApplicationsSearchRequest();
    void setNetworkAccessManager(QNetworkAccessManager* networkAccessManager);
    void setApi(const QString& api);

public:
    void setQuery(const QString& query);
    void setCategory(const QString& category);
    const QList<ApplicationAbstract>& getResults() const;

    QUrl getUrl() const;
public slots:

    void start();
    void stop();
signals:
    void resultsReady();

    void failed(const QString& reason);
protected slots:

    void handleReplyFinished();
protected:
    void trySetRunning();
    QList<ApplicationAbstract> parseResponse(const QByteArray& response) const;
    ApplicationAbstract getAbstract(const QVariantMap& vMap) const;
};

#endif //NX_SOFTWARE_CENTER_APPLICATIONSSEARCH_H
