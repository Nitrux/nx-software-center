#include "InstallService.h"
#include "InstallWorker.h"

#include <QDebug>
#include <models/app.h>

InstallService::InstallService(const QString &applicationsDir, QObject *parent)
    : QObject(parent)
    , _applicationsDir(applicationsDir)
    , _partialsDir(applicationsDir + "/.partials")
{
    if (!_partialsDir.exists())
        _partialsDir.mkpath(".");
}

void InstallService::installFromPlingStore(const QUrl &appimageUrl, PlingStoreApplicationData *plingStoreApplicationData)
{
    if (!plingStoreApplicationData->previewPics.isEmpty())
        plingStoreApplicationData->previewPics.first()->smallPic;

    auto worker = new InstallWorker(_applicationsDir, _partialsDir, this);
    connect(worker, &InstallWorker::progressNotification, this, &InstallService::progressNotification);

    ApplicationData appData;
    appData.setName(plingStoreApplicationData->name);
    worker->installFromUrl(appimageUrl, Application(appData));
}

void InstallService::handleApplicationAdded(const Application &application)
{
    installMainBundle(application);
}
void InstallService::handleApplicationUpdated(const Application &application)
{
    uninstallAllBundles(application);
    installMainBundle(application);
}

void InstallService::handleApplicationRemoved(const Application &application)
{
    uninstallAllBundles(application);
}

void InstallService::uninstallAllBundles(const Application &applicationData) const
{
    for (const auto &bundle : applicationData.getBundles())
        AppImageTools::unintegrate(QUrl::fromLocalFile(bundle.path));
}
void InstallService::installMainBundle(const Application &applicationData) const
{
    const auto &mainBundle = applicationData.getMainBundle();
    if (!mainBundle.path.isEmpty())
        AppImageTools::integrate(QUrl::fromLocalFile(mainBundle.path));
}
