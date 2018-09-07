//
// Created by alexis on 8/02/18.
//

#ifndef NOMAD_SOFTWARE_CENTER_NOTIFICATIONSCONTROLLER_H
#define NOMAD_SOFTWARE_CENTER_NOTIFICATIONSCONTROLLER_H

#include <QObject>
#include <QTimer>

#include <entities/Worker.h>

class NotificationsController : public QObject {
Q_OBJECT
    Worker *worker;
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

    void setWorker(Worker *worker);

signals:
    void showNotificationRequest(const int notficationType, const QString &message);
    void notificationExpired();
    void tasksNotificationsEnabledChanged(bool tasksNotificationsEnabled);

public slots:

    void hideNotification();

protected slots:

    void handleTaskStarted(const QVariantMap &data);

    void handleTaskCompleted(const QVariantMap &data);

    void notifyTaskDescription(const QVariantMap &data);

    bool shouldBeNotified(const QVariantMap &data) const;
};


#endif //NOMAD_SOFTWARE_CENTER_NOTIFICATIONSCONTROLLER_H
