//
// Created by alexis on 8/02/18.
//

#ifndef NOMAD_SOFTWARE_CENTER_NOTIFICATIONSCONTROLLER_H
#define NOMAD_SOFTWARE_CENTER_NOTIFICATIONSCONTROLLER_H

#include <QObject>
#include <QTimer>

#include <entities/Executor.h>

class NotificationsController : public QObject {
Q_OBJECT
    Executor *executor;
    QTimer expirationTimer;
    void resetExpirationTimer();

    bool tasksNotificationsEnabled;
    Q_PROPERTY(bool tasksNotificationsEnabled MEMBER tasksNotificationsEnabled NOTIFY tasksNotificationsEnabledChanged)
public:
    enum {
        MESSAGE = 0,
        ALERT,
        FAILURE
    } NOTIFICATION_TYPE;

    explicit NotificationsController(QObject *parent = nullptr);

    void setExecutor(Executor *executor);
signals:

    void showNotificationRequest(const int notficationType, const QString &message);
    void notificationExpired();
    void tasksNotificationsEnabledChanged(bool tasksNotificationsEnabled);

public slots:

    void hideNotification();

protected slots:

    void handleTaskStarted(const QString &id, const QVariantMap &data);

    void handleTaskCompleted(const QString &id, const QVariantMap &data);

    void notifyTaskDescription(const QVariantMap &data);
};


#endif //NOMAD_SOFTWARE_CENTER_NOTIFICATIONSCONTROLLER_H
