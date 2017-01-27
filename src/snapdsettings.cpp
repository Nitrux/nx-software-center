#include "snapdsettings.h"


#include <QDebug>
#include <QSettings>

SnapdSettings::SnapdSettings()
{

}

bool SnapdSettings::useProxy() const
{
    return m_useProxy;
}

QString SnapdSettings::httpsProxy() const
{
    return m_httpsProxy;
}

QString SnapdSettings::httpProxy() const
{
    return m_httpProxy;
}

QString SnapdSettings::noProxy() const
{
    return m_noProxy;
}

SnapdSettings::Store SnapdSettings::store() const
{
    return m_store;
}

QString SnapdSettings::customStoreUrl() const
{
    return m_customStoreUrl;
}

void SnapdSettings::setUseProxy(bool useProxy)
{
    if (m_useProxy == useProxy)
        return;

    m_useProxy = useProxy;
    emit useProxyChanged(useProxy);
}

void SnapdSettings::setHttpsProxy(QString httpsProxy)
{
    if (m_httpsProxy == httpsProxy)
        return;

    m_httpsProxy = httpsProxy;
    emit httpsProxyChanged(httpsProxy);
}

void SnapdSettings::setHttpProxy(QString httpProxy)
{
    if (m_httpProxy == httpProxy)
        return;

    m_httpProxy = httpProxy;
    emit httpProxyChanged(httpProxy);
}

void SnapdSettings::setNoProxy(QString noProxy)
{
    if (m_noProxy == noProxy)
        return;

    m_noProxy = noProxy;
    emit noProxyChanged(noProxy);
}

void SnapdSettings::setStore(SnapdSettings::Store store)
{
    if (m_store == store)
        return;

    m_store = store;
    emit storeChanged(store);
}

void SnapdSettings::setCustomStoreUrl(QString customStoreUrl)
{
    if (m_customStoreUrl == customStoreUrl)
        return;

    m_customStoreUrl = customStoreUrl;
    emit customStoreUrlChanged(customStoreUrl);
}

void SnapdSettings::load()
{
    QSettings settings("/etc/environment", QSettings::IniFormat);
    //    qDebug() << settings.
    for (QString key: settings.allKeys())
        qDebug() << key << settings.value(key);

    // Set ubuntu as default store
    m_store = Ubuntu;

    if (settings.contains("SNAPPY_FORCE_CPI_URL")) {
        m_store = Custom;
        m_customStoreUrl = settings.value("SNAPPY_FORCE_CPI_URL", "").toString();
    }

    if (settings.contains("#SNAPPY_FORCE_CPI_URL")) {
        m_customStoreUrl = settings.value("SNAPPY_FORCE_CPI_URL").toString();
    }

    m_httpProxy = settings.value("HTTP_PROXY", "").toString();
    m_httpsProxy = settings.value("HTTPS_PROXY", "").toString();
    QStringList noProxyList = settings.value("NO_PROXY").toStringList();

    if (m_httpProxy.isEmpty() && m_httpsProxy.isEmpty())
        m_useProxy = false;

    if (m_httpProxy.isEmpty())
        m_httpProxy = settings.value("#HTTP_PROXY", "").toString();

    if (m_httpsProxy.isEmpty())
        m_httpsProxy = settings.value("#HTTPS_PROXY", "").toString();

    if (noProxyList.isEmpty())
        noProxyList = settings.value("#NO_PROXY", "").toStringList();

    m_noProxy = noProxyList.join(", ");

    emit storeChanged(m_store);
    emit customStoreUrlChanged(m_customStoreUrl);
    emit useProxyChanged(m_useProxy);
    emit httpProxyChanged(m_httpProxy);
    emit httpsProxyChanged(m_httpsProxy);
    emit noProxyChanged(m_noProxy);
}

KAuth::ExecuteJob *SnapdSettings::apply()
{
    KAuth::Action action("org.nomad.softwarecenter.applysettings");
    action.setHelperId("org.nomad.softwarecenter");

    action.addArgument("store", m_store);
    action.addArgument("storeUrl", m_customStoreUrl);

    action.addArgument("useProxy", m_useProxy);
    action.addArgument("httpProxy", m_httpProxy);
    action.addArgument("httpsProxy", m_httpsProxy);
    action.addArgument("noProxy", m_noProxy);


    KAuth::ExecuteJob *job = action.execute();
    connect(job, &KAuth::ExecuteJob::result, [=] (KJob *kjob) {
        auto job = qobject_cast<KAuth::ExecuteJob *>(kjob);
        qDebug() << QString("Apply settings finished, errors ? ") << job->errorString() << job->error();
    });

    return job;
}
