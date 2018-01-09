//
// Created by alexis on 8/01/18.
//

#include "TasksController.h"

void TasksController::assignTaskToApplication(const QString &applicationId, const QString &taskId) {
    applicationsTasks.insert(applicationId, taskId);
}

QString TasksController::getTaskOnApplication(const QString &applicationId) {
    return applicationsTasks.value(applicationId, QString());
}

void TasksController::handleTaskStarted(const QString &id) {

}

void TasksController::handleTaskComplete(const QString &id) {
    removeTaskApplicationRelation(id);
}

void TasksController::removeTaskApplicationRelation(const QString &id)  {
    for (const QString &key : applicationsTasks.keys()) {
        if (id == applicationsTasks.value(key)) {
            applicationsTasks.remove(key);
            break;
        }
    }
}
