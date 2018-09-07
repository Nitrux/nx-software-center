#ifndef INSTALLCONTROLLER_H
#define INSTALLCONTROLLER_H

#include <QObject>
#include <QList>
#include "entities/Worker.h"
#include "entities/Deployer.h"
#include "tasks/DeployTask.h"

class DeployController : public QObject {
Q_OBJECT
    Worker *worker;
    Deployer* installer;
public:
    DeployController(Deployer* installer, QObject* parent = nullptr);

    void setWorker(Worker *worker);

public slots:
    void deploy(const QString &application_id);
};

#endif // INSTALLCONTROLLER_H
