#ifndef REGISTRY_H
#define REGISTRY_H

#include <QMap>
#include <QString>

class Change;

class Registry
{
public:
    Registry();
    virtual ~Registry();

    Change* getChange(const QString &changeId) const;
    virtual bool registerChange(Change *change);
    virtual void registerReleaseDownload(QString appId, QString releaseId, QString filePath);
    virtual void registerReleaseInstall(QString appId, QString releaseId, QStringList files);
    virtual void registerReleaseUninstall(QString appId, QString releaseId);
    virtual void registerReleaseRemove(QString appId, QString releaseId);

    bool isReleaseDownloaded(QString appId, QString releaseId);
    QString getReleaseFilePath(QString appId, QString releaseId);
    QStringList getReleaseInstalleFilePaths(QString appId, QString releaseId);

    QList<Change *> runningChanges();
    QList<Change *> allChanges();

protected:
    QString storeKey(QString appId, QString releaseId);
    QMap<QString, Change *> m_changes;

    QMap<QString, QString> m_downloadedReleaseIds;
    QMap<QString, QStringList> m_installedReleaseIds;
};

#endif // REGISTRY_H
