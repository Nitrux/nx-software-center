#include "application.h"

Application::Application() {}

Application::Application(const QString &name, const QString &version) {
  this->name = name;
  this->version = version;

  this->id = generateId(name, version);
}

QString Application::getId() const { return id; }

QString Application::getName() const { return name; }

QString Application::getVersion() const { return version; }

int Application::Compare(const Application &a) const {
  int diff = id.compare(a.id);
  if (diff) return diff;

  return 0;
}

bool Application::operator==(const Application &a) const { return !Compare(a); }

bool Application::operator<(const Application &a) const {
  return Compare(a) < 0;
}

QString Application::generateId(const QString &id,
                                const QString &version) const {
  return QString("%1-%2").arg(id).arg(version);
}
