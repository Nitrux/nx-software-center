#include "registry.h"

#include <algorithm>

#include "change.h"

Registry::Registry()
{

}

QList<QString> Registry::downloadedReleaseIds()
{
    return m_downloadedReleaseIds;
}

QList<QString> Registry::installedReleaseIds()
{
    return m_installedReleaseIds;
}

Change *Registry::getChange(const QString &changeId)
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

void Registry::registerReleaseDownload(QString releaseId)
{

    QList<QString>::iterator iter = std::find(m_downloadedReleaseIds.begin(), m_downloadedReleaseIds.end(), releaseId);
    if (iter == m_downloadedReleaseIds.end())
        m_downloadedReleaseIds.push_back(releaseId);
}

void Registry::registerReleaseInstall(QString releaseId)
{
    QList<QString>::iterator iter = std::find(m_installedReleaseIds.begin(), m_installedReleaseIds.end(), releaseId);
    if (iter == m_installedReleaseIds.end())
        m_installedReleaseIds.push_back(releaseId);
}

void Registry::registerReleaseUninstall(QString releaseId)
{
    m_installedReleaseIds.removeAll(releaseId);
}

void Registry::registerReleaseRemove(QString releaseId)
{
    m_downloadedReleaseIds.removeAll(releaseId);
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
