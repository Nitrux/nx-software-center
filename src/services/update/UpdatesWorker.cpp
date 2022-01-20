#include "UpdatesWorker.h"

// libraries
#include <QtCore>
#include <appimage/update.h>

// local

UpdatesWorker::UpdatesWorker(QObject *parent)
    : QObject(parent)
{
}

void UpdatesWorker::queueCheck(const QList<ApplicationData> &applications)
{
    _checkQueue.append(applications);
    QMetaObject::invokeMethod(this, &UpdatesWorker::processNextCheck, Qt::QueuedConnection);
}

void UpdatesWorker::queueUpdate(const QList<ApplicationData> &applications)
{
    _updateQueue.append(applications);
    QMetaObject::invokeMethod(this, &UpdatesWorker::processNextUpdate, Qt::QueuedConnection);
}
void UpdatesWorker::processNextCheck()
{
    if (_checkQueue.isEmpty()) {
        emit queueCompleted();
        return;
    }

    const auto application = _checkQueue.takeFirst();
    checkApplicationUpdates(application);

    QMetaObject::invokeMethod(this, &UpdatesWorker::processNextCheck, Qt::QueuedConnection);
}
void UpdatesWorker::checkApplicationUpdates(const ApplicationData &application)
{
    const auto application_bundles = application.getBundles();
    if (!application_bundles.isEmpty()) {
        qDebug() << "Checking updates of" << application.getName();

        ProgressNotification progress;
        notifyCheckStart(progress, application);

        const auto &bundle = application_bundles.first();
        auto updater = QScopedPointer<appimage::update::Updater>(new appimage::update::Updater(bundle.path.toStdString()));

        UpdateInformation result(application);
        result.source = QString::fromStdString(updater->updateInformation());
        if (result.source.isEmpty()) {
            notifyMissingUpdateInformation(progress);
        } else {
            bool checkSucceed = updater->checkForChanges(result.updateAvailable);
            if (checkSucceed) {
                notifyCheckResult(progress, result);
                if (result.updateAvailable)
                    emit(updateFound(result));
            } else {
                notifyCheckError(progress);
            }
        }
    }
}
void UpdatesWorker::processNextUpdate()
{
    if (_updateQueue.isEmpty())
        return;

    const auto application = _updateQueue.takeFirst();
    qDebug() << "UpdatesWorker::processNextUpdate " << application.getId();

    const auto applicationBundles = application.getBundles();
    if (!applicationBundles.isEmpty()) {
        const auto &bundle = applicationBundles.first();
        qDebug() << "UpdatesWorker::processNextUpdate " << bundle.path;

        ProgressNotification progress;
        UpdateInformation result(application);

        notifyCheckStart(progress, application);

        appimage::update::Updater updater(bundle.path.toStdString());
        result.source = QString::fromStdString(updater.updateInformation());
        if (!result.source.isEmpty()) {
            bool startSucceed = updater.start();
            progress.title = "Downloading " + application.getName() + " updates";

            if (startSucceed) {
                watchUpdateProgress(progress, &updater);

                std::string newBundlePath;
                updater.pathToNewFile(newBundlePath);

                if (updater.hasError() || newBundlePath.empty()) {
                    qDebug() << "UpdatesWorker::processNextUpdate"
                             << "update error";
                    notifyUpdateError(progress);
                } else {
                    ApplicationBundle newBundle(QString::fromStdString(newBundlePath));
                    newBundle.app->setId(application.getId());

                    qDebug() << "UpdatesWorker::processNextUpdate"
                             << "update completed" << newBundle.path;

                    notifyUpdateSucceed(progress);
                    emit(updateCompleted(newBundle));
                }
            }
        } else {
            qDebug() << "UpdatesWorker::processNextUpdate"
                     << "missing update information";
            notifyMissingUpdateInformation(progress);
        }
    }

    QMetaObject::invokeMethod(this, &UpdatesWorker::processNextUpdate, Qt::QueuedConnection);
}
void UpdatesWorker::watchUpdateProgress(ProgressNotification &progress, appimage::update::Updater *updater)
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
void UpdatesWorker::notifyUpdateProgress(ProgressNotification &progress, double current_progress)
{
    progress.current_progress = (long)std::round(current_progress * 100);
    emit(progressNotification(progress));
}
void UpdatesWorker::notifyProgressError(ProgressNotification &progress)
{
    progress.status = ProgressNotification::FAILED;
    progress.subTitle = "Something went wrong while trying to update.";

    emit(progressNotification(progress));
}
void UpdatesWorker::notifyCheckResult(ProgressNotification &progress, const UpdateInformation &updateInformation)
{
    auto resultText = updateInformation.updateAvailable ? "New update available" : "Update not available";
    progress.status = ProgressNotification::SUCCEED;
    progress.subTitle = resultText;

    emit(progressNotification(progress));
}
void UpdatesWorker::notifyCheckError(ProgressNotification &progress)
{
    progress.status = ProgressNotification::FAILED;
    progress.subTitle = "Something went wrong while trying to resolve the update information.";

    emit(progressNotification(progress));
}
void UpdatesWorker::notifyMissingUpdateInformation(ProgressNotification &progress)
{
    progress.status = ProgressNotification::FAILED;
    progress.subTitle = "Update information not available";

    emit(progressNotification(progress));
}
void UpdatesWorker::notifyCheckStart(ProgressNotification &builder, const ApplicationData &application)
{
    builder.related_app_id = application.getId();
    builder.total_progress = 100;
    builder.status = ProgressNotification::RUNNING;
    builder.title = "Checking " + application.getName() + " updates";

    emit(progressNotification(builder));
}
void UpdatesWorker::notifyUpdateError(ProgressNotification &progress)
{
    progress.status = ProgressNotification::FAILED;
    progress.subTitle = "Something went wrong while trying to update.";

    emit(progressNotification(progress));
}
void UpdatesWorker::notifyUpdateSucceed(ProgressNotification &progress)
{
    progress.status = ProgressNotification::SUCCEED;
    progress.subTitle = "Update downloaded successfully";

    emit(progressNotification(progress));
}
