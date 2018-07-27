//
// Created by alexis on 7/25/18.
//

#include <QFile>
#include <appimage/appimage.h>

#include "RemoveTask.h"

void RemoveTask::start() {

    emit changed();
    const auto path = appImageInfo.file.path;
    QFile::remove(path);
    int res = appimage_unregister_in_system(path.toStdString().c_str(), false);


    if (!res)
        emit Task::completed();
    else
        emit Task::failed("Something went wrong! That's all we know.");
}

void RemoveTask::stop() {
}

RemoveTask::RemoveTask(const AppImageInfo &appImageInfo) {
    Task::appImageInfo = appImageInfo;
}
