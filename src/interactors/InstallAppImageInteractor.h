//
// Created by alexis on 10/01/18.
//

#ifndef NOMAD_SOFTWARE_CENTER_INSTALLAPPIMAGEINTERACTOR_H
#define NOMAD_SOFTWARE_CENTER_INSTALLAPPIMAGEINTERACTOR_H

#include "entities/Application.h"
#include "interactors/Interactor.h"
#include "gateways/DownloadManager.h"

class InstallAppImageInteractor : public  Interactor {
    Q_OBJECT
    Application app;
    DownloadManager *downloadManager;
    DownloadToFileJob *downloadJob;
    QString installationPath;
    void createInstallationDirIfNotExist();

    void createInstallationPath(QString appFileName);

public:
    InstallAppImageInteractor(const Application &application, DownloadManager *downloadManager, QObject *parent = nullptr);

    void execute() override;

    void setRunningMetadata();

    void setCompletedMetadata();

protected slots:
    void handleDownloadJobProgress(const int value, const int total, const QString &message);
    void handleDownloadJobFinished();
    void handleDownloadJobError(const QString &error);
    void handleCanceled();
};


#endif //NOMAD_SOFTWARE_CENTER_INSTALLAPPIMAGEINTERACTOR_H
