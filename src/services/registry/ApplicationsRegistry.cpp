#include "ApplicationsRegistry.h"

// libraries
#include <QDebug>
#include <QVector>

// local
#include "utils/appimagetools.h"

ApplicationsRegistry::ApplicationsRegistry(QStringList appDirs, QMap<QString, Application> applications)
    : _applications(applications)
    , _appDirs(std::move(appDirs))
{
    qRegisterMetaType<Application>();
    qRegisterMetaType<ApplicationBundle>();
}
void ApplicationsRegistry::addBundle(const ApplicationBundle &bundle)
{
    auto appId = bundle.data.getId();
    if (_applications.contains(appId)) {
        auto &app = _applications[appId];
        app.addBundle(bundle);

        emit(applicationUpdated(app));
    } else {
        Application app;
        app.addBundle(bundle);
        _applications.insert(appId, app);

        emit(applicationAdded(app));
    }
}
Application ApplicationsRegistry::getApplication(const QString &appId) const
{
    return _applications.value(appId);
}
void ApplicationsRegistry::removeBundleByPath(const QString &path)
{
    ApplicationBundle targetBundle;

    const auto &apps = _applications.values();
    for (const auto &app : apps) {
        const auto &bundles = app.getBundles();
        for (const auto &bundle : bundles) {
            if (bundle.path == path) {
                targetBundle = bundle;
                break;
            }
        }
    }

    if (!targetBundle.path.isEmpty())
        removeBundle(targetBundle);
    else
        qWarning() << "Unable to remove bundle " << path << " as it isn't found.";
}
void ApplicationsRegistry::removeBundle(const ApplicationBundle &bundle)
{
    auto appId = bundle.data.getId();
    if (_applications.contains(appId)) {
        auto &app = _applications[appId];
        app.removeBundle(bundle);

        if (app.getBundles().length() == 0) {
            auto deletedApp = _applications.take(appId);

            emit(applicationRemoved(deletedApp));
        } else {
            emit(applicationUpdated(app));
        }

        // remove deleted bundle integration
        AppImageTools::unintegrate(QUrl::fromLocalFile(bundle.path));
    } else {
        qWarning() << "Unable to remove bundle " << bundle.path << " as it isn't found.";
    }
}
bool ApplicationsRegistry::applicationExist(const QString &appId) const
{
    return _applications.contains(appId);
}
const QStringList &ApplicationsRegistry::getAppDirs()
{
    return _appDirs;
}

ApplicationsList ApplicationsRegistry::getApplications() const
{
    return QVector<Application>::fromList(_applications.values());
}

int ApplicationsRegistry::getApplicationsCount() const
{
    return _applications.size();
}
void ApplicationsRegistry::updateApplicationData(const Application &applicationData)
{
    const auto &appId = applicationData.getId();
    if (_applications.contains(appId)) {
        _applications[appId] = applicationData;
        emit(applicationUpdated(applicationData));
    }
}
