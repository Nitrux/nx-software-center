#include "DeleteService.h"

// libraries
#include <QFile>

// local
#include <services/Application.h>

DeleteService::DeleteService(int appBundlesLimit, QObject *parent)
    : QObject(parent)
    , _appBundlesLimit(appBundlesLimit)
{
}
void DeleteService::deleteApplication(const QVariant &variant)
{
    const Application &application = qvariant_cast<Application>(variant);
    const auto &bundles = application.getBundles();
    for (const auto &bundle : bundles)
        removeApplicationBundle(variant, bundle);
}
void DeleteService::removeApplicationBundle(const QVariant &variant, const ApplicationBundle &bundle)
{
    auto deleted = QFile::remove(bundle.path);
    if (!deleted)
        emit(deleteError(variant, "Unable to remove file: " + bundle.path));
}
void DeleteService::onApplicationUpdated(const Application &applicationData)
{
    const auto &bundles = applicationData.getBundles();
    if (bundles.length() > _appBundlesLimit)
        for (int i = _appBundlesLimit; i < bundles.length(); i++)
            removeApplicationBundle(QVariant::fromValue(applicationData), bundles[i]);
}
