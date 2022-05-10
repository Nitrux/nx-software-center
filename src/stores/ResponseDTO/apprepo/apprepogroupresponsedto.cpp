#include "apprepogroupresponsedto.h"

AppRepoGroupResponseDTO::AppRepoGroupResponseDTO() { }

void AppRepoGroupResponseDTO::setName(QString name) {
    this->_name = name;
}

QString AppRepoGroupResponseDTO::getName() {
    return this->_name;
}

void AppRepoGroupResponseDTO::setDescription(QString description) {
    this->_description = description;
}

QString AppRepoGroupResponseDTO::getDescription() {
    return this->_description;
}

void AppRepoGroupResponseDTO::setIcon(QUrl icon) {
    this->_icon = icon;
}

QUrl AppRepoGroupResponseDTO::getIcon() {
    return this->_icon;
}

void AppRepoGroupResponseDTO::setUnique(int unique) {
    this->_unique = unique;
}

int AppRepoGroupResponseDTO::getUnique() {
    return this->_unique;
}