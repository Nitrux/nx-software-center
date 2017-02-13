#ifndef SNAPSTORELISTDEPARTMENTSREQUEST_H
#define SNAPSTORELISTDEPARTMENTSREQUEST_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "snapstorerequest.h"

class SnapStoreListDepartamentsRequest : public QObject
{
    Q_OBJECT
public:
    SnapStoreListDepartamentsRequest(const QString &storeUrl, QNetworkAccessManager *networkAccessManager, QObject * parent = 0);
    Q_INVOKABLE virtual void start();
    Q_INVOKABLE virtual void cancel();

    Q_INVOKABLE int departamentCount() const;
    Q_INVOKABLE QVariantMap departament(int idx) const;

signals:
    void finished();

public slots:
    void onNetworkErrorResponse(QNetworkReply::NetworkError error);
    void onFinished(QNetworkReply* reply);

private:
    QNetworkAccessManager * m_networkAccessManager;
    QList<QVariantMap> m_result;
    QString m_url;
    QString m_errorString;
};

#endif // SNAPSTORELISTDEPARTMENTSREQUEST_H
