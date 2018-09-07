//
// Created by alexis on 7/25/18.
//

#include <QFile>
#include <appimage/appimage.h>

#include "RemoveTask.h"
#include <QtConcurrent/QtConcurrent>
#include <tasks/TaskMetadata.h>

void RemoveTask::start() {
    const auto path = appImageInfo.file.path;

    setStatus(Task::VALUE_STATUS_RUNNING);
    setProgressMessage("Removing " + path);
    emit changed();

    QtConcurrent::run([=]() {
        const auto path = appImageInfo.file.path;

        if (path.isEmpty()) {
            setStatus(Task::VALUE_STATUS_FAILED);
            emit failed("No file path in AppInfo.");
            return;
        }

        QFile::remove(path);
        int res = appimage_unregister_in_system(path.toStdString().c_str(), false);

        if (!res) {
            setProgressMessage("Remove completed");
            setStatus(Task::VALUE_STATUS_COMPLETED);
            emit completed();
        } else {
            setStatus(Task::VALUE_STATUS_FAILED);
            emit failed("Something went wrong! That's all we know.");
        }
    });
}

void RemoveTask::stop() {
}

RemoveTask::RemoveTask(const AppImageInfo &appImageInfo) {
    Task::appImageInfo = appImageInfo;
    type = TaskMetadata::VALUE_TYPE_REMOVE;
}

