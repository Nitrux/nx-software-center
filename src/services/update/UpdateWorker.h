#pragma once
// system

// libraries
#include <QList>
#include <QObject>
#include <QPointer>
#include <services/TaskData.h>

// local
#include "ApplicationUpdateData.h"
#include "services/ApplicationData.h"

namespace appimage::update
{
class Updater;
}

class UpdateWorker : public QObject
{
    Q_OBJECT

public:
    explicit UpdateWorker(QObject *parent = nullptr);

    // add applications to the work queue
    Q_SLOT void enqueue(const QList<ApplicationData> &applications);
    Q_SLOT void enqueue(const ApplicationData &application);

    Q_SIGNAL void updateCompleted(ApplicationUpdateData updateData);
    Q_SIGNAL void progressNotification(const TaskData &update);
    Q_SIGNAL void queueCompleted();

private:
    bool _running;
    QList<ApplicationData> _updateQueue;
    Q_SLOT void processNextApp();
    void runUpdate(appimage::update::Updater &updater, TaskData &progress, ApplicationUpdateData updateData);
    void watchUpdaterProgress(TaskData &progress, appimage::update::Updater *updater);

    void notifyCheck(TaskData &_progress, const ApplicationData &application);
    void notifyMissingUpdateInformation(TaskData &progress);
    void notifyProgressError(TaskData &progress);
    void notifyUpdateError(TaskData &progress);
    void notifyUpdateSucceed(TaskData &progress);
    void notifyUpdateProgress(TaskData &progress, double current_progress);
};
