#ifndef REGISTRYCONTROLLER_H
#define REGISTRYCONTROLLER_H

#include <QObject>

class Registry;
class RegistryController : public QObject
{
    Q_OBJECT
public:
    explicit RegistryController(Registry *registry, QObject *parent = nullptr);

    Q_INVOKABLE bool isReleaseDownloaded(QString appId, QString releaseId);
signals:
    void registryChanged();

public slots:

protected:
    Registry *m_registry;
};

#endif // REGISTRYCONTROLLER_H
