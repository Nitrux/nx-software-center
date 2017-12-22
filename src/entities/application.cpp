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


QString Application::generateId(const QString &id,
                                const QString &version) const {
    return QString("%1-%2").arg(id).arg(version);
}

QString Application::getDescription() const {
    return description;
}

int Application::compare(const Application &a) const {
    int diff = id.compare(a.id);
    if (diff) return diff;

    return 0;
}

bool Application::operator==(const Application &a) const { return !compare(a); }

bool Application::operator!=(const Application &a) const { return (bool) compare(a); }

bool Application::operator<(const Application &a) const {
    return compare(a) < 0;
}

int Application::compare_by_name(const Application &a) const {
    int diff = name.compare(a.name);
    return diff;
}

int Application::compare_by_version(const Application &a) const {
    int diff = version.compare(a.version);
    return diff;
}

void Application::setDescription(const QString &description) {
    this->description = description;
}
