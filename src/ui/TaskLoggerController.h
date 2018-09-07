#ifndef REGISTRYCONTROLLER_H
#define REGISTRYCONTROLLER_H

#include <QObject>
#include "entities/TaskLogger.h"
#include "RegistryListModel.h"

class TaskLoggerController : public QObject {
Q_OBJECT
    TaskLogger *registry;
    RegistryListModel *model;
    Q_PROPERTY(RegistryListModel *model MEMBER model NOTIFY modelChanged)
public:
    explicit TaskLoggerController(TaskLogger *registry, QObject *parent = nullptr);

signals:

    void modelChanged(RegistryListModel *model);

public slots:

    void clearRecords();

protected slots:

    void handleRecordsChanged(const QList<QVariantMap> &records);
};

#endif // REGISTRYCONTROLLER_H
