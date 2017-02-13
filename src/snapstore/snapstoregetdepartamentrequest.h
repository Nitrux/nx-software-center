#ifndef SNAPSTOREGETDEPARTAMENTSNAPSREQUEST_H
#define SNAPSTOREGETDEPARTAMENTSNAPSREQUEST_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

#include "snapstorerequest.h"



class SnapStoreGetDepartamentRequest : public QObject
{
    Q_OBJECT
public:
    explicit SnapStoreGetDepartamentRequest(const QString &storeUrl, const QString &slug, QNetworkAccessManager *networkAccessManager, QObject *parent = 0);

    Q_INVOKABLE virtual void start();
    Q_INVOKABLE virtual void cancel();

    Q_INVOKABLE int highlightsCount();
    Q_INVOKABLE QVariantMap highlight(int idx);

    Q_INVOKABLE int packagesCount();
    Q_INVOKABLE QVariantMap package(int idx);
signals:
    void dataReady();

public slots:
    void onNetworkErrorResponse(QNetworkReply::NetworkError error);
    void onFinished(QNetworkReply* reply);

private:
    QString m_storeUrl;
    QString m_slug;
    QNetworkAccessManager *m_networkAccessManager;

    QList<QVariantMap> m_higlights;
    QList<QVariantMap> m_packages;
};

#endif // SNAPSTOREGETDEPARTAMENTSNAPSREQUEST_H
