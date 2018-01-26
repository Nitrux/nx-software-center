#ifndef REGISTRYCONTROLLER_H
#define REGISTRYCONTROLLER_H

#include <QObject>
#include "entities/Registry.h"
#include "RegistryListModel.h"

class RegistryController : public QObject
{
    Q_OBJECT
    Registry *registry;
    RegistryListModel *model;
    Q_PROPERTY(RegistryListModel *model MEMBER model NOTIFY modelChanged)
    Q_PROPERTY(QStringList installedApplications READ getInstalledApplications NOTIFY installedApplicationsChanged)
public:
    explicit RegistryController(Registry *registry, QObject *parent = nullptr);

    QStringList getInstalledApplications();

signals:
    void installedApplicationsChanged(const QStringList &installedApplications);
    void modelChanged(RegistryListModel *model);
public slots:
    void clearRecords();

protected slots:
    void handleRecordsChanged(const QList<QVariantMap> &records);
};

#endif // REGISTRYCONTROLLER_H
