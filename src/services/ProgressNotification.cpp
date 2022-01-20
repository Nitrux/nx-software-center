#include "ProgressNotification.h"

#include <QUuid>

ProgressNotification::ProgressNotification()
    : id(QUuid::createUuid().toString())
    , status(CREATED)
{
}
