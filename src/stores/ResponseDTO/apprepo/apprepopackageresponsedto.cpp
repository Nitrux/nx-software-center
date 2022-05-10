#include "apprepopackageresponsedto.h"

AppRepoPackageResponseDTO::AppRepoPackageResponseDTO() { }

void AppRepoPackageResponseDTO::setName(QString name) {
    this->_name = name;
}

QString AppRepoPackageResponseDTO::getName() {
    return this->_name;
}

void AppRepoPackageResponseDTO::setSlug(QString slug) {
    this->_slug = slug;
}

QString AppRepoPackageResponseDTO::getSlug() {
    return this->_slug;
}

void AppRepoPackageResponseDTO::setVersion(QString version) {
    this->_version = version;
}

QString AppRepoPackageResponseDTO::getVersion() {
    return this->_version;
}

void AppRepoPackageResponseDTO::setDescription(QString description) {
    this->_description = description;
}

QString AppRepoPackageResponseDTO::getDescription() {
    return this->_description;
}

void AppRepoPackageResponseDTO::setHash(QString hash) {
    this->_hash = hash;
}

QString AppRepoPackageResponseDTO::getHash() {
    return this->_hash;
}

void AppRepoPackageResponseDTO::setPackage(QString package) {
    this->_package = package;
}

QString AppRepoPackageResponseDTO::getPackage() {
    return this->_package;
}

void AppRepoPackageResponseDTO::setIcon(QUrl icon) {
    this->_icon = icon;
}

QUrl AppRepoPackageResponseDTO::getIcon() {
    return this->_icon;
}

void AppRepoPackageResponseDTO::setPreview(QUrl preview) {
    this->_preview = preview;
}

QUrl AppRepoPackageResponseDTO::getPreview() {
    return this->_preview;
}

void AppRepoPackageResponseDTO::setFile(QUrl file) {
    this->_file = file;
}

QUrl AppRepoPackageResponseDTO::getFile() {
    return this->_file;
}

void AppRepoPackageResponseDTO::setVersions(QList<AppRepoVersionDTO> versions) {
    this->_versions = versions;
}

QList<AppRepoVersionDTO> AppRepoPackageResponseDTO::getVersions() {
    return this->_versions;
}

void AppRepoPackageResponseDTO::setGroups(QList<AppRepoGroupResponseDTO> groups) {
    this->_groups = groups;
}

QList<AppRepoGroupResponseDTO> AppRepoPackageResponseDTO::getGroups() {
    return this->_groups;
}

void AppRepoPackageResponseDTO::setImages(QStringList images) {
    this->_images = images;
}

QStringList AppRepoPackageResponseDTO::getImages() {
    return this->_images;
}