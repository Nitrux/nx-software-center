#include <QDebug>
#include <QVariant>
#include <interactors/TaskMetadata.h>
#include "NotificationsController.h"

NotificationsController::NotificationsController(QObject *parent) :
    QObject(parent), executor(nullptr), tasksNotificationsEnabled(true)
{
    connect(&expirationTimer, &QTimer::timeout, this, &NotificationsController::notificationExpired);
}

void NotificationsController::setExecutor(Executor *executor) {
    if (NotificationsController::executor != nullptr)
        disconnect(NotificationsController::executor, nullptr, this, nullptr);

    NotificationsController::executor = executor;

    connect(executor, &Executor::taskStarted, this, &NotificationsController::handleTaskStarted);
    connect(executor, &Executor::taskCompleted, this, &NotificationsController::handleTaskCompleted);
}

void NotificationsController::handleTaskStarted(const QString &/*id*/, const QVariantMap &data) {
    if (tasksNotificationsEnabled)
        notifyTaskDescription(data);
}

void NotificationsController::handleTaskCompleted(const QString &/*id*/, const QVariantMap &data) {
    if (tasksNotificationsEnabled)
        notifyTaskDescription(data);
}

void NotificationsController::resetExpirationTimer()
{
    expirationTimer.stop();
    expirationTimer.start(3000);
}

void NotificationsController::notifyTaskDescription(const QVariantMap &data) {
    QString msg = data.value(TaskMetadata::KEY_DESCRIPTION).toString();
    int notificationType = MESSAGE;

    if (data.value(TaskMetadata::KEY_STATUS) == TaskMetadata::VALUE_STATUS_FAILED)
        notificationType = FAILURE;

    emit showNotificationRequest(notificationType, msg);

    resetExpirationTimer();
}


void NotificationsController::hideNotification() {
    expirationTimer.stop();
    emit notificationExpired();
}
