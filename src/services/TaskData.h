#pragma once
// system

// libraries
#include <QList>
#include <QMetaType>
#include <QString>
#include <QVariantMap>

// local

class TaskActionData
{
public:
    QString id;
    QString label;
    QString icon;
    bool active;

    [[nodiscard]] QVariantMap toVariant() const;
};

typedef QList<TaskActionData> TasksActionDataList;

class TaskData
{
public:
    enum Status { CREATED = 1, RUNNING, FAILED, SUCCEED };
    TaskData();

    QString id;

    // task information
    Status status;
    QString title;
    QString subTitle;
    QString iconPath;
    qlonglong current_progress;
    qlonglong total_progress;

    // related app id
    QString related_app_id;
    TasksActionDataList actions;
};

Q_DECLARE_METATYPE(TaskData)
Q_DECLARE_METATYPE(TaskActionData)
Q_DECLARE_METATYPE(TasksActionDataList)
