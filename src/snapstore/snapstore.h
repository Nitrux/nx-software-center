#ifndef SNAPSTORE_H
#define SNAPSTORE_H

#include <QNetworkAccessManager>
#include <QNetworkProxy>

#include <QObject>

#include "snapstorelistdepartamentsrequest.h"

class SnapdSettings;

class SnapStore : public QObject
{
    Q_OBJECT
public:
    SnapStore(SnapdSettings *settings, QObject * parent = NULL);

    Q_INVOKABLE SnapStoreListDepartamentsRequest *getDepartments();
//    Q_INVOKABLE void getDepartmentSnaps(QString department);
//    Q_INVOKABLE SnapStoreRequest getSnapDetails();


protected slots:
    void onNetworkProxyHostChanged(QString host);
    void onNetworkProxyPortChanged(int port);

private:
    SnapdSettings * m_settings;
    QNetworkProxy m_proxy;
    QNetworkAccessManager m_networkAccessManager;
};

#endif // SNAPSTORE_H
