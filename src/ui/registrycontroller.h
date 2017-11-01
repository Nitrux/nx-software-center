#ifndef REGISTRYCONTROLLER_H
#define REGISTRYCONTROLLER_H

#include <QObject>

class RegistryController : public QObject
{
    Q_OBJECT
public:
    explicit RegistryController(QObject *parent = nullptr);

signals:

public slots:
};

#endif // REGISTRYCONTROLLER_H