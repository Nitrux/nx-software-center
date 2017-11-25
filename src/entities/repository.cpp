#include "repository.h"

#include "app.h"
#include "release.h"

Repository::Repository()
{

}

Repository::~Repository()
{

}

bool Repository::contains(QString appId)
{
    return apps.contains(appId);
}

bool Repository::contains(QString appId, QString releaseId)
{
    if (apps.contains(appId))
    {
        App * app = apps.value(appId);
        return app->getRelease(releaseId) != nullptr;
    } else
        return false;
}

App *Repository::getApp(QString appId)
{
    return apps.value(appId, nullptr);
}

Release *Repository::getRelease(QString appId, QString releaseId)
{
    App * app = apps.value(appId);
    if (app == nullptr)
        return nullptr;

    return app->getRelease(releaseId);
}

void Repository::add(App *app)
{
    Q_ASSERT(app != nullptr);
    apps.insert(app->id, app);
}

void Repository::clear()
{
    apps.clear();
}

QList<App *> Repository::list()
{
    QList<App *> appList;
    for (auto it = apps.begin(); it != apps.end(); it ++)
    {
        appList.append(it.value());
    }
    return appList;
}
