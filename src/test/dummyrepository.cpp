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
        for (int j = 0; j < 3; j ++) {
            QString releaseId = QString("r%1").arg(j);
            Release *release = new DummyRelease(releaseId, appId);
            release->version = releaseId;
            release->arch = "x86_64";
            release->name = appName + QString(" %1").arg(i);
            release->description = "Dummy app";
            release->download_link = "https://github.com/AppImage/AppImageUpdate/releases/download/continuous/AppImageUpdate-x86_64.AppImage";
            app->addRelease(release);
        }

        apps.insert(appId, app);
    }
}
