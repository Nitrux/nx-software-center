#include "system.h"

#include "app.h"
#include "change.h"
#include "release.h"
#include "registry.h"
#include "repository.h"

#include <QVariant>
#include <QVariantList>

System::System()
{

}

void System::setRegistry(Registry *registry)
{
    this->registry = registry;
}

void System::addRepository(Repository *repository)
{
    repositories.append(repository);
}

QList<QString> System::listAppIds()
{
    QList<QString> appIds;
    for (Repository * repository: repositories)
    {
        QList<App *> apps = repository->list();
        for (App * app: apps)
        {
            appIds.append(app->id);
        }
    }

    return appIds;
}

QVariantMap System::appDetails(QString appId)
{
    QVariantMap details;
    QVariantList sources;
    details["id"] = appId;
    Release * lastRelease = nullptr;
    QVariantList releaseIds;

    for (Repository * repository: repositories)
    {
        if (repository->contains(appId))
        {
            App * app = repository->getApp(appId);
            sources << repository->id();

            for (Release * release: app->releases())
                releaseIds.append(release->id);

            // Find the very last release
            auto release = app->lastRelease();
            if (lastRelease == nullptr)
                lastRelease = release;
            else
                lastRelease = std::max(release, lastRelease);
        }
    }

    details["releases"] = releaseIds;
    details["sources"] = sources;
    if (lastRelease)
    {
        details["name"] = lastRelease->name;
        details["description"] = lastRelease->description;
        details["icon_link"] = lastRelease->icon_link;
        details["licence"] = lastRelease->licence;

        QVariantList screenshot_links;
        for (QString link: lastRelease->screenshot_links)
            screenshot_links.append(link);

        details["screenshot_links"] = screenshot_links;

        QVariantList authors;
        for (QString author: lastRelease->authors)
            screenshot_links.append(author);

        details["authors"] = authors;
    }
    return details;
}

QList<QString> System::listAppReleaseIds(QString appId)
{
    QStringList releaseIds;

    for (Repository * repository: repositories)
    {
        App * app = repository->getApp(appId);
        if (app != nullptr)
        {
            for (Release *release: app->releases())
                releaseIds << release->id;
        }
    }

    return releaseIds;
}

QVariantMap System::releaseDetails(QString appId, QString releaseId)
{
    QVariantMap details;
    Release * release = findRelease(appId, releaseId);

    if (release)
    {
        details["id"] = release->id;
        details["app_id"] = release->app_id;

        details["name"] = release->name;
        details["description"] = release->description;
        details["icon_link"] = release->icon_link;
        details["licence"] = release->licence;

        QVariantList screenshot_links;
        for (QString link: release->screenshot_links)
            screenshot_links.append(link);

        details["screenshot_links"] = screenshot_links;

        QVariantList authors;
        for (QString author: release->authors)
            screenshot_links.append(author);

        details["authors"] = authors;
    }
    return details;
}

QString System::downloadRelease(QString appId, QString releaseId)
{
    Release *release = findRelease(appId, releaseId);
    if (release)
    {
        Change *change = release->download();
        if (registry != nullptr)
            registry->registerChange(change);

        change->execute();
        return change->id;
    }

    return QString();
}

QString System::installRelease(QString appId, QString releaseId)
{
    Release *release = findRelease(appId, releaseId);
    if (release)
    {
        Change *change = release->install();
        if (registry != nullptr)
            registry->registerChange(change);

        change->execute();
        return change->id;
    }

    return QString();
}

QString System::uninstallRelease(QString appId, QString releaseId)
{
    Release *release = findRelease(appId, releaseId);
    if (release)
    {
        Change *change = release->uninstall();
        if (registry != nullptr)
            registry->registerChange(change);

        change->execute();
        return change->id;
    }

    return QString();
}

QString System::removeRelease(QString appId, QString releaseId)
{
    Release *release = findRelease(appId, releaseId);
    if (release)
    {
        Change *change = release->remove();
        if (registry != nullptr)
            registry->registerChange(change);

        change->execute();
        return change->id;
    }

    return QString();
}

Release *System::findRelease(QString appId, QString releaseId)
{
    Release * release = nullptr;

    for (Repository * repository: repositories)
    {
        if (repository->contains(appId, releaseId))
        {
            release = repository->getRelease(appId, releaseId);
            break;
        }
    }

    return release;
}
