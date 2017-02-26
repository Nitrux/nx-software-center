#include "customnetworkaccessmanagerfactory.h"

#include <QNetworkProxy>
#include <QNetworkAccessManager>

#include "../snapdsettings.h"

CustomNetworkAccessManagerFactory::CustomNetworkAccessManagerFactory(SnapdSettings *snapdSettings) : QObject(snapdSettings), QQmlNetworkAccessManagerFactory()
{
    Q_ASSERT_X(snapdSettings != NULL, "CustomNetworkAccessManager initialization","Invaling snapdSettings provided");
    m_settings = snapdSettings;
    connect(snapdSettings, &SnapdSettings::httpProxyChanged, this, &CustomNetworkAccessManagerFactory::updateProxies);
    connect(snapdSettings, &SnapdSettings::httpsProxyChanged, this, &CustomNetworkAccessManagerFactory::updateProxies);
    connect(snapdSettings, &SnapdSettings::noProxyChanged, this, &CustomNetworkAccessManagerFactory::updateProxies);
    connect(snapdSettings, &SnapdSettings::useProxyChanged, this, &CustomNetworkAccessManagerFactory::updateProxies);
}

QNetworkAccessManager *CustomNetworkAccessManagerFactory::create(QObject *parent) {
    QNetworkAccessManager *nam = new QNetworkAccessManager(parent);
    if (m_settings->useProxy()) {
        QNetworkProxy proxy = createProxy();
        nam->setProxy(proxy);
    }
    qDebug() << "Created QNetworkAccessManager " << nam->proxy();
    m_instances.append(QPointer<QNetworkAccessManager>(nam));
    return nam;
}

void CustomNetworkAccessManagerFactory::updateProxies()
{
    qDebug() << "refreshing proxyies";
    for (int i = 0; i < m_instances.size(); i ++) {
        QPointer<QNetworkAccessManager> pointer = m_instances.at(i);
        if (!pointer.isNull())
            pointer->setProxy(createProxy());
    }
}

QNetworkProxy CustomNetworkAccessManagerFactory::createProxy()
{
    if (!m_settings->useProxy())
        return QNetworkProxy();

    QString proxyHost = m_settings->httpProxy();
    int32_t proxyPort = m_settings->httpProxyPort();


    QNetworkProxy proxy(QNetworkProxy::HttpProxy, proxyHost, proxyPort);
    return proxy;
}
