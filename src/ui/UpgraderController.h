//
// Created by alexis on 25/01/18.
//

#ifndef NOMAD_SOFTWARE_CENTER_UPGRADERCONTROLLER_H
#define NOMAD_SOFTWARE_CENTER_UPGRADERCONTROLLER_H

#include <QObject>
#include <entities/Executor.h>
#include <entities/Registry.h>
#include <entities/Repository.h>
#include <gateways/DownloadManager.h>
#include <entities/Upgrader.h>

#include "UpgradesListModel.h"
class InstallAppImageInteractor;
class UpgraderController : public QObject {
Q_OBJECT
    Upgrader *upgrader;
    Repository *repository;
    Registry *registry;
    Executor *executor;
    DownloadManager *downloadManager;

    QStringList upgradableApplications;
    UpgradesListModel *model;
    Q_PROPERTY(QStringList upgradableApplications MEMBER upgradableApplications NOTIFY upgradableApplicationsChanged)
    Q_PROPERTY(UpgradesListModel *model MEMBER model NOTIFY modelChanged)

public:
    UpgraderController(Upgrader *upgrader, Repository *repository, Registry *registry,
                       Executor *executor, DownloadManager *downloadManager, QObject *parent = nullptr);

public slots:
    void upgrade(const QString &appCodeName);
    void upgrade(const QString &oldAppId, const QString &newAppId);

signals:
    void upgradableApplicationsChanged(const QStringList &upgradableApplications);
    void modelChanged(const UpgradesListModel *upgradableApplications);

protected slots:
    void handleUpgradableApplicationsChanged(const UpgradeList &upgradableApplications);

private:
    void updateApplicationsModel(const UpgradeList &upgradableApplications);

    InstallAppImageInteractor *executeInstall(const QString &newAppId) const;

    void executeRemove(const QString &oldAppId) const;

    bool isTaskCompletedSuccesfully(const QVariantMap &metadata) const;
};


#endif //NOMAD_SOFTWARE_CENTER_UPGRADERCONTROLLER_H
