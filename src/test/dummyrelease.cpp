#include "dummyrelease.h"

#include "dummychange.h"

DummyRelease::DummyRelease(QString id, QString app_id):Release(id, app_id)
{

}

Change *DummyRelease::download()
{
    Change * change = new DummyChange(this->id);
    return change;
}

Change *DummyRelease::install()
{
    Change * change = new DummyChange(this->id);
    return change;
}

Change *DummyRelease::uninstall()
{
    Change * change = new DummyChange(this->id);
    return change;
}

Change *DummyRelease::remove()
{
    Change * change = new DummyChange(this->id);
    return change;
}
