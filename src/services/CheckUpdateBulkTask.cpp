#include "CheckUpdateBulkTask.h"
#include <QFile>

CheckUpdateBulkTask::CheckUpdateBulkTask(const QString &id, AppsModel *appsModel, TaskManager *taskManager)
    : Task(id, "Looking for updates", QString(), "qrc:/download.svg", -1, -1, taskManager)
    , _worker(nullptr)
    , _appsModel(appsModel)
    , _taskManager(taskManager)
{
}

void CheckUpdateBulkTask::start()
{
    setStatus(Task::Status::ACTIVE);
    setTitle("Looking for applications update information");

    _worker = QThread::create([=]() {
        FMH::MODEL_LIST sorterApps = getAppSortedByName();
        auto updatable = getUpdatableApps(sorterApps);
        updateUpdatablePropertyOnModel(updatable);

        setTitle("Update lockup completed");
        if (updatable.empty())
            setSubtitle(QString("No updates were found."));
        else
            setSubtitle(QString("%1 updates are available").arg(updatable.size()));
    });

    connect(_worker, &QThread::finished, this, &CheckUpdateBulkTask::onCheckCompleted);
    _worker->start();
}

void CheckUpdateBulkTask::updateUpdatablePropertyOnModel(const QSet<QString> &updatable)
{
    for (int i = 0; i < _appsModel->getCount(); i++) {
        const auto &app = _appsModel->getItem(i);
        auto path = QUrl(app.value(FMH::PATH)).toLocalFile();

        if (updatable.contains(path))
            _appsModel->setAppUpdatable(i);
    }
}

FMH::MODEL_LIST CheckUpdateBulkTask::getAppSortedByName() const
{
    auto sorterApps = _appsModel->items();
    std::sort(sorterApps.begin(), sorterApps.end(), [](const auto &a, const auto &b) {
        const auto &a_name = a.value(FMH::NAME);
        const auto &b_name = b.value(FMH::NAME);

        return a_name > b_name;
    });
    return sorterApps;
}

QSet<QString> CheckUpdateBulkTask::getUpdatableApps(const QVector<FMH::MODEL> &apps)
{
    setProgressTotal(apps.length());
    int progress = 0;
    QSet<QString> processedUpdateStrings;
    QSet<QString> updatableApps;
    for (auto &app : apps) {
        auto path = QUrl(app.value(FMH::PATH)).toLocalFile();
        auto name = app.value(FMH::NAME);
        setProgress(progress++);
        setSubtitle("Inspecting " + name);

        if (QFile::exists(path)) {
            auto updater = new appimage::update::Updater(path.toStdString(), false);

            QString update_information = QString::fromStdString(updater->updateInformation());
            if (!update_information.isEmpty() && !processedUpdateStrings.contains(update_information)) {
                processedUpdateStrings.insert(update_information);

                bool changesAvailable;
                bool succeed = updater->checkForChanges(changesAvailable);
                if (succeed && changesAvailable)
                    updatableApps.insert(path);
            }

            delete updater;
        }
    }

    return updatableApps;
}

void CheckUpdateBulkTask::onCheckCompleted()
{
    setStatus(Task::Status::COMPLETED);

    auto action = addAction("dismiss", "Dismiss", "user-trash");
    action->setIsActive(true);
    connect(action, &TaskAction::triggered, [=]() {
        _taskManager->destroy(this);
    });
}
