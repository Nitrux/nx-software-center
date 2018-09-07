#ifndef NX_SOFTWARE_CENTER_DEPLOYEDAPPLICATIONSREGISTRY_H
#define NX_SOFTWARE_CENTER_DEPLOYEDAPPLICATIONSREGISTRY_H

#include <QDir>
#include <QList>
#include <QString>

#include <entities/AppImageInfo.h>
#include <entities/DeployedApplicationAbstract.h>

class DeployedApplicationsRegistry {

public:
    DeployedApplicationsRegistry();

    void setApplicationsDir(const QString &applicationsDir);

    void setCacheDir(const QString &cacheDir);

    AppImageInfo getApplicationInfo(const QString &filePath);

    QList<DeployedApplicationAbstract> listApplications();

    void registerFile(const QString &path, const AppImageInfo & info);

    AppImageInfo getLatestDeployedVersionOf(const QString &id);

protected:
    QDir applicationsDir;
    QDir cacheDir;
    QMap<QString, AppImageInfo> appImageInfoCache;
    QMap<QString, QString> filesHash;

    void synchronizeCache();

    QStringList listApplicationFiles();

    void extractFileInfo(const QString &path);

    QString getSha1(const QString &path) const;

    void writeAppImageInfo(const QVariantMap &metadata, const QDir &outputDir);

    void writeAppImageIcon(QByteArray icon, const QDir &outputDir);

    AppImageInfo getAppImageInfoFromCache(const QDir &entryDir) const;

    void syncFilesHashCache(const QStringList &files);

    void extractInfoFromNewlyDeployedFiles(const QStringList &files);

    void syncAppImageInfoCache();

    DeployedApplicationAbstract getAppImageInfoAbstract(const AppImageInfo &appImageInfo) const;

    void dropDeleteFilesCache();
};


#endif //NX_SOFTWARE_CENTER_DEPLOYEDAPPLICATIONSREGISTRY_H
