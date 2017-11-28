#include "repository.h"

#include <QException>
#include <QSet>

#include "application.h"

Repository::Repository(QObject* parent) : QObject(parent) {}

void Repository::add(Application app) {
  applications.insert(app.getId(), app);
}

bool Repository::contains(const QString& id) const {
  return applications.contains(id);
}

Application Repository::get(const QString& id) const {
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
  for (const Application& a : applications.values())
    set.insert(a.getName());

  return set.count();
}

QList<Application> Repository::getAll() const {
  return applications.values();
}

QList<Application> Repository::getAllLatestVersions() const {
  QMap<QString, Application> latest;

  for (const Application& a : applications.values()) {
    if (!latest.contains(a.getName()))
      latest.insert(a.getName(), a);
    else {
      const Application& b = latest.value(a.getName());
      if (b < a)
        latest.insert(a.getName(), a);
    }
  }

  return latest.values();
}

QList<Application> Repository::getAllVersions(const QString& id) const {
  QList<Application> apps;

  for (const Application& a : applications.values()) {
    if (id.compare(a.getName()) == 0)
      apps.append(a);
  }

  qSort(apps);
  return apps;
}
