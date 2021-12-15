#pragma once
// system

// libraries
#include <QMetaType>
#include <QObject>
#include <QStringList>

// local
#include "ApplicationBundle.h"
#include "ApplicationData.h"

/**
 * Stores data of the applications available to the user
 */
class ApplicationsRegistry : public QObject
{
    Q_OBJECT

public:
    explicit ApplicationsRegistry(QStringList appDirs);

    const QStringList &getAppDirs();
    bool applicationExist(const QString &appId) const;
    ApplicationData getApplication(const QString &appId) const;

    QList<ApplicationData> getApplications() const;
    int getApplicationsCount() const;

    // Add an AppImage file to the registry and updates or create the related Application data
    Q_SLOT void addBundle(const ApplicationBundle &bundle);

    // removes an AppImage file from the registry update the related Application data
    Q_SLOT void removeBundle(const ApplicationBundle &bundle);
    Q_SLOT void removeBundleByPath(const QString &path);

public:
    Q_SIGNAL void applicationAdded(ApplicationData);
    Q_SIGNAL void applicationRemoved(ApplicationData);
    Q_SIGNAL void applicationUpdated(ApplicationData);

    QMap<QString, ApplicationData> _applications;

private:
    // directories to look for AppImages
    QStringList _appDirs;
};
