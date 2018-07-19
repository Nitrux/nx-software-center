//
// Created by alexis on 7/4/18.
//

#ifndef NX_SOFTWARE_CENTER_INSTALLER_H
#define NX_SOFTWARE_CENTER_INSTALLER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <gateways/ApplicationRepository.h>

class InstallTask;

class Installer : public QObject {
Q_OBJECT
    ApplicationRepository *repository;
public:
    Installer();

    void setRepository(ApplicationRepository *repository);

    InstallTask *buildInstallLatestReleaseTask(const QString &id);
};

#endif //NX_SOFTWARE_CENTER_INSTALLER_H
