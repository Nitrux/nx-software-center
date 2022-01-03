#include <QDebug>
#include <utility>

#include "ApplicationsRegistry.h"

ApplicationsRegistry::ApplicationsRegistry(QStringList appDirs)
    : _appDirs(std::move(appDirs))
    , _appsDBHelper(_appsDBHelper->getInstance())
{
    qRegisterMetaType<ApplicationData>();
    qRegisterMetaType<ApplicationBundle>();

    initApplicationsList();
}
void ApplicationsRegistry::addBundle(const ApplicationBundle &bundle)
{
    auto appId = bundle.app->getId();
    if (_applications.contains(appId)) {
        auto &app = _applications[appId];
        app.addBundle(bundle);

        // Update app to db cache
        _appsDBHelper->saveOrUpdateApp(&app);

        emit(applicationUpdated(app));
    } else {
        ApplicationData data;
        data.addBundle(bundle);
        _applications.insert(appId, data);

        // Insert app to db cache
        _appsDBHelper->saveOrUpdateApp(&data);

        emit(applicationAdded(data));
    }
}
ApplicationData ApplicationsRegistry::getApplication(const QString &appId) const
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

            // Delete app from db cache
            _appsDBHelper->deleteApp(&deletedApp);

            emit(applicationRemoved(deletedApp));
        } else {
            // Update app to db cache
            _appsDBHelper->saveOrUpdateApp(&app);

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
const QStringList &ApplicationsRegistry::getAppDirs()
{
    return _appDirs;
}

QList<ApplicationData> ApplicationsRegistry::getApplications() const
{
    return _applications.values();
}

int ApplicationsRegistry::getApplicationsCount() const
{
    return _applications.size();
}

void ApplicationsRegistry::initApplicationsList()
{
    QList<ApplicationData> _apps = _appsDBHelper->getApps();

    foreach (ApplicationData app, _apps) {
        QString appId = app.getId();
        _applications.insert(appId, app);
	}
}