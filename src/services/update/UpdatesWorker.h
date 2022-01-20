#pragma once
// system

// libraries
#include <QList>
#include <QObject>
#include <QPointer>
#include <services/ProgressNotification.h>

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
    Q_SIGNAL void updateCompleted(ApplicationBundle bundle);
    Q_SIGNAL void queueCompleted();
    Q_SIGNAL void progressNotification(const ProgressNotification &update);

private:
    Q_SLOT void processNextCheck();
    Q_SLOT void processNextUpdate();

    QList<ApplicationData> _checkQueue;
    QList<ApplicationData> _updateQueue;

    void notifyCheckStart(ProgressNotification &builder, const ApplicationData &application);
    void notifyMissingUpdateInformation(ProgressNotification &progress);
    void notifyCheckError(ProgressNotification &progress);
    void notifyCheckResult(ProgressNotification &progress, const UpdateInformation &updateInformation);
    void notifyProgressError(ProgressNotification &progress);
    void notifyUpdateError(ProgressNotification &progress);
    void notifyUpdateSucceed(ProgressNotification &progress);
    void notifyUpdateProgress(ProgressNotification &progress, double current_progress);
    void watchUpdateProgress(ProgressNotification &progress, appimage::update::Updater *updater);
    void checkApplicationUpdates(const ApplicationData &application);
};
