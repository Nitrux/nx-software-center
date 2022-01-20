#pragma once
// system

// libraries
#include <QMetaType>
#include <QString>

// local

class ProgressNotification
{
public:
    enum Status { CREATED = 1, RUNNING, FAILED, SUCCEED };
    ProgressNotification();

    QString id;

    // task information
    Status status;
    QString title;
    QString subTitle;
    QString iconPath;
    long current_progress;
    long total_progress;

    // related app id
    QString related_app_id;
};

Q_DECLARE_METATYPE(ProgressNotification)