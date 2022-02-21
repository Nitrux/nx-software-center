#include "DummyProgressNotificationSource.h"

#include <QDebug>

DummyProgressNotificationSource::DummyProgressNotificationSource(QObject *parent)
    : QObject(parent)
{
    progress.title = "Dummy task";
    progress.subTitle = "Here goes a subtitle";
    progress.iconPath = "qrc:/download.svg";
    progress.status = TaskData::RUNNING;

    progress.total_progress = 100;

    TaskActionData data;
    data.id = "cancel-task";
    data.icon = "qrc:/download.svg";
    data.active = true;
    data.label = "cancel";
    progress.actions.push_back(data);

    emitProgressNotification();

    connect(&timer, &QTimer::timeout, this, &DummyProgressNotificationSource::emitProgressNotification);
    timer.start(2000);
}
void DummyProgressNotificationSource::emitProgressNotification()
{
    progress.current_progress = (this->progress.current_progress + 5) % 100;
    progress.subTitle = "Dummy Progress " + QString::number(this->progress.current_progress);

    qDebug() << progress.subTitle;
    emit(progressNotification(progress));
}
