#ifndef TASKCONTROLLER_H
#define TASKCONTROLLER_H

#include <QString>
#include <QObject>
#include <QtQml>

#include <QQmlEngine>

class Interactor;
class TaskController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int total MEMBER m_total NOTIFY totalChanged)
    Q_PROPERTY(int progress MEMBER m_progress NOTIFY progressChanged)
    Q_PROPERTY(QString message MEMBER m_message NOTIFY messageChanged)

    Q_PROPERTY(TaskController::TaskState state MEMBER m_state NOTIFY stateChanged)
public:
    enum TaskState
    {
        TASK_CREATED,
        TASK_RUNNING,
        TASK_COMPLETED,
        TASK_FAILED
    };
    Q_ENUMS(TaskState)

    explicit TaskController(QObject *parent = nullptr);
    Q_INVOKABLE virtual void run() = 0;

signals:
    void totalChanged(int total);
    void progressChanged(int progress);
    void messageChanged(QString message);
    void stateChanged(TaskState state);

public slots:

protected:
    Interactor * m_interactor;
    TaskState m_state = TASK_CREATED;


    int m_progress;
    int m_total;
    QString m_message;
};

#endif // TASKCONTROLLER_H
