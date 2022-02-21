#include "UpdateWorker.h"

// libraries
#include <QtCore>
#include <appimage/update.h>

// local

UpdateWorker::UpdateWorker(QObject *parent)
    : QObject(parent)
    , _running(false)
{
}

void UpdateWorker::enqueue(const QList<Application> &applications)
{
    for (const auto &app : applications)
        enqueue(app);
}

void UpdateWorker::enqueue(const Application &application)
{
    if (!_updateQueue.contains(application))
        _updateQueue.push_back(application);

    if (!_running)
        QMetaObject::invokeMethod(this, "processNextApp");
}

void UpdateWorker::processNextApp()
{
    if (!_updateQueue.isEmpty()) {
        const auto application = _updateQueue.takeFirst();
        const auto &applicationBundles = application.getBundles();

        if (!applicationBundles.isEmpty()) {
            const auto &bundle = applicationBundles.first();

            TaskData progress;
            ApplicationUpdateData updateData(application);

            notifyCheck(progress, application);
            appimage::update::Updater updater(bundle.path.toStdString());
            updateData.source = QString::fromStdString(updater.updateInformation());
            if (updateData.source.isEmpty())
                notifyMissingUpdateInformation(progress);
            else
                runUpdate(updater, progress, updateData);
        } else {
            qWarning() << "UpdateWorker::processNextApp"
                       << "trying to update an application without bundles. This is an error, please report it.";
        }

        QMetaObject::invokeMethod(this, &UpdateWorker::processNextApp, Qt::QueuedConnection);
    } else {
        emit(queueCompleted());
    }
}
void UpdateWorker::runUpdate(appimage::update::Updater &updater, TaskData &progress, ApplicationUpdateData updateData)
{
    bool startSucceed = updater.start();
    const auto &applicationData = updateData.application.getData();
    progress.title = "Downloading " + applicationData.getName() + " updates";

    if (!startSucceed) {
        notifyUpdateError(progress);
        return;
    }

    watchUpdaterProgress(progress, &updater);

    if (updater.hasError()) {
        notifyUpdateError(progress);
        return;
    }

    // set update information
    updateData.updateAvailable = false;
    updateData.updateDate = QDateTime::currentDateTime();
    emit(updateCompleted(updateData));

    notifyUpdateSucceed(progress);
}
void UpdateWorker::watchUpdaterProgress(TaskData &progress, appimage::update::Updater *updater)
{
    while (!updater->isDone()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        double current_progress;
        if (!updater->progress(current_progress)) {
            notifyProgressError(progress);
            break;
        }

        notifyUpdateProgress(progress, current_progress);
    }
}
void UpdateWorker::notifyUpdateProgress(TaskData &progress, double current_progress)
{
    progress.current_progress = (long)std::round(current_progress * 100);
    emit(progressNotification(progress));
}
void UpdateWorker::notifyProgressError(TaskData &progress)
{
    progress.status = TaskData::FAILED;
    progress.subTitle = "Something went wrong while trying to update.";

    progress.current_progress = -1;
    progress.total_progress = -1;

    emit(progressNotification(progress));
}
void UpdateWorker::notifyMissingUpdateInformation(TaskData &progress)
{
    qDebug() << "UpdatesWorker::processNextApp"
             << "missing update information";

    progress.status = TaskData::FAILED;
    progress.subTitle = "Update information not available";

    emit(progressNotification(progress));
}
void UpdateWorker::notifyCheck(TaskData &_progress, const Application &application)
{
    _progress.related_app_id = application.getId();
    _progress.current_progress = -1;
    _progress.status = TaskData::RUNNING;
    const auto &applicationData = application.getData();
    _progress.subTitle = "Checking " + applicationData.getName() + " updates";

    emit(progressNotification(_progress));
}
void UpdateWorker::notifyUpdateError(TaskData &progress)
{
    qDebug() << "UpdatesWorker::processNextApp"
             << "update error";

    progress.status = TaskData::FAILED;
    progress.subTitle = "Something went wrong while trying to update.";
    progress.current_progress = -1;
    progress.total_progress = -1;

    emit(progressNotification(progress));
}
void UpdateWorker::notifyUpdateSucceed(TaskData &progress)
{
    qDebug() << "UpdatesWorker update completed: " << progress.related_app_id;

    progress.status = TaskData::SUCCEED;
    progress.subTitle = "Update downloaded successfully";

    progress.current_progress = -1;
    progress.total_progress = -1;

    emit(progressNotification(progress));
}
