#ifndef UPDATERCONTROLLER_H
#define UPDATERCONTROLLER_H

#include <QObject>

#include "entities/Updater.h"

class UpdaterController : public QObject
{
    Q_OBJECT
    Updater *updater;

    Q_PROPERTY(bool isWorking READ isWorking NOTIFY isWorkingChanged)
    Q_PROPERTY(bool isReady READ isReady NOTIFY isReadyChanged)
public:
    explicit UpdaterController(Updater *updater);

    bool isWorking();
    bool isReady();

signals:
    void isWorkingChanged(bool isWorking);

    void isReadyChanged(bool isWorking);

public slots:
    void update();
};

#endif // UPDATERCONTROLLER_H
