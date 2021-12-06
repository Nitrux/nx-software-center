#include "UpdateAllTask.h"
#include <QFile>

UpdateAllTask::UpdateAllTask(const QString &id, AppsModel *appsModel, TaskManager *taskManager)
    : Task(id, "Updating apps", QString(), "qrc:/download.svg", -1, -1, taskManager)
    , _worker(nullptr)
    , _appsModel(appsModel)
    , _taskManager(taskManager)
{
}

void UpdateAllTask::start()
{
    setStatus(Task::Status::ACTIVE);
    setTitle("Updating apps");

    _worker = QThread::create([=]() {
        FMH::MODEL_LIST updatableApps = getUpdatableApps();
        int totalAppsUpdated = updateApps(updatableApps);

        setTitle("Update completed");
        if (totalAppsUpdated>0)
            setSubtitle(QString("%1 apps updated").arg(totalAppsUpdated));
        else
            setSubtitle(QString("No apps updated."));

        _appsModel->setIsUpdateAvailable(false);
    });

    connect(_worker, &QThread::finished, this, &UpdateAllTask::onUpdateCompleted);
    _worker->start();
}

FMH::MODEL_LIST UpdateAllTask::getUpdatableApps() const
{
    auto apps = _appsModel->items();
    FMH::MODEL_LIST updatableApps;

    std::copy_if (apps.begin(), apps.end(), std::back_inserter(updatableApps), [](const auto app){
        const auto &app_name        = app.value(FMH::NAME);
        const auto &app_updatable   = app.value(FMH::UPDATABLE);

        return app_updatable=="true";
    });

    return updatableApps;
}

int UpdateAllTask::updateApps(const QVector<FMH::MODEL> &apps)
{
    setProgressTotal(apps.length());
    int progress = 0;
    int totalAppsUpdated = 0;

    for (auto &app : apps) {
        auto path = QUrl(app.value(FMH::PATH)).toLocalFile();
        auto name = app.value(FMH::NAME);
        setProgress(progress++);
        setSubtitle("Updating " + name);

        if (QFile::exists(path)) {
            auto updater = new appimage::update::Updater(path.toStdString(), false);
            updater->start();

            while (!updater->isDone()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            if (updater->hasError()) {
                qDebug() << "Updating " << name << " failed";
                setSubtitle("Updating " + name + " failed");
            } else {
                totalAppsUpdated++;

                string pathToUpdatedFile;
                if (!updater->pathToNewFile(pathToUpdatedFile))
                
                // Integrate AppImage
                if(FMH::fileExists(QUrl::fromLocalFile(QString::fromStdString(pathToUpdatedFile))))
                    AppImageTools::integrate(QUrl::fromLocalFile(QString::fromStdString(pathToUpdatedFile)));
            }

            delete updater;
        }
    }

    return totalAppsUpdated;
}

void UpdateAllTask::onUpdateCompleted()
{
    setStatus(Task::Status::COMPLETED);

    auto action = addAction("dismiss", "Dismiss", "user-trash");
    action->setIsActive(true);
    connect(action, &TaskAction::triggered, [=]() {
        _taskManager->destroy(this);
    });
}
