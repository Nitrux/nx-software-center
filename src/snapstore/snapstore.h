#ifndef SNAPSTORE_H
#define SNAPSTORE_H

#include <QNetworkAccessManager>
#include <QNetworkProxy>

#include <QObject>

#include "snapstorelistdepartamentsrequest.h"
#include "snapstoregetdepartamentrequest.h"

class SnapdSettings;

class SnapStore : public QObject
{
    Q_OBJECT
public:
    SnapStore(SnapdSettings *settings, QObject * parent = NULL);

    Q_INVOKABLE SnapStoreListDepartamentsRequest *listDepartments();
    Q_INVOKABLE SnapStoreGetDepartamentRequest *getDepartment(const QString &slug);
//    Q_INVOKABLE SnapStoreRequest getSnapDetails();


    QString storeUrl();
    QNetworkAccessManager * networkAccessManager();

protected slots:
    void onNetworkProxyHostChanged(QString host);
    void onNetworkProxyPortChanged(int port);
    void onStoreChanged();

private:
    QString m_storeUrl;
    SnapdSettings * m_settings;
    QNetworkProxy m_proxy;
    QNetworkAccessManager m_networkAccessManager;
};

#endif // SNAPSTORE_H
