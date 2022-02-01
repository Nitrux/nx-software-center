#include "UpdateCheckWorker.h"

// libraries
#include <QtCore>
#include <appimage/update.h>

// local

UpdateCheckWorker::UpdateCheckWorker(QObject *parent)
    : QObject(parent)
    , busy(false)
    , _missing_update_info_count(0)
    , _update_check_error_count(0)
    , _update_available_count(0)
{
}

void UpdateCheckWorker::checkUpdates(const ApplicationsList &applications)
{
    if (!busy) {
        busy = true;
        resetCounters();

        notifyStart(applications);

        for (const auto &application : applications) {
            _progress.current_progress++;
            const auto &applicationData = application.getData();
            _progress.subTitle = applicationData.getName();
            emit(progressNotification(_progress));

            checkApplicationUpdates(application);
        }

        notifyCompletion();
        busy = false;
    }
}
void UpdateCheckWorker::resetCounters()
{
    _missing_update_info_count = 0;
    _update_check_error_count = 0;
    _update_available_count = 0;
}
void UpdateCheckWorker::notifyStart(const ApplicationsList &applications)
{
    _progress.title = "Looking for applications updates";
    _progress.iconPath = "qrc://cancel";
    _progress.total_progress = applications.length();
    _progress.current_progress = 0;

    _progress.status = TaskData::RUNNING;
    emit(progressNotification(_progress));
}
void UpdateCheckWorker::notifyCompletion()
{
    _progress.status = TaskData::SUCCEED;
    _progress.title = "Updates lockup completed";
    _progress.subTitle = QString("%1 updates available").arg(_update_available_count);

    if (_update_check_error_count) {
        _progress.subTitle += QString(", %1 lockups failed").arg(_update_available_count);
    }

    emit(checkCompleted());
    emit(progressNotification(_progress));
}

void UpdateCheckWorker::checkApplicationUpdates(const Application &application)
{
    const auto application_bundles = application.getBundles();
    if (!application_bundles.isEmpty()) {
        const auto &applicationData = application.getData();
        qDebug() << "Checking updates of" << applicationData.getName();

        const auto &bundle = application_bundles.first();
        auto updater = QScopedPointer<appimage::update::Updater>(new appimage::update::Updater(bundle.path.toStdString()));

        ApplicationUpdateData result(application);
        result.source = QString::fromStdString(updater->updateInformation());
        if (result.source.isEmpty()) {
            _missing_update_info_count++;
        } else {
            bool checkSucceed = updater->checkForChanges(result.updateAvailable);
            if (checkSucceed) {
                if (result.updateAvailable) {
                    _update_available_count++;
                    emit(updateFound(result));
                }
            } else {
                _update_check_error_count++;
            }
        }
    }
}
