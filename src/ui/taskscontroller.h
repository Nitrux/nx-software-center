#ifndef TASKSCONTROLLER_H
#define TASKSCONTROLLER_H

#include <QMap>
#include <QString>
#include <QObject>

#include "../interactors/downloadappimagereleaseinteractorlistener.h"
#include "../interactors/removeappimagereleaseinteractorlistener.h"

class Repository;
class Registry;
class TaskController;
class DownloadManager;

class TasksController : public QObject
{
    Q_OBJECT
public:
    explicit TasksController(QList<Repository *> repositories,
                             Registry *registry,
                             DownloadManager *downloadManager,
                             QObject *parent = nullptr);


    Q_INVOKABLE TaskController* getTask(QString taskId);
public slots:
    QString getTaskId(QString appId, QString releaseId);
    QString download(QString appId, QString releaseId);
    QString remove(QString appId, QString releaseId);
    QString execute(QString appId, QString releaseId);

protected:

    bool m_busy = false;
    QString m_currentTaskId;
    QMap<QString, TaskController*> m_tasks;
    QList<Repository *> m_repositories;
    Registry * m_registry;
    DownloadManager *m_downladManager;
};

#endif // TASKSCONTROLLER_H
