#ifndef SNAPSTORELISTDEPARTMENTSREQUEST_H
#define SNAPSTORELISTDEPARTMENTSREQUEST_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "snapstorerequest.h"

class SnapStoreListDepartamentsRequest : public SnapStoreRequest
{
    Q_OBJECT
public:
    SnapStoreListDepartamentsRequest(const QString &storeUrl, QNetworkAccessManager *networkAccessManager);
    Q_INVOKABLE virtual void start();
    Q_INVOKABLE virtual void cancel();

    Q_INVOKABLE int departamentCount() const;
    Q_INVOKABLE QVariantMap departament(int idx) const;

public slots:
    void onNetworkErrorResponse(QNetworkReply::NetworkError error);
    void onFinished(QNetworkReply* reply);

private:
    QNetworkAccessManager * m_networkAccessManager;
    QList<QVariantMap> m_result;
};

#endif // SNAPSTORELISTDEPARTMENTSREQUEST_H
