#ifndef TASKSCONTROLLER_H
#define TASKSCONTROLLER_H

#include <QObject>

#include "../interactors/downloadappimagereleaseinteractorlistener.h"
#include "../interactors/removeappimagereleaseinteractorlistener.h"

class Repository;
class Registry;
class Interactor;
class DownloadManager;

class TasksController : public QObject,
        DownloadAppImageReleaseInteractorListener,
        RemoveAppImageReleaseInteractorListener
{
    Q_OBJECT
public:
    explicit TasksController(QList<Repository *> repositories,
                             Registry *registry,
                             DownloadManager *downloadManager,
                             QObject *parent = nullptr);

    virtual void progress(const int progress, const int total, const QString statusMessage);
    virtual void downloadComplete(const QString filePath);
    virtual void finished();
    virtual void error(const QString &errorMessage);

signals:
    void systemBusy();
    void taskComplete();
    void taskProgressUpdate(int current, int total, QString message);
    void taskError(QString message);

public slots:
    void download(QString appId, QString releaseId);
    void remove(QString appId, QString releaseId);

protected:
    bool m_busy = false;
    QList<Repository *> m_repositories;
    Registry * m_registry;
    DownloadManager *m_downladManager;

    Interactor * m_currentTask = nullptr;
};

#endif // TASKSCONTROLLER_H
