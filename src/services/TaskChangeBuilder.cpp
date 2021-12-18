#include "TaskChangeBuilder.h"

#include <QUuid>
#include <utility>

TaskChangeBuilder::TaskChangeBuilder(QObject *parent)
    : QObject(parent)
    , _fieldNames(QMetaEnum::fromType<TaskChangeBuilder::Field>())
    , _statusNames(QMetaEnum::fromType<TaskChangeBuilder::Status>())
{
    QString task_id = QUuid::createUuid().toString();
    _data.insert(_fieldNames.key(Field::ID), task_id);
}

QVariantMap TaskChangeBuilder::build()
{
    QString reference_id = QUuid::createUuid().toString();
    _data.insert(_fieldNames.key(Field::REFERENCE), reference_id);
    return _data;
}
void TaskChangeBuilder::set(TaskChangeBuilder::Field field, const QVariant &value)
{
    _data.insert(_fieldNames.key(field), value);
}
