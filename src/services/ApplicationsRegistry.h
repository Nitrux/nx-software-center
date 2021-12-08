#pragma once
// system

// libraries
#include <QStringList>

// local
#include "ApplicationBundle.h"
#include "ApplicationData.h"

/**
 * Stores data of the applications available to the user
 */
class ApplicationsRegistry
{
public:
    explicit ApplicationsRegistry(QStringList appDirs);

    ApplicationData getApplication(const QString& appId) const;

    // Add an AppImage file to the registry and updates or create the related Application data
    void addBundle(const QSharedPointer<ApplicationBundle> &bundle);

    // removes an AppImage file from the registry update the related Application data
    void removeBundle(const QSharedPointer<ApplicationBundle> &bundle);
    void removeBundle(const QString &path);

public:
    Q_SIGNAL void applicationAdded(ApplicationData);
    Q_SIGNAL void applicationRemoved(ApplicationData);
    Q_SIGNAL void applicationUpdate(ApplicationData);

    QMap<QString, ApplicationData> _applications;

private:
    // directories to look for AppImages
    QStringList _appDirs;
};
