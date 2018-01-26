#include "UpdaterController.h"

#include <QMetaObject>
UpdaterController::UpdaterController(Updater *updater) : QObject(updater), updater(updater)
{
    connect(updater, &Updater::isReadyChanged, this, &UpdaterController::isReadyChanged);
    connect(updater, &Updater::isWorkingChanged, this, &UpdaterController::isWorkingChanged);
}

bool UpdaterController::isWorking() {return updater->getIsWorking();}

bool UpdaterController::isReady() {return updater->getIsReady();}

void UpdaterController::update()  { QMetaObject::invokeMethod(updater, "update");}
