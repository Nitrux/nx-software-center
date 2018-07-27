//
// Created by alexis on 7/27/18.
//

#ifndef NX_SOFTWARE_CENTER_DEPLOYEDAPPLICATIONSCONTROLLER_H
#define NX_SOFTWARE_CENTER_DEPLOYEDAPPLICATIONSCONTROLLER_H

#include <QObject>
#include <QVariantList>

#include <entities/Worker.h>
#include <gateways/DeployedApplicationsRegistry.h>

#include "ApplicationListModel.h"

class DeployedApplicationsController : public QObject {
Q_OBJECT
    Q_PROPERTY(QVariantList applicationList MEMBER applicationList NOTIFY applicationListChanged);
    Q_PROPERTY(bool isBusy MEMBER busy NOTIFY isBusyChanged);

    DeployedApplicationsRegistry *registry;
    QVariantList applicationList;
    Worker *worker;
    bool busy;

public:
    DeployedApplicationsController(QObject* parent = nullptr);
    void setWorker(Worker* worker);

    void setRegistry(DeployedApplicationsRegistry *registry);

signals:
    void isBusyChanged(const bool &isBusy);
    void applicationListChanged(QVariantList QVAriantList);

protected slots:
    void handleWorkerTaskCompleted(const QVariantMap task);
    void updateApplicationsList();

private:
    QString formatMemoryValue(float num);
};


#endif //NX_SOFTWARE_CENTER_DEPLOYEDAPPLICATIONSCONTROLLER_H
