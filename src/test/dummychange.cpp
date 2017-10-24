#include "dummychange.h"

#include <QDebug>

DummyChange::DummyChange(std::string id, std::string target_release_id): Change(id, target_release_id)
{

}

bool DummyChange::execute()
{
    qDebug() << "Executing change";
    return true;
}
