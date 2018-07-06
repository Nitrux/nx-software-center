//
// Created by alexis on 7/6/18.
//
#include <interactors/TaskMetadata.h>
#include <entities/Task.h>
#include "Worker.h"
void Worker::enqueueForExecution(Task* task)
{
    if (task !=nullptr)
        queue << task;

    if (queue.size() == 1)
        startNextTask();
}
void Worker::startNextTask()
{
    const auto task = queue.first();
    task->start();
    QVariantMap map;
    map["app_abstract"] = task->getApplicationAbstract().toVariant();
}

void Worker::handleTaskCompleted() {

}

void Worker::handleTaskChanged() {

}
