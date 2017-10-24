#include "change.h"

Change::Change(std::string id, std::string target_release_id)
{
    this->id = id;
    this->target_release_id = target_release_id;
    this->status = CREATED;
}

Change::~Change()
{

}

std::string Change::description ()
{
    return "Changing release " + target_release_id;
}
