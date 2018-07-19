#ifndef INSTALLCONTROLLER_H
#define INSTALLCONTROLLER_H

#include <QObject>
#include <QList>
#include "entities/Worker.h"
#include "entities/Installer.h"
#include "entities/InstallTask.h"

class InstallController : public QObject {
Q_OBJECT
    Worker *worker;
    Installer* installer;
public:
    InstallController(Installer* installer, QObject* parent = nullptr);

    void setWorker(Worker *worker);

public slots:
    void install(const QString& application_id);
};

#endif // INSTALLCONTROLLER_H
