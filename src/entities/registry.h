#ifndef REGISTRY_H
#define REGISTRY_H

#include <QMap>
#include <QString>

class Change;

class Registry
{
public:
    Registry();

    Change* getChange(const QString &changeId) const;
    bool registerChange(Change *change);
    void registerReleaseDownload(QString appId, QString releaseId, QString filePath);
    void registerReleaseInstall(QString appId, QString releaseId, QStringList files);
    void registerReleaseUninstall(QString appId, QString releaseId);
    void registerReleaseRemove(QString appId, QString releaseId);

    QString getReleaseFilePath(QString appId, QString releaseId);
    QStringList getReleaseInstalleFilePaths(QString appId, QString releaseId);

    QList<Change *> runningChanges();
    QList<Change *> allChanges();

protected:
    QMap<QString, Change *> m_changes;

    QMap<QString, QString> m_downloadedReleaseIds;
    QMap<QString, QStringList> m_installedReleaseIds;
};

#endif // REGISTRY_H
