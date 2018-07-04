#include "ApplicationViewController.h"

#include "interactors/TaskMetadata.h"
#include "LocalizationUtils.h"

#include <QDebug>
ApplicationViewController::ApplicationViewController(QObject* parent)
        :
        QObject(parent), repository(nullptr), registry(nullptr), executor(nullptr),
        hasPendingTasks(false)
{

}

void ApplicationViewController::setRepository(Repository* repository)
{
    ApplicationViewController::repository = repository;
}

void ApplicationViewController::setRegistry(Registry* registry)
{
//    ApplicationViewController::registry = registry;
}

void ApplicationViewController::setExecutor(Executor* executor)
{
    ApplicationViewController::executor = executor;
    connect(executor, &Executor::taskStarted, this, &ApplicationViewController::handleTaskStarted);
    connect(executor, &Executor::taskCompleted, this, &ApplicationViewController::handleTaskCompleted);
}

QString ApplicationViewController::getApplicationId()
{
    return application.value("id").toString();
}

QString ApplicationViewController::getBackgroundImage()
{
    const auto screenshots = application.value("screenshots").toList();
    if (screenshots.length()>0) {
        const auto first = screenshots.first().toMap();
        return first["url"].toString();
    }

    return QString();
}

bool ApplicationViewController::isInstalled()
{
    if (registry && !application.isEmpty())
        return registry->getInstalledApplications().contains(application["id"].toString());

    return false;
}

bool ApplicationViewController::hasScreenShots()
{
    const auto screenshots = application.value("screenshots").toList();
    return screenshots.length()>0;
}

QString ApplicationViewController::getApplicationIcon()
{
    return application["icon"].toString();
}

QString ApplicationViewController::getApplicationName()
{
    return LocalizationUtils::getLocalizedValue(application["name"].toMap()).toString();
}

QString ApplicationViewController::getApplicationAuthor()
{
    const auto developer = application["developer"].toMap();
    if (developer.contains("name"))
        return developer["name"].toString();

    return QString();
}

QString ApplicationViewController::getApplicationVersion()
{
    QVariantMap latestRelease = getLatestRelease();

    QString version = latestRelease["version"].toString();
    if (!version.isEmpty())
        return version;

    QDateTime date = latestRelease["date"].toDateTime();
    if (date.isValid())
        return date.toString();

    return "latest";
}
QVariantMap ApplicationViewController::getLatestRelease() const
{
    const auto releases = application["releases"].toList();
    QVariantMap latestRelease;
    for (const auto& v: releases) {
        auto release = v.toMap();

        if (latestRelease.isEmpty())
            latestRelease = release;

        if (release["date"].toDateTime() > latestRelease["date"].toDateTime())
            latestRelease = release;
    }
    return latestRelease;
}

QString ApplicationViewController::getApplicationDownloadSize()
{
        auto latestRelease = getLatestRelease();
        auto files = latestRelease["files"].toList();
        QVariantMap downloadFile;
        for (const auto& v: files) {
            const auto file = v.toMap();
            if (file["architecture"].toString().replace("-","_") == QSysInfo::currentCpuArchitecture())
                downloadFile = file;
        }
        auto size = downloadFile.value("size", 0).toInt();
        if (size > 0)
            return formatMemoryValue(size);

        return QString();
}

QString ApplicationViewController::getApplicationDescription()
{
    auto description = LocalizationUtils::getLocalizedValue(application["description"].toMap()).toString();
    if (!description.isEmpty())
        return description;

    auto abstract = LocalizationUtils::getLocalizedValue(application["abstract"].toMap()).toString();
    if (!abstract.isEmpty())
        return abstract;

    return "No description provided by the application developer.";
}

QStringList ApplicationViewController::getApplicationScreenShots()
{
    const auto screenshots = application.value("screenshots").toList();
    QStringList res;
    for (const auto v: screenshots) {
        auto map = v.toMap();
        if (map.contains("url"))
            res << map["url"].toString();
    }

    return res;
}

QString ApplicationViewController::getApplicationWebsite()
{
    const auto links = application["links"].toMap();
    return links.value("homepage", QVariant()).toString();
}

void ApplicationViewController::loadApplication(const QString& id)
{
    if (explorer) {
        connect(explorer, &RestClient::getApplicationCompleted, this,
                &ApplicationViewController::handleGetApplicationCompleted);
        explorer->getApplication(id);
    }
}

void ApplicationViewController::checkIfHasPendingTasks()
{
    hasPendingTasks = false;

    auto runningTasks = executor->getRunningTasks();
    for (const QString taskId: runningTasks) {
        const auto data = executor->getTaskData(taskId);
        const QString appId = data.value(TaskMetadata::KEY_APP_ID).toString();

        if (appId==application["id"].toString())
            hasPendingTasks = true;
    }

    emit hasPendingTasksChanged(hasPendingTasks);
}

void ApplicationViewController::handleTaskStarted(const QString&, const QVariantMap& data)
{
    const QString appId = data.value(TaskMetadata::KEY_APP_ID).toString();
    if (appId==application["id"].toString()) {
        hasPendingTasks = true;
        emit hasPendingTasksChanged(hasPendingTasks);
    }
}

void ApplicationViewController::handleTaskCompleted(const QString&, const QVariantMap& data)
{
    const QString appId = data.value(TaskMetadata::KEY_APP_ID).toString();
    if (appId==application["id"].toString()) {
        hasPendingTasks = false;
        emit hasPendingTasksChanged(hasPendingTasks);
        emit applicationChanged();
    }
}

QString ApplicationViewController::formatMemoryValue(float num)
{
    QStringList list;
    list << "KiB" << "MiB" << "GiB" << "TiB";

    QStringListIterator i(list);
    QString unit("Byte");

    while (num>=1024.0 && i.hasNext()) {
        unit = i.next();
        num /= 1024.0;
    }

    return QString().setNum(num, 'f', 2)+" "+unit;
}
void ApplicationViewController::setExplorer(RestClient* explorer)
{
    ApplicationViewController::explorer = explorer;
}
void ApplicationViewController::handleGetApplicationCompleted(const QVariantMap& application)
{
    disconnect(explorer, &RestClient::getApplicationCompleted, this,
            &ApplicationViewController::handleGetApplicationCompleted);
    ApplicationViewController::application = application;

    checkIfHasPendingTasks();
    emit applicationChanged();
}
