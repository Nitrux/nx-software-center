#pragma once
// system

// libraries
#include <QList>
#include <QObject>
#include <QPointer>
#include <services/TaskChangeBuilder.h>

// local
#include "UpdateInformation.h"
#include "services/ApplicationData.h"

namespace appimage::update
{
class Updater;
}

class UpdatesWorker : public QObject
{
    Q_OBJECT
public:
    explicit UpdatesWorker(QObject *parent = nullptr);

    Q_SLOT void queueCheck(const QList<ApplicationData> &applications);
    Q_SLOT void queueUpdate(const QList<ApplicationData> &applications);

    Q_SIGNAL void updateFound(UpdateInformation updateInformation);
    Q_SIGNAL void taskUpdate(QVariantMap update);
    Q_SIGNAL void updateCompleted(ApplicationBundle bundle);
    Q_SIGNAL void queueCompleted();

private:
    Q_SLOT void processNextCheck();
    Q_SLOT void processNextUpdate();

    QList<ApplicationData> _checkQueue;
    QList<ApplicationData> _updateQueue;

    void notifyCheckStart(TaskChangeBuilder &builder, const ApplicationData &application);
    void notifyMissingUpdateInformation(TaskChangeBuilder &builder);
    void notifyCheckError(TaskChangeBuilder &builder);
    void notifyCheckResult(TaskChangeBuilder &builder, const UpdateInformation &updateInformation);
    void notifyProgressError(TaskChangeBuilder &builder);
    void notifyUpdateError(TaskChangeBuilder &builder);
    void notifyUpdateSucceed(TaskChangeBuilder &builder);
    void notifyUpdateProgress(TaskChangeBuilder &builder, double progress);
    void watchUpdateProgress(TaskChangeBuilder &taskChangeBuilder, appimage::update::Updater *updater);
    void checkApplicationUpdates(const ApplicationData &application);
};
