#include "snapstore.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkProxy>
#include <QSslConfiguration>
#include <QByteArray>

#include <QDebug>

#include "../snapdsettings.h"
#include "customnetworkaccessmanagerfactory.h"

SnapStore::SnapStore(SnapdSettings *settings, QObject *parent): QObject(parent)
{
    Q_ASSERT_X(settings != NULL, "initialization", "NULL settings object");

    m_settings = settings;
    connect(m_settings, &SnapdSettings::storeChanged, this, &SnapStore::onStoreChanged);
    onStoreChanged();

    m_networkAccessManagerFactory = NULL;
    m_networkAccessManager = NULL;
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
    if (m_networkAccessManager != NULL)
        return m_networkAccessManager;

    if (m_networkAccessManagerFactory == NULL) {
        m_networkAccessManager = new QNetworkAccessManager(this);
    } else {
        m_networkAccessManager = m_networkAccessManagerFactory->create(this);
    }

    return m_networkAccessManager;
}

void SnapStore::setNetworkAccessManagerFactory(QQmlNetworkAccessManagerFactory *factory)
{
    m_networkAccessManagerFactory = factory;
}

void SnapStore::onStoreChanged()
{
    if (m_settings->store() == SnapdSettings::Ubuntu)
        m_storeUrl = "https://search.apps.ubuntu.com/api/v1";
    else
        m_storeUrl = m_settings->customStoreUrl();
}
