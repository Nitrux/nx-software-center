#include "dummyrepository.h"

#include "dummyrelease.h"
#include "../entities/app.h"

DummyRepository::DummyRepository(): Repository()
{

}

QString DummyRepository::id()
{
    return "DummyRepository";
}

void DummyRepository::updateCache()
{
    apps.clear();

    for (int i = 0; i < 5; i ++)
    {
        QString appId = QString("app_%1").arg(i);
        App * app = new App(appId);
        app->addRelease(new DummyRelease("r1.0.0", appId));
        app->addRelease(new DummyRelease("r2.0.0", appId));

        apps.insert(appId, app);
    }
}
