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
    explicit ApplicationsRegistry(QStringList appDirs, QMap<QString, ApplicationData> applications);

    const QStringList &getAppDirs();
    [[nodiscard]] bool applicationExist(const QString &appId) const;
    [[nodiscard]] ApplicationData getApplication(const QString &appId) const;

    [[nodiscard]] QList<ApplicationData> getApplications() const;
    [[nodiscard]] int getApplicationsCount() const;

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
