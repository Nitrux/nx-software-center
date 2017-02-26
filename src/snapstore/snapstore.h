#ifndef SNAPSTORE_H
#define SNAPSTORE_H

#include <QNetworkAccessManager>
#include <QNetworkProxy>
#include <QQmlNetworkAccessManagerFactory>
#include <QObject>

#include "snapstorelistdepartamentsrequest.h"
#include "snapstoregetdepartamentrequest.h"
#include "snapstoresnapdetailsrequest.h"

class SnapdSettings;

class SnapStore : public QObject
{
    Q_OBJECT
public:
    SnapStore(SnapdSettings *settings, QObject * parent = NULL);

    Q_INVOKABLE SnapStoreListDepartamentsRequest *listDepartments();
    Q_INVOKABLE SnapStoreGetDepartamentRequest *getDepartment(const QString &slug);
    Q_INVOKABLE SnapStoreSnapDetailsRequest *getSnapDetails(const QString &packageName);


    QString storeUrl();
    QNetworkAccessManager * networkAccessManager();
    void setNetworkAccessManagerFactory(QQmlNetworkAccessManagerFactory * factory);

protected slots:
    void onStoreChanged();

private:
    QString m_storeUrl;
    SnapdSettings * m_settings;
    QNetworkProxy m_proxy;

    QQmlNetworkAccessManagerFactory * m_networkAccessManagerFactory;
    QNetworkAccessManager * m_networkAccessManager;
};

#endif // SNAPSTORE_H
