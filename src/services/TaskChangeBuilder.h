#pragma once
// system

// libraries
#include "ApplicationData.h"
#include <QMetaEnum>
#include <QObject>
#include <QVariantMap>

// local

/**
 * Creates Tasks updates to be emitted from the different places of the application
 */
class TaskChangeBuilder : public QObject
{
    Q_OBJECT
public:
    explicit TaskChangeBuilder(QObject *parent = nullptr);
    enum Field { REFERENCE = 0, ID, TITLE, SUBTITLE, ICON, CURRENT_PROGRESS, TOTAL_PROGRESS, STATUS, TARGET_APP_ID };
    enum Status { CREATED = 1, RUNNING, FAILED, SUCCEED };

    Q_ENUM(Field)
    Q_ENUM(Status)

    void set(Field field, const QVariant &value);

    QVariantMap build();

private:
    QVariantMap _data;
    QMetaEnum _fieldNames;
    QMetaEnum _statusNames;
};
