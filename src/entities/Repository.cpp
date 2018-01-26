#include "Repository.h"

#include <QList>
#include <QSet>
#include <QReadLocker>
#include <QWriteLocker>

#include "entities/Application.h"
#include "interactors/FetchApplicationsInteractor.h"

Repository::Repository(QObject *parent) : QObject(parent), isBeingUpdated(false) {}

void Repository::add(Application app) {
    {
        QWriteLocker lock(&mutex);
        applications.insert(app.getId(), app);
    }

    if (!isBeingUpdated)
            emit changed();
}

bool Repository::contains(const QString &id) {
    QReadLocker lock(&mutex);

    return applications.contains(id);
}

Application Repository::get(const QString &id) {
    QReadLocker lock(&mutex);

    if (!applications.contains(id))
        throw ApplicationNotFoundException(id);
    else
        return applications.value(id);
}

int Repository::countAll() {
    QReadLocker lock(&mutex);

    return applications.count();
}

int Repository::countByName() {
    QReadLocker lock(&mutex);

    QSet<QString> set;
    for (const Application &a : applications.values())
        set.insert(a.getCodeName());

    return set.count();
}

QList<Application> Repository::getAll() {
    QReadLocker lock(&mutex);

    return applications.values();
}

QList<Application> Repository::getAllLatestVersions() {
    QReadLocker lock(&mutex);

    QMap<QString, Application> latest;

    for (const Application &a : applications.values()) {
        if (!latest.contains(a.getCodeName()))
            latest.insert(a.getCodeName(), a);
        else {
            const Application &b = latest.value(a.getCodeName());
            if (b < a)
                latest.insert(a.getCodeName(), a);
        }
    }

    return latest.values();
}

QList<Application> Repository::getAllVersions(const QString &name) {
    QReadLocker lock(&mutex);

    QList<Application> apps;

    for (const Application &a : applications.values()) {
        if (name.compare(a.getCodeName()) == 0)
            apps.append(a);
    }

    qSort(apps);
    return apps;
}

QList<Application> Repository::filterAll(const QString &query) {
    QReadLocker lock(&mutex);

    QList<Application> results;
    for (const Application &a: applications) {
        if (a.getName().contains(query))
            results.append(a);
    }
    return results;
}

QList<Application> Repository::filterLatestsVersions(const QString &query) {
    QReadLocker lock(&mutex);

    QMap<QString, Application> latest;

    for (const Application &a : applications.values()) {
        if (a.getName().contains(query, Qt::CaseInsensitive) ||
            a.getDescription().contains(query, Qt::CaseInsensitive)) {
            if (!latest.contains(a.getCodeName()))
                latest.insert(a.getCodeName(), a);
            else {
                const Application &b = latest.value(a.getCodeName());
                if (b < a)
                    latest.insert(a.getCodeName(), a);
            }
        }
    }

    return latest.values();
}

Application Repository::getLatestVersion(const QString &name) {
    QReadLocker lock(&mutex);

    Application l;
    for (const Application &a : applications.values()) {
        if (name.compare(a.getCodeName()) == 0 &&
            l.compare_by_version(a) < 0) {
            l = a;
        }
    }

    return l;
}

void Repository::removeAll() {
    {
        QWriteLocker lock(&mutex);
        applications.clear();
    }

    if (!isBeingUpdated)
            emit changed();
}

void Repository::remove(const QString &id) {
    {
        QWriteLocker lock(&mutex);
        applications.remove(id);
    }

    if (!isBeingUpdated)
            emit changed();
}

void Repository::removeAllVersions(const QString &name) {
    {
        QWriteLocker lock(&mutex);

        for (const Application &a : applications.values()) {
            if (name.compare(a.getCodeName()) == 0)
                applications.remove(a.getId());
        }
    }

    if (!isBeingUpdated)
            emit changed();
}

void Repository::setIsBeingUpdated(bool isBeingUpdated) {

    {
        QReadLocker lock(&mutex);
        if (this->isBeingUpdated && !isBeingUpdated)
            emit changed();
    }

    {
        QWriteLocker lock(&mutex);
        this->isBeingUpdated = isBeingUpdated;
    }

    emit isBeingUpdateChanged(this->isBeingUpdated);
}

bool Repository::getIsBeingUpdated() {
    QReadLocker lock(&mutex);

    return isBeingUpdated;
}
