#include "change.h"

Change::Change(QString id, QString appId, QString releaseId)
{
    this->id = id;
    this->appId = appId;
    this->releaseId = releaseId;
    this->status = CREATED;
}

Change::~Change()
{

}

QString Change::description ()
{
    return "Changing release " + releaseId;
}

void Change::attachRegistry(Registry *registry)
{
    m_registry = registry;
}
