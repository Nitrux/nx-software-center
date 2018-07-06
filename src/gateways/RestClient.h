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

class ApplicationsSearchRequest;
class GetApplicationRequest;
class RestClient : public QObject {
Q_OBJECT
    QString api;
    QNetworkAccessManager* networkAccessManager;
public:
    explicit RestClient(QString url, QObject* parent = nullptr);
    ApplicationsSearchRequest* buildSearchRequest(const QString& query = "", const QString& category = "") const;
    GetApplicationRequest* buildGetApplicationRequest(const QString& id) const;
    QNetworkAccessManager* getNetworkAccessManager() const;
    void setNetworkAccessManager(QNetworkAccessManager* networkAccessManager);
};

#endif //NX_SOFTWARE_CENTER_EXPLORER_H
