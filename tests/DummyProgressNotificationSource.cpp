#include "DummyProgressNotificationSource.h"

#include <QDebug>

DummyProgressNotificationSource::DummyProgressNotificationSource(QObject *parent)
    : QObject(parent)
{
    timer.setInterval(500);
    progress.title = "Dummy task";
    progress.subTitle = "Here goes a subtitle";
    progress.iconPath = "package";
    progress.status = TaskData::RUNNING;

    progress.total_progress = 100;

    TaskActionData data;
    data.id = "zxcasd";
    data.icon = "cancel";
    data.active = true;
    data.label = "cancel";
    progress.actions.push_back(data);

    connect(&timer, &QTimer::timeout, [this] {
        this->progress.current_progress = (this->progress.current_progress + 5) % 100;
        qDebug() << "Dummy Progress" << this->progress.current_progress;
        emit(progressNotification(progress));
    });

    timer.start();
}
