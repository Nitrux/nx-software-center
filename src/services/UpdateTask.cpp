#include "UpdateTask.h"

UpdateTask::UpdateTask(const QString &id, const QString &appImagePath, const QString &appName, QObject *parent)
    : Task(id, appName, QString(), "qrc:/download.svg", -1, -1, parent)
    , _appName(appName)
    , _worker(nullptr)
{
    qDebug() << appImagePath << "  " << appName;
    _worker = new Updater(appImagePath.toStdString());

    // auto cancelAction = this->addCancelAction("Cancel", "dialog-cancel");
    // connect(cancelAction, &TaskAction::triggered, _worker, [=](){
    //     setStatus(Task::Status::FAILED);
    //     setSubtitle("Download aborted");
    //     setActions({});
    // });
}
void UpdateTask::start()
{
    setProgressTotal(100);
    Task::start();

    std::thread processUpdateThread(&UpdateTask::processUpdate, this);
    processUpdateThread.detach();
}

void UpdateTask::processUpdate() {
    
    if (checkIfUpdateAvailable()) {
        _worker->start();

        while (!_worker->isDone()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            double progress;
            if (!_worker->progress(progress)) {
                qDebug() << "Call to progress() failed";

                setStatus(Task::Status::FAILED);
                setSubtitle("Could not track download progress");
                setActions({});

                return ;
            }

            QString progressMessage = "Downloading Update...";
            setSubtitle(progressMessage);
            setProgress(progress * 100);
        }

        if (_worker->hasError()) {
            setStatus(Task::Status::FAILED);
            setSubtitle("Update failed");
            setActions({});
        } else {
            string pathToUpdatedFile;
            if (!_worker->pathToNewFile(pathToUpdatedFile))
            
            // Integrate AppImage
            if(!FMH::fileExists(QUrl::fromLocalFile(QString::fromStdString(pathToUpdatedFile))))
                return;

            AppImageTools::integrate(QUrl::fromLocalFile(QString::fromStdString(pathToUpdatedFile)));

            setTitle(QString::fromStdString(pathToUpdatedFile.substr(pathToUpdatedFile.find_last_of("\\/") + 1)));
            setStatus(Task::Status::COMPLETED);
            setSubtitle("Update completed");
            setActions({});
        }
    }
}

bool UpdateTask::checkIfUpdateAvailable() {
    if ( _worker->updateInformation().empty() ) {
        setStatus(Task::Status::FAILED);
        setSubtitle("Update information empty");
        setActions({});

        return false;
    }

    bool updateAvailable;
    if (!_worker->checkForChanges(updateAvailable)) {
        setStatus(Task::Status::FAILED);
        setSubtitle("Update not available");
        setActions({});

        return false;
    }

    if ( !updateAvailable ) {
        qDebug() << "Update not available for app";

        setStatus(Task::Status::FAILED);
        setSubtitle("Update not available");
        setActions({});

        return false;
    }

    return true;
}