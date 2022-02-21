#include "TaskData.h"

#include <QUuid>

TaskData::TaskData()
    : id(QUuid::createUuid().toString())
    , status(CREATED)
{
}

QVariantMap TaskActionData::toVariant() const
{
    QVariantMap map;
    map["id"] = id;
    map["label"] = label;
    map["icon"] = icon;
    map["active"] = active;

    return map;
}
