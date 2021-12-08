#include <QDebug>
#include <utility>

#include "ApplicationsRegistry.h"

ApplicationsRegistry::ApplicationsRegistry(QStringList appDirs)
    : _appDirs(std::move(appDirs))
{
}
void ApplicationsRegistry::addBundle(const ApplicationBundle &bundle)
{
    auto appId = bundle.app->getId();
    if (_applications.contains(appId)) {
        auto &app = _applications[appId];
        app.addBundle(bundle);

        emit(applicationUpdated(app));
    } else {
        ApplicationData data;
        data.addBundle(bundle);
        _applications.insert(appId, data);

        emit(applicationAdded(data));
    }
}
ApplicationData ApplicationsRegistry::getApplication(const QString &appId) const
{
    return _applications.value(appId);
}
void ApplicationsRegistry::removeBundle(const QString &path)
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
    if (bundle.app.isNull()) {
        qWarning() << "Unable to remove bundle " << bundle.path << " as it doesn't contain application data.";
        return;
    }

    auto appId = bundle.app->getId();
    if (_applications.contains(appId)) {
        auto &app = _applications[appId];
        app.removeBundle(bundle);

        if (app.getBundles().length() == 0) {
            auto deletedApp = _applications.take(appId);
            emit(applicationRemoved(deletedApp));
        } else {
            emit(applicationUpdated(app));
        }
    } else {
        qWarning() << "Unable to remove bundle " << bundle.path << " as it isn't found.";
    }
}
bool ApplicationsRegistry::applicationExist(const QString &appId) const
{
    return _applications.contains(appId);
}
