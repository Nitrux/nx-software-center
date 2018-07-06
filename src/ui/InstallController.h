#ifndef INSTALLCONTROLLER_H
#define INSTALLCONTROLLER_H

#include <QObject>
#include <QList>
#include "entities/Installer.h"
#include <entities/InstallTask.h>

class InstallController : public QObject {
Q_OBJECT
    Installer* installer;
    QList<InstallTask*> tasks;
public:
    InstallController(Installer* installer, QObject* parent = nullptr);

public slots:
    void install(const QString& application_id);
protected slots:
    void handleInstallTaskCompleted();
    void startTask(InstallTask* task) const;
};

#endif // INSTALLCONTROLLER_H
