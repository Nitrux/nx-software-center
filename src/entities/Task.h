//
// Created by alexis on 7/6/18.
//

#ifndef NX_SOFTWARE_CENTER_TASK_H
#define NX_SOFTWARE_CENTER_TASK_H

#include <QMutex>
#include <QObject>
#include <QString>
#include "ApplicationAbstract.h"

class Task : public QObject {
Q_OBJECT
    QMutex mutex;

    QString id;
    QString status;
    QString type;
    qint64 progressValue;
    qint64 progressTotal;
    QString progressMessage;
    ApplicationAbstract applicationAbstract;

    qint64 creationTime;
    bool deleteOnceCompleted;
public:
    Task(QObject *parent = nullptr);

    static constexpr const char *VALUE_STATUS_CREATED = "created";
    static constexpr const char *VALUE_STATUS_RUNNING = "running";
    static constexpr const char *VALUE_STATUS_COMPLETED = "completed";
    static constexpr const char *VALUE_STATUS_FAILED = "failed";

    const QString &getId();

    const QString &getStatus();

    void setStatus(const QString &status);

    const QString &getType();

    void setType(const QString &type);

    qint64 getProgressValue();

    void setProgressValue(qint64 progressValue);

    qint64 getProgressTotal();

    void setProgressTotal(qint64 progressTotal);

    const QString &getProgressMessage();

    void setProgressMessage(const QString &progressMessage);

    const ApplicationAbstract &getApplicationAbstract();

    void setApplicationAbstract(const ApplicationAbstract &applicationAbstract);

    bool isDeleteOnceCompletedSet();

    void setDeleteOnceCompleted(bool deleteOnceCompleted);

    qint64 getCreationTime();

    virtual void start() = 0;

    virtual void stop() = 0;

    static QVariantMap toVariant(Task *task);

signals:
    void changed();
    void failed();
    void completed();
};

#endif //NX_SOFTWARE_CENTER_TASK_H
