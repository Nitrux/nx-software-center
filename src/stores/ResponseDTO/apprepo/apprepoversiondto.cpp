#include "apprepoversiondto.h"

AppRepoVersionDTO::AppRepoVersionDTO() { }

void AppRepoVersionDTO::setName(QString name) {
    this->_name = name;
}

QString AppRepoVersionDTO::getName() {
    return this->_name;
}

void AppRepoVersionDTO::setDescription(QString description) {
    this->_description = description;
}

QString AppRepoVersionDTO::getDescription() {
    return this->_description;
}

void AppRepoVersionDTO::setHash(QString hash) {
    this->_hash = hash;
}

QString AppRepoVersionDTO::getHash() {
    return this->_hash;
}

void AppRepoVersionDTO::setFile(QUrl file) {
    this->_file = file;
}

QUrl AppRepoVersionDTO::getFile() {
    return this->_file;
}

void AppRepoVersionDTO::setIpfsCid(QString ipfsCid) {
    this->_ipfsCid = ipfsCid;
}

QString AppRepoVersionDTO::getIpfsCid() {
    return this->_ipfsCid;
}

void AppRepoVersionDTO::setIpfsGateway(QString ipfsGateway) {
    this->_ipfsGateway = ipfsGateway;
}

QString AppRepoVersionDTO::getIpfsGateway() {
    return this->_ipfsGateway;
}