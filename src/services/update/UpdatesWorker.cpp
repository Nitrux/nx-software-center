#include "UpdatesWorker.h"

// libraries
#include <QtCore>
#include <appimage/update.h>

// local
#include "services/TaskChangeBuilder.h"
#include "services/TaskManager.h"

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
    if (_checkQueue.isEmpty())
        return;

    const auto application = _checkQueue.takeFirst();
    const auto &bundle = application.getBundles().first();

    TaskChangeBuilder taskUpdate(this);
    UpdateCheckData result(application, false);

    notifyCheckStart(taskUpdate, application);

    auto updater = QScopedPointer<appimage::update::Updater>(new appimage::update::Updater(bundle.path.toStdString()));
    result.source = QString::fromStdString(updater->updateInformation());
    if (result.source.isEmpty()) {
        notifyMissingUpdateInformation(taskUpdate);
        emit(updateInformation(result));
    } else {
        bool checkSucceed = updater->checkForChanges(result.updateAvailable);
        if (checkSucceed) {
            notifyCheckResult(taskUpdate, result);
            emit(updateInformation(result));
        } else {
            notifyCheckError(taskUpdate);
        }
    }

    QMetaObject::invokeMethod(this, &UpdatesWorker::processNextCheck, Qt::QueuedConnection);
}
void UpdatesWorker::processNextUpdate()
{
    if (_updateQueue.isEmpty())
        return;

    const auto application = _updateQueue.takeFirst();
    const auto &bundle = application.getBundles().first();

    TaskChangeBuilder taskChangeBuilder(this);
    UpdateCheckData result(application, false);

    notifyCheckStart(taskChangeBuilder, application);

    appimage::update::Updater updater(bundle.path.toStdString());
    result.source = QString::fromStdString(updater.updateInformation());

    if (result.source.isEmpty()) {
        notifyMissingUpdateInformation(taskChangeBuilder);
        emit(updateInformation(result));
    } else {
        bool startSucceed = updater.start();
        taskChangeBuilder.set(TaskChangeBuilder::TITLE, "Downloading " + application.getName() + " updates");

        if (startSucceed) {
            watchUpdateProgress(taskChangeBuilder, &updater);

            std::string newBundlePath;
            updater.pathToNewFile(newBundlePath);

            if (updater.hasError() || newBundlePath.empty()) {
                notifyUpdateError(taskChangeBuilder);
            } else {
                ApplicationBundle newBundle(QString::fromStdString(newBundlePath));
                newBundle.app->setId(application.getId());

                notifyUpdateSucceed(taskChangeBuilder);
                emit(updateCompleted(newBundle));
            }
        }
    }

    QMetaObject::invokeMethod(this, &UpdatesWorker::processNextUpdate, Qt::QueuedConnection);
}
void UpdatesWorker::watchUpdateProgress(TaskChangeBuilder &taskChangeBuilder, appimage::update::Updater *updater)
{
    while (!updater->isDone()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        double progress;
        if (!updater->progress(progress)) {
            notifyProgressError(taskChangeBuilder);
            break;
        }

        notifyUpdateProgress(taskChangeBuilder, progress);
    }
}
void UpdatesWorker::notifyUpdateProgress(TaskChangeBuilder &builder, double progress)
{
    builder.set(TaskChangeBuilder::CURRENT_PROGRESS, trunc(progress * 100));
    emit(taskUpdate(builder.build()));
}
void UpdatesWorker::notifyProgressError(TaskChangeBuilder &builder)
{
    builder.set(TaskChangeBuilder::STATUS, TaskChangeBuilder::FAILED);
    builder.set(TaskChangeBuilder::SUBTITLE, "Something went wrong while trying to update.");

    emit(taskUpdate(builder.build()));
}
void UpdatesWorker::notifyCheckResult(TaskChangeBuilder &builder, const UpdateCheckData &updateInformation)
{
    auto resultText = updateInformation.updateAvailable ? "New update available" : "Update not available";
    builder.set(TaskChangeBuilder::STATUS, TaskChangeBuilder::SUCCEED);
    builder.set(TaskChangeBuilder::SUBTITLE, resultText);

    emit(builder.build());
}
void UpdatesWorker::notifyCheckError(TaskChangeBuilder &builder)
{
    builder.set(TaskChangeBuilder::STATUS, TaskChangeBuilder::FAILED);
    builder.set(TaskChangeBuilder::SUBTITLE, "Something went wrong while trying to resolve the update information.");

    emit(taskUpdate(builder.build()));
}
void UpdatesWorker::notifyMissingUpdateInformation(TaskChangeBuilder &builder)
{
    builder.set(TaskChangeBuilder::STATUS, TaskChangeBuilder::FAILED);
    builder.set(TaskChangeBuilder::SUBTITLE, "Update information not available");

    emit(builder.build());
}
void UpdatesWorker::notifyCheckStart(TaskChangeBuilder &builder, const ApplicationData &application)
{
    builder.set(TaskChangeBuilder::TARGET_APP_ID, application.getId());
    builder.set(TaskChangeBuilder::TOTAL_PROGRESS, 100);
    builder.set(TaskChangeBuilder::STATUS, TaskChangeBuilder::RUNNING);
    builder.set(TaskChangeBuilder::TITLE, "Checking " + application.getName() + " updates");

    emit(taskUpdate(builder.build()));
}
void UpdatesWorker::notifyUpdateError(TaskChangeBuilder &builder)
{
    builder.set(TaskChangeBuilder::STATUS, TaskChangeBuilder::FAILED);
    builder.set(TaskChangeBuilder::SUBTITLE, "Something went wrong while trying to update.");

    emit(taskUpdate(builder.build()));
}
void UpdatesWorker::notifyUpdateSucceed(TaskChangeBuilder &builder)
{
    builder.set(TaskChangeBuilder::STATUS, TaskChangeBuilder::SUCCEED);
    builder.set(TaskChangeBuilder::SUBTITLE, "Update downloaded successfully");

    emit(taskUpdate(builder.build()));
}
