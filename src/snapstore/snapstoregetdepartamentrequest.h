#ifndef SNAPSTOREGETDEPARTAMENTSNAPSREQUEST_H
#define SNAPSTOREGETDEPARTAMENTSNAPSREQUEST_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

#include "snapstorerequest.h"


class SnapStore;

class SnapStoreGetDepartamentRequest : public SnapStoreRequest
{
    Q_OBJECT
public:
    explicit SnapStoreGetDepartamentRequest(const QString &slug, SnapStore * snapStore);

    Q_INVOKABLE virtual void runAsync();
    Q_INVOKABLE virtual void cancel();

    Q_INVOKABLE int highlightsCount();
    Q_INVOKABLE QVariantMap highlight(int idx);

    Q_INVOKABLE int packagesCount();
    Q_INVOKABLE QVariantMap package(int idx);

public slots:
    void onNetworkRequestFinished();

private:
    QString m_slug;
    QNetworkReply* m_reply;
    QList<QVariantMap> m_higlights;
    QList<QVariantMap> m_packages;
};

#endif // SNAPSTOREGETDEPARTAMENTSNAPSREQUEST_H
