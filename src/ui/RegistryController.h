#ifndef REGISTRYCONTROLLER_H
#define REGISTRYCONTROLLER_H

#include <QObject>
#include "entities/Registry.h"

class RegistryController : public QObject
{
    Q_OBJECT
    Registry *registry;
    Q_PROPERTY(QStringList installedApplications READ getInstalledApplications NOTIFY installedApplicationsChanged)
public:
    explicit RegistryController(Registry *registry, QObject *parent = nullptr);

    QStringList getInstalledApplications();

signals:
    void installedApplicationsChanged(const QStringList &installedApplications);
public slots:

protected slots:
    void handleInstalledApplicationsChanged(const QSet<QString> &installedApplications);
};

#endif // REGISTRYCONTROLLER_H
