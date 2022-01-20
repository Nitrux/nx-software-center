#include "TaskData.h"

#include <QUuid>

TaskData::TaskData()
    : id(QUuid::createUuid().toString())
    , status(CREATED)
{
}
