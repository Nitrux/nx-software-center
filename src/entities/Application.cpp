#include "Application.h"

Application::Application() {}

Application::Application(const QString &codeName, const QString &version) {
    this->codeName = codeName;
    this->version = version;

    this->id = generateId(codeName, version);
}

QString Application::getId() const { return id; }

QString Application::getCodeName() const { return codeName; }

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

    diff = arch.compare(a.arch);
    if (diff) return diff;

    diff = name.compare(a.name);
    if (diff) return diff;

    diff = description.compare(a.description);
    if (diff) return diff;

    diff = licence.compare(a.licence);
    if (diff) return diff;

    diff = downloadUrl.compare(a.downloadUrl);
    if (diff) return diff;

    diff = homePageUrl.compare(a.homePageUrl);
    if (diff) return diff;

    diff = authors != a.authors;
    if (diff) return diff;

    diff = categories != a.categories;
    if (diff) return diff;

    diff = screenshots != a.screenshots;
    if (diff) return diff;

    return 0;
}

bool Application::operator==(const Application &a) const { return !compare(a); }

bool Application::operator!=(const Application &a) const { return (bool) compare(a); }

bool Application::operator<(const Application &a) const {
    return compare(a) < 0;
}

int Application::compare_by_name(const Application &a) const {
    int diff = codeName.compare(a.codeName);
    return diff;
}

int Application::compare_by_version(const Application &a) const {
    int diff = version.compare(a.version);
    return diff;
}

void Application::setDescription(const QString &description) {
    this->description = description;
}

const QString &Application::getName() const {
    return name;
}

void Application::setName(const QString &name) {
    Application::name = name;
}

const QString &Application::getLicence() const {
    return licence;
}

void Application::setLicence(const QString &licence) {
    Application::licence = licence;
}

const QStringList &Application::getAuthors() const {
    return authors;
}

void Application::setAuthors(const QStringList &authors) {
    Application::authors = authors;
}

const QStringList &Application::getCategories() const {
    return categories;
}

void Application::setCategories(const QStringList &categories) {
    Application::categories = categories;
}

const QString &Application::getArch() const {
    return arch;
}

void Application::setArch(const QString &arch) {
    Application::arch = arch;
}

const QStringList &Application::getScreenshots() const {
    return screenshots;
}

void Application::setScreenshots(const QStringList &screenshots) {
    Application::screenshots = screenshots;
}

const QString &Application::getDownloadUrl() const {
    return downloadUrl;
}

void Application::setDownloadUrl(const QString &downloadUrl) {
    Application::downloadUrl = downloadUrl;
}

const QString &Application::getHomePageUrl() const {
    return homePageUrl;
}

void Application::setHomePageUrl(const QString &webPortalUrl) {
    Application::homePageUrl = webPortalUrl;
}

bool Application::isEmpty() {
    return id.isEmpty();
}
