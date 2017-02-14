#ifndef SNAPSTORESNAPDETAILSREQUEST_H
#define SNAPSTORESNAPDETAILSREQUEST_H

#include <QVariantMap>

#include "snapstorerequest.h"

class SnapStore;
class SnapStoreSnapDetailsRequest : public SnapStoreRequest
{
    Q_OBJECT
public:
    explicit SnapStoreSnapDetailsRequest(const QString &packageName, SnapStore * store);

    Q_INVOKABLE void runAsync();
    Q_INVOKABLE void cancel();
    Q_INVOKABLE QVariantMap snapDetails();

signals:

public slots:

    void onNetworkRequestFinished();
private:
    QString m_packageName;
    QNetworkReply* m_reply;
    QVariantMap m_snapDetails;
};

#endif // SNAPSTORESNAPDETAILSREQUEST_H
