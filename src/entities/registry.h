#ifndef REGISTRY_H
#define REGISTRY_H

#include <QMap>
#include <QList>
#include <QString>

class Change;

class Registry
{
public:
    Registry();

    QList<QString> downloadedReleaseIds();
    QList<QString> installedReleaseIds();

    Change* getChange(const QString &changeId);
    bool registerChange(Change *change);
    void registerReleaseDownload(QString releaseId);
    void registerReleaseInstall(QString releaseId);
    void registerReleaseUninstall(QString releaseId);
    void registerReleaseRemove(QString releaseId);

    QList<Change *> runningChanges();
    QList<Change *> allChanges();

protected:
    QMap<QString, Change *> m_changes;

    QList<QString> m_downloadedReleaseIds;
    QList<QString> m_installedReleaseIds;
};

#endif // REGISTRY_H
