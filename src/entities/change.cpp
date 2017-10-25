#include "change.h"

Change::Change(QString id, QString target_release_id)
{
    this->id = id;
    this->target_release_id = target_release_id;
    this->status = CREATED;
}

Change::~Change()
{

}

QString Change::description ()
{
    return "Changing release " + target_release_id;
}

void Change::attachRegistry(Registry *registry)
{
    m_registry = registry;
}
