#pragma once
// system

// libraries
#include <QList>
#include <QObject>
#include <QPointer>
#include <services/TaskData.h>

// local
#include "ApplicationUpdateData.h"
#include "services/Application.h"

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
    Q_SLOT void enqueue(const QList<Application> &applications);
    Q_SLOT void enqueue(const Application &application);

    Q_SIGNAL void updateCompleted(ApplicationUpdateData updateData);
    Q_SIGNAL void progressNotification(const TaskData &update);
    Q_SIGNAL void queueCompleted();

private:
    bool _running;
    QList<Application> _updateQueue;
    Q_SLOT void processNextApp();
    void runUpdate(appimage::update::Updater &updater, TaskData &progress, ApplicationUpdateData updateData);
    void watchUpdaterProgress(TaskData &progress, appimage::update::Updater *updater);

    void notifyCheck(TaskData &_progress, const Application &application);
    void notifyMissingUpdateInformation(TaskData &progress);
    void notifyProgressError(TaskData &progress);
    void notifyUpdateError(TaskData &progress);
    void notifyUpdateSucceed(TaskData &progress);
    void notifyUpdateProgress(TaskData &progress, double current_progress);
};
