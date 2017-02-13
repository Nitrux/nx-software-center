#ifndef SNAPSTORELISTDEPARTMENTSREQUEST_H
#define SNAPSTORELISTDEPARTMENTSREQUEST_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "snapstorerequest.h"

class SnapStore;

class SnapStoreListDepartamentsRequest : public SnapStoreRequest
{
    Q_OBJECT
public:
    SnapStoreListDepartamentsRequest(SnapStore * snapStore);
    ~SnapStoreListDepartamentsRequest();

    Q_INVOKABLE virtual void runAsync();
    Q_INVOKABLE virtual void cancel();

    Q_INVOKABLE int departamentCount() const;
    Q_INVOKABLE QVariantMap departament(int idx) const;


    void onRequestFinished();

private:
    QNetworkReply* m_reply;
    QList<QVariantMap> m_result;
    QString m_errorString;
};

#endif // SNAPSTORELISTDEPARTMENTSREQUEST_H
