#include "registry.h"

#include <algorithm>

#include "change.h"

Registry::Registry()
{

}

Registry::~Registry()
{

}

Change *Registry::getChange(const QString &changeId) const
{
    return m_changes.value(changeId, nullptr);
}

bool Registry::registerChange(Change *change)
{
    if (change != nullptr) {
        change->attachRegistry(this);
        m_changes.insert(change->id, change);
        return true;
    } else
    {
        return false;
    }
}

void Registry::registerReleaseDownload(QString appId, QString releaseId, QString filePath)
{
    QString key = storeKey(appId, releaseId);
    if (!m_downloadedReleaseIds.contains(key))
        m_downloadedReleaseIds.insert(key, filePath);
}

void Registry::registerReleaseInstall(QString appId, QString releaseId, QStringList files)
{
    QString key = storeKey(appId, releaseId);
    if (!m_installedReleaseIds.contains(key))
        m_installedReleaseIds.insert(key, files);
}

void Registry::registerReleaseUninstall(QString appId, QString releaseId)
{
    m_installedReleaseIds.remove(storeKey(appId, releaseId));
}

void Registry::registerReleaseRemove(QString appId, QString releaseId)
{
    m_downloadedReleaseIds.remove(storeKey(appId, releaseId));
}

bool Registry::isReleaseDownloaded(QString appId, QString releaseId)
{
    QString key = storeKey(appId, releaseId);
    return m_downloadedReleaseIds.contains(key);
}

QString Registry::getReleaseFilePath(QString appId, QString releaseId)
{
    return m_downloadedReleaseIds.value(storeKey(appId, releaseId), QString());
}

QStringList Registry::getReleaseInstalleFilePaths(QString appId, QString releaseId)
{
    return m_installedReleaseIds.value(storeKey(appId, releaseId), QStringList());
}

QList<Change *> Registry::runningChanges()
{
    QList<Change *> running;
    for (Change * change: m_changes)
        if (change->status == Change::RUNNING)
            running.append(change);

    return running;
}

QList<Change *> Registry::allChanges()
{
    return m_changes.values();
}

QString Registry::storeKey(QString appId, QString releaseId)
{
    Q_ASSERT(!appId.isEmpty());
    Q_ASSERT(!releaseId.isEmpty());

    return appId + "_" + releaseId;
}
