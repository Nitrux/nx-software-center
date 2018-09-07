//
// Created by alexis on 25/01/18.
//

#ifndef NOMAD_SOFTWARE_CENTER_UPGRADERCONTROLLER_H
#define NOMAD_SOFTWARE_CENTER_UPGRADERCONTROLLER_H

#include <QObject>
#include <entities/Upgrader.h>
#include <entities/Worker.h>

class UpgraderController : public QObject {
Q_OBJECT
    Upgrader *upgrader;
    Worker *worker;

    QVariantList upgrades;
    Q_PROPERTY(QVariantList upgrades MEMBER upgrades NOTIFY upgradesChanged)

    QStringList upgradableApplicationIds;
    Q_PROPERTY(QStringList upgradableApplicationIds MEMBER upgradableApplicationIds NOTIFY upgradableApplicationIdsChanged)

public:
    explicit UpgraderController(QObject *parent = nullptr);

    void setUpgrader(Upgrader *upgrader);

    void setWorker(Worker *worker);

    Q_INVOKABLE void findUpgrades();

    Q_INVOKABLE void upgradeApplication(const QString &appId);

signals:

    void upgradableApplicationIdsChanged(const QStringList &upgradableApplications);

    void upgradesChanged(const QVariantList &upgrades);

protected slots:

    void handleUpgradesLookUpCompleted();

};


#endif //NOMAD_SOFTWARE_CENTER_UPGRADERCONTROLLER_H
