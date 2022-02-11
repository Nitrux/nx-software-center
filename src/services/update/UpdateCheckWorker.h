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

class UpdateCheckWorker : public QObject
{
    Q_OBJECT
public:
    explicit UpdateCheckWorker(QObject *parent = nullptr);

    Q_SLOT void checkUpdates(const ApplicationsList &applications);
    Q_SIGNAL void checkCompleted();

    Q_SIGNAL void updateFound(ApplicationUpdateData updateInformation);
    Q_SIGNAL void progressNotification(const TaskData &update);

private:
    bool busy;
    TaskData _progress;
    int _missing_update_info_count;
    int _update_check_error_count;
    int _update_available_count;

    void checkApplicationUpdates(const Application &application);
    void notifyStart(const ApplicationsList &applications);
    void notifyCompletion();
    void resetCounters();
};
