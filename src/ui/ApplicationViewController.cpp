#include "ApplicationViewController.h"

#include "interactors/TaskMetadata.h"

#include <QDebug>
ApplicationViewController::ApplicationViewController(QObject *parent) :
    QObject(parent), repository(nullptr), registry(nullptr), executor(nullptr),
    hasPendingTasks(false)
{

}

void ApplicationViewController::setRepository(Repository *repository) {
    ApplicationViewController::repository = repository;
}

void ApplicationViewController::setRegistry(Registry *registry)
{
    ApplicationViewController::registry = registry;
}

void ApplicationViewController::setExecutor(Executor *executor)
{
    ApplicationViewController::executor = executor;
    connect(executor, &Executor::taskStarted, this, &ApplicationViewController::handleTaskStarted);
    connect(executor, &Executor::taskCompleted, this, &ApplicationViewController::handleTaskCompleted);
}

QString ApplicationViewController::getApplicationId()
{
    return application.getId();
}

QString ApplicationViewController::getBackgroundImage()
{
    const QStringList scrennshots = application.getScreenshots();
    for (const QString &candidate: scrennshots)
        if (candidate.section('/', -1) != application.getIcon().section('/', -1))
            return candidate;

    return QString();
}

bool ApplicationViewController::isInstalled()
{
    if (registry && !application.isEmpty())
        return registry->getInstalledApplications().contains(application.getId());

    return false;
}

bool ApplicationViewController::hasScreenShots()
{
    const QStringList scrennshots = application.getScreenshots();
    for (const QString &candidate: scrennshots)
        if (candidate.section('/', -1) != application.getIcon().section('/', -1))
            return true;

    return false;
}

QString ApplicationViewController::getApplicationIcon()
{
    return application.getIcon();
}

QString ApplicationViewController::getApplicationName()
{
    return application.getName();
}

QString ApplicationViewController::getApplicationAuthor()
{
    const auto authors = application.getAuthors();
    if (authors.size() > 0)
        return application.getAuthors().first();

    return QString();
}

QString ApplicationViewController::getApplicationVersion()
{
    return application.getVersion();
}

QString ApplicationViewController::getApplicationDownloadSize()
{
    const auto size = application.getDownloadSize();
    if (size > 0)
        return formatMemoryValue(size);
    else
        return QString();
}

QString ApplicationViewController::getApplicationDescription()
{
    return application.getDescription();
}

QStringList ApplicationViewController::getApplicationScreenShots()
{
    return application.getScreenshots();
}

QString ApplicationViewController::getApplicationWebsite()
{
    return application.getHomePageUrl();
}

void ApplicationViewController::loadApplication(const QString &id)
{
    if (repository) {
        application = repository->get(id);

        checkIfHasPendingTasks();
        emit applicationChanged();
    }
}

void ApplicationViewController::checkIfHasPendingTasks()
{
    hasPendingTasks = false;

    auto runningTasks = executor->getRunningTasks();
    for (const QString taskId: runningTasks) {
        const auto data = executor->getTaskData(taskId);
        const QString appId = data.value(TaskMetadata::KEY_APP_ID).toString();

        if (appId == application.getId())
            hasPendingTasks = true;
    }

    emit hasPendingTasksChanged(hasPendingTasks);
}

void ApplicationViewController::handleTaskStarted(const QString &, const QVariantMap &data)
{
    const QString appId = data.value(TaskMetadata::KEY_APP_ID).toString();
    if (appId == application.getId()) {
        hasPendingTasks = true;
        emit hasPendingTasksChanged(hasPendingTasks);
    }
}

void ApplicationViewController::handleTaskCompleted(const QString &, const QVariantMap &data)
{
    const QString appId = data.value(TaskMetadata::KEY_APP_ID).toString();
    if (appId == application.getId()) {
        hasPendingTasks = false;
        emit hasPendingTasksChanged(hasPendingTasks);
        emit applicationChanged();
    }
}

QString ApplicationViewController::formatMemoryValue(float num)
{
    QStringList list;
    list << "MiB" << "GiB" << "TiB";

    QStringListIterator i(list);
    QString unit("KiB");

    while (num >= 1024.0 && i.hasNext()) {
        unit = i.next();
        num /= 1024.0;
    }

    return QString().setNum(num, 'f', 2) + " " + unit;
}
