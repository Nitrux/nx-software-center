#include "simplefileregistry.h"

#include <QSettings>

SimpleFileRegistry::SimpleFileRegistry(): Registry()
{
    QSettings settings;

    settings.beginGroup("AppImages_Downloaded");
    QStringList keys = settings.allKeys();
    for (QString key: keys)
        m_downloadedReleaseIds[key] = settings.value(key).toString();
    settings.endGroup();
}

void SimpleFileRegistry::registerReleaseDownload(QString appId, QString releaseId, QString filePath)
{
    Registry::registerReleaseDownload(appId, releaseId, filePath);
    save();
}

void SimpleFileRegistry::registerReleaseRemove(QString appId, QString releaseId)
{
    Registry::registerReleaseRemove(appId, releaseId);
    save();
}

void SimpleFileRegistry::save()
{
    QSettings settings;

    settings.beginGroup("AppImages_Downloaded");
    settings.remove("");
    QStringList keys = m_downloadedReleaseIds.keys();
    for (QString key: keys)
        settings.setValue(key, m_downloadedReleaseIds[key]);
    settings.endGroup();
}
