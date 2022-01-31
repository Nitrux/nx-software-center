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

void InstallService::installFromPlingStore(const QUrl &appimageUrl, Application *plingStoreApplicationData)
{
    ApplicationData applicationData;
    applicationData.setName(plingStoreApplicationData->name);
    if (!plingStoreApplicationData->previewPics.isEmpty())
        plingStoreApplicationData->previewPics.first()->smallPic;

    auto worker = new InstallWorker(_applicationsDir, _partialsDir, this);
    connect(worker, &InstallWorker::progressNotification, this, &InstallService::progressNotification);

    worker->installFromUrl(appimageUrl, applicationData);
}

void InstallService::handleApplicationAdded(const ApplicationData &application)
{
    installMainBundle(application);
}
void InstallService::handleApplicationUpdated(const ApplicationData &application)
{
    uninstallAllBundles(application);
    installMainBundle(application);
}

void InstallService::handleApplicationRemoved(const ApplicationData &application)
{
    uninstallAllBundles(application);
}

void InstallService::uninstallAllBundles(const ApplicationData &applicationData) const
{
    for (const auto &bundle : applicationData.getBundles())
        AppImageTools::unintegrate(QUrl::fromLocalFile(bundle.path));
}
void InstallService::installMainBundle(const ApplicationData &applicationData) const
{
    const auto &mainBundle = applicationData.getMainBundle();
    if (!mainBundle.path.isEmpty())
        AppImageTools::integrate(QUrl::fromLocalFile(mainBundle.path));
}
