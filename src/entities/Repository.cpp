#include "Repository.h"

#include <QException>
#include <QSet>

#include "entities/Application.h"

Repository::Repository(QObject *parent) : QObject(parent) {}

void Repository::add(Application app) {
    applications.insert(app.getId(), app);
}

bool Repository::contains(const QString &id) const {
    return applications.contains(id);
}

Application Repository::get(const QString &id) const {
    if (!applications.contains(id))
        throw ApplicationNotFoundException();
    else
        return applications.value(id);
}

int Repository::countAll() const {
    return applications.count();
}

int Repository::countByName() const {
    QSet<QString> set;
    for (const Application &a : applications.values())
        set.insert(a.getCodeName());

    return set.count();
}

QList<Application> Repository::getAll() const {
    return applications.values();
}

QList<Application> Repository::getAllLatestVersions() const {
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

QList<Application> Repository::getAllVersions(const QString &name) const {
    QList<Application> apps;

    for (const Application &a : applications.values()) {
        if (name.compare(a.getCodeName()) == 0)
            apps.append(a);
    }

    qSort(apps);
    return apps;
}

QList<Application> Repository::filterAll(const QString &query) const
{
    QList<Application> results;
    for (const Application &a: applications) {
        if (a.getName().contains(query))
            results.append(a);
    }
    return results;
}

QList<Application> Repository::filterLatestsVersions(const QString &query) const
{
    QMap<QString, Application> latest;

    for (const Application &a : applications.values()) {
        if (a.getName().contains( query, Qt::CaseInsensitive) ||
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

Application Repository::getLatestVersion(const QString &name) const {
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
    applications.clear();
}

void Repository::remove(const QString &id) {
    applications.remove(id);
}

void Repository::removeAllVersions(const QString &name) {
    for (const Application &a : applications.values()) {
        if (name.compare(a.getCodeName()) == 0)
            applications.remove(a.getId());
    }
}
