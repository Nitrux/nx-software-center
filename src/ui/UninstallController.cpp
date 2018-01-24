//
// Created by alexis on 23/01/18.
//

#include <entities/Repository.h>
#include <entities/Registry.h>
#include <entities/Executor.h>
#include "UninstallController.h"

#include "interactors/RemoveAppImageInteractor.h"

UninstallController::UninstallController(Repository *repository, Registry *registry, Executor *executor,
                                         QObject *parent)
        : QObject(parent), repository(repository), registry(registry), executor(executor)  {}

void UninstallController::uninstall(const QString &appId) {
    const Application &a = repository->get(appId);
    const QStringList &files = registry->getInstalledApplicationFiles(appId);
    auto *i = new RemoveAppImageInteractor(a, files);
    executor->execute(i);
}
