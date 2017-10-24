#include "dummyrelease.h"

#include "dummychange.h"

DummyRelease::DummyRelease(std::string id, std::string app_id):Release(id, app_id)
{

}

Change *DummyRelease::download()
{
    Change * change = new DummyChange("random dummy download change", this->id);
    return change;
}

Change *DummyRelease::install()
{
    Change * change = new DummyChange("random dummy install change", this->id);
    return change;
}

Change *DummyRelease::uninstall()
{
    Change * change = new DummyChange("random dummy uninstall change", this->id);
    return change;
}

Change *DummyRelease::remove()
{
    Change * change = new DummyChange("random dummy remove change", this->id);
    return change;
}
