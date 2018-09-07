//
// Created by alexis on 23/01/18.
//

#include <entities/TaskLogger.h>
#include "RemoveController.h"

RemoveController::RemoveController(QObject *parent)
        : QObject(parent) {}

void RemoveController::remove(const QString &id)
{
    Q_ASSERT(remover);
    Q_ASSERT(worker);

    auto *t = remover->buildRemoveTaskById(id);
    t->setDeleteOnceCompleted(true);
    worker->execute(t);
}

void RemoveController::removeAppImage(const QString &path)
{
    Q_ASSERT(remover);
    Q_ASSERT(worker);

    auto *t = remover->buildRemoveTaskByPath(path);
    t->setDeleteOnceCompleted(true);
    worker->execute(t);
}

void RemoveController::setRemover(Remover *remover) {
    RemoveController::remover = remover;
}

void RemoveController::setWorker(Worker *worker) {
    RemoveController::worker = worker;
}

