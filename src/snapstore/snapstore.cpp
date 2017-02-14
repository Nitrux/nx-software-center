#include "snapstore.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkProxy>
#include <QSslConfiguration>
#include <QByteArray>

#include <QDebug>

#include "../snapdsettings.h"


SnapStore::SnapStore(SnapdSettings *settings, QObject *parent): QObject(parent)
{
    Q_ASSERT_X(settings != NULL, "initialization", "NULL settings object");

    m_settings = settings;
    connect(m_settings, &SnapdSettings::storeChanged, this, &SnapStore::onStoreChanged);
    onStoreChanged();

    m_proxy.setType(QNetworkProxy::HttpProxy);
    m_proxy.setHostName(settings->httpProxy());
    m_proxy.setPort(settings->httpProxyPort());

    QNetworkProxy::setApplicationProxy(m_proxy);

    QSslConfiguration config(QSslConfiguration::defaultConfiguration());

    m_networkAccessManager.setProxy(m_proxy);
}

SnapStoreListDepartamentsRequest * SnapStore::listDepartments()
{
    return new SnapStoreListDepartamentsRequest(this);
}

SnapStoreGetDepartamentRequest * SnapStore::getDepartment(const QString &slug)
{
    SnapStoreGetDepartamentRequest * request = new SnapStoreGetDepartamentRequest(slug, this);
    return request;
}

SnapStoreSnapDetailsRequest *SnapStore::getSnapDetails(const QString &packageName)
{
    return new SnapStoreSnapDetailsRequest(packageName, this);
}

QString SnapStore::storeUrl()
{
    return m_storeUrl;
}

QNetworkAccessManager *SnapStore::networkAccessManager()
{
    return &m_networkAccessManager;
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

void SnapStore::onStoreChanged()
{
    if (m_settings->store() == SnapdSettings::Ubuntu)
        m_storeUrl = "https://search.apps.ubuntu.com/api/v1";
    else
        m_storeUrl = m_settings->customStoreUrl();
}
