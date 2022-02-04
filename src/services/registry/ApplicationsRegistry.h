#pragma once
// system

// libraries
#include <QMetaType>
#include <QObject>
#include <QStringList>

// local
#include "../Application.h"
#include "../ApplicationBundle.h"

/**
 * Stores data of the applications available to the user
 */
class ApplicationsRegistry : public QObject
{
    Q_OBJECT

public:
    explicit ApplicationsRegistry(QStringList appDirs);

    [[nodiscard]] bool applicationExist(const QString &appId) const;
    [[nodiscard]] Application getApplication(const QString &appId) const;
    Q_SCRIPTABLE void updateApplication(const Application &application);

    [[nodiscard]] Q_SCRIPTABLE ApplicationsList getApplications() const;
    void setApplications(const QVector<Application> &applications);
    int countApplications();

    // Add an AppImage file to the registry and updates or create the related Application data
    Q_SLOT void addBundle(const ApplicationBundle &bundle);

    // removes an AppImage file from the registry update the related Application data
    Q_SLOT void removeBundle(const ApplicationBundle &bundle);
    Q_SLOT void removeBundleByPath(const QString &path);

    Q_SIGNAL void applicationAdded(Application);
    Q_SIGNAL void applicationRemoved(Application);
    Q_SIGNAL void applicationUpdated(Application);

    QMap<QString, Application> _applications;

private:
    // directories to look for AppImages
    QStringList _appDirs;
};
