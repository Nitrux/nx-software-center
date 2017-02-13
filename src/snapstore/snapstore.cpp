#include "snapstore.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkProxy>
#include <QSslConfiguration>
#include <QByteArray>

#include <QDebug>

#include "../snapdsettings.h"
#include "snapstorelistdepartamentsrequest.h"

SnapStore::SnapStore(SnapdSettings *settings, QObject *parent): QObject(parent)
{
    Q_ASSERT_X(settings != NULL, "initialization", "NULL settings object");

    m_settings = settings;
    m_proxy.setType(QNetworkProxy::HttpProxy);

    m_proxy.setHostName(settings->httpProxy());
    m_proxy.setPort(settings->httpProxyPort());

    QNetworkProxy::setApplicationProxy(m_proxy);

    QSslConfiguration config(QSslConfiguration::defaultConfiguration());

    m_networkAccessManager.setProxy(m_proxy);
}

SnapStoreListDepartamentsRequest * SnapStore::getDepartments()
{
    QString storeUrl;
    if (m_settings->store() == SnapdSettings::Ubuntu)
        storeUrl = "https://search.apps.ubuntu.com/api/v1";
    else
        storeUrl = m_settings->customStoreUrl();

    return new SnapStoreListDepartamentsRequest(storeUrl, &m_networkAccessManager);
}


void SnapStore::onNetworkProxyHostChanged(QString host)
{
    if (m_settings->useProxy()) {
        m_networkAccessManager.setProxy(QNetworkProxy::NoProxy);
    } else {
        m_proxy.setHostName(host);
        m_networkAccessManager.setProxy(m_proxy);
    }
}

void SnapStore::onNetworkProxyPortChanged(int port)
{
    if (m_settings->useProxy()) {
        m_networkAccessManager.setProxy(QNetworkProxy::NoProxy);
    } else {
        m_proxy.setPort(port);
        m_networkAccessManager.setProxy(m_proxy);
    }
}
