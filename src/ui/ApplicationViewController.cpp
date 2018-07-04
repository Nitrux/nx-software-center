#include "ApplicationViewController.h"

#include "interactors/TaskMetadata.h"
#include "LocalizationUtils.h"

#include <QDebug>
ApplicationViewController::ApplicationViewController(QObject* parent)
        :
        QObject(parent), registry(nullptr), executor(nullptr), restClient(nullptr),
        request(nullptr), hasPendingTasks(false)
{

}


void ApplicationViewController::setExecutor(Executor* executor)
{
    ApplicationViewController::executor = executor;
    connect(executor, &Executor::taskStarted, this, &ApplicationViewController::handleTaskStarted);
    connect(executor, &Executor::taskCompleted, this, &ApplicationViewController::handleTaskCompleted);
}

QString ApplicationViewController::getApplicationId()
{
    return application.id;
}

QString ApplicationViewController::getBackgroundImage()
{
    const auto screenshots = application.screenshots;
    if (screenshots.length()>0) {
        const auto first = screenshots.first();
        return first.url;
    }

    return QString();
}

bool ApplicationViewController::isInstalled()
{
    if (registry && !application.id.isEmpty())
        return registry->getInstalledApplications().contains(application.id);

    return false;
}

bool ApplicationViewController::hasScreenShots()
{
    return application.screenshots.length()>0;
}

QString ApplicationViewController::getApplicationIcon()
{
    return application.icon;
}

QString ApplicationViewController::getApplicationName()
{
    return LocalizationUtils::getLocalizedValue(application.name).toString();
}

QString ApplicationViewController::getApplicationAuthor()
{
    const auto developer = application.developer;
    if (!developer.name.isEmpty())
        return developer.name;

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
    const auto releases = application.releases;
    ApplicationFull::Release latestRelease;
    for (const auto& release: releases) {
        if (latestRelease.date.isValid())
            latestRelease = release;

        if (release.date>latestRelease.date)
            latestRelease = release;
    }

    auto result = ApplicationFull::Release::toVariant(latestRelease);
    return result.toMap();
}

QString ApplicationViewController::getApplicationDownloadSize()
{
    auto latestRelease = getLatestRelease();
    auto files = latestRelease["files"].toList();
    QVariantMap downloadFile;
    for (const auto& v: files) {
        const auto file = v.toMap();
        if (file["architecture"].toString().replace("-", "_")==QSysInfo::currentCpuArchitecture())
            downloadFile = file;
    }
    auto size = downloadFile.value("size", 0).toInt();
    if (size>0)
        return formatMemoryValue(size);

    return QString();
}

QString ApplicationViewController::getApplicationDescription()
{
    auto description = LocalizationUtils::getLocalizedValue(application.description).toString();
    if (!description.isEmpty())
        return description;

    auto abstract = LocalizationUtils::getLocalizedValue(application.abstract).toString();
    if (!abstract.isEmpty())
        return abstract;

    return QString();
}

QStringList ApplicationViewController::getApplicationScreenShots()
{
    const auto screenshots = application.screenshots;
    QStringList res;
    for (const auto screenshot: screenshots) {
        if (!screenshot.url.isEmpty())
            res << screenshot.url;
    }

    return res;
}

QString ApplicationViewController::getApplicationWebsite()
{
    const auto links = application.links;
    return links.value("homepage");
}

void ApplicationViewController::loadApplication(const QString& id)
{
    if (restClient) {
        request = restClient->buildGetApplicationRequest(id);
        connect(request, &GetApplicationRequest::resultReady, this,
                &ApplicationViewController::handleGetApplicationResult);
        request->start();
    }
}

void ApplicationViewController::checkIfHasPendingTasks()
{
    hasPendingTasks = false;

    auto runningTasks = executor->getRunningTasks();
    for (const QString taskId: runningTasks) {
        const auto data = executor->getTaskData(taskId);
        const QString appId = data.value(TaskMetadata::KEY_APP_ID).toString();

        if (appId==application.id)
            hasPendingTasks = true;
    }

    emit hasPendingTasksChanged(hasPendingTasks);
}

void ApplicationViewController::handleTaskStarted(const QString&, const QVariantMap& data)
{
    const QString appId = data.value(TaskMetadata::KEY_APP_ID).toString();
    if (appId==application.id) {
        hasPendingTasks = true;
        emit hasPendingTasksChanged(hasPendingTasks);
    }
}

void ApplicationViewController::handleTaskCompleted(const QString&, const QVariantMap& data)
{
    const QString appId = data.value(TaskMetadata::KEY_APP_ID).toString();
    if (appId==application.id) {
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
    ApplicationViewController::restClient = explorer;
}
void ApplicationViewController::handleGetApplicationResult()
{
    disconnect(request, &GetApplicationRequest::resultReady, this,
            &ApplicationViewController::handleGetApplicationResult);
    ApplicationViewController::application = request->getResult();

    checkIfHasPendingTasks();
    request->deleteLater();
    request = nullptr;
    emit applicationChanged();
}
void ApplicationViewController::setRegistry(Registry* registry)
{
    ApplicationViewController::registry = registry;
}
