#ifndef TASKDOWNLOADAPPIMAGECONTROLLER_H
#define TASKDOWNLOADAPPIMAGECONTROLLER_H

#include <QObject>

#include "taskcontroller.h"
#include "../interactors/downloadappimagereleaseinteractorlistener.h"

class Registry;
class Interactor;
class Repository;
class DownloadManager;

class TaskDownloadAppImageController : public TaskController, DownloadAppImageReleaseInteractorListener
{
    Q_OBJECT
public:
    explicit TaskDownloadAppImageController(
            QString appImageId, QString appImageReleaseId,
            QList<Repository *> repositories,
            DownloadManager *downloadManager,
            Registry *registry,
            QObject *parent = nullptr);

    Q_INVOKABLE virtual void run() override;
signals:

public slots:

protected:
    virtual void downloadComplete(const QString filePaht);
    virtual void progress(const int progress, const int total, const QString statusMessage);
    virtual void error(const QString &errorMessage);

private:
    Interactor *m_interactor;
};

#endif // TASKDOWNLOADAPPIMAGECONTROLLER_H
