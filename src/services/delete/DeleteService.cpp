#include "DeleteService.h"

// libraries
#include <QFile>

// local
#include <services/ApplicationData.h>

void DeleteService::deleteApplication(const QVariant &variant)
{
    const ApplicationData &application = qvariant_cast<ApplicationData>(variant);
    const auto &bundles = application.getBundles();
    for (const auto &bundle : bundles) {
        auto deleted = QFile::remove(bundle.path);
        if (!deleted)
            emit(deleteError(variant, "Unable to remove file: " + bundle.path));
    }
}
