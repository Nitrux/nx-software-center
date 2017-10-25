#ifndef SYSTEM_H
#define SYSTEM_H

#include <list>
#include <string>

#include <QList>
#include <QString>
#include <QVariant>
#include <QVariantMap>

class Registry;
class Repository;

class System
{
public:
    System();

    void setRegistry(Registry *registry);
    void addRepository(Repository *repository);

    QList<QString> listAppIds();
    QVariantMap appDetails(QString appId);

    QList<QString> listAppReleaseIds(QString appId);
    QVariantMap releaseDetails(QString appId, QString releaseId);

    QString downloadRelease(QString appId, QString releaseId);
    QString installRelease(QString appId, QString releaseId);
    QString uninstallRelease(QString appId, QString releaseId);
    QString removeRelease(QString appId, QString releaseId);

protected:
    std::list<Repository *> repositories;
    Registry *registry = nullptr;
};

#endif // SYSTEM_H
