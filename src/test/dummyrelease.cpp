#include "dummyrelease.h"

#include "dummyinstallchange.h"
#include "dummydownloadchange.h"
#include "dummyuninstallchange.h"
#include "dummyremovechange.h"

DummyRelease::DummyRelease(QString id, QString app_id):Release(id, app_id)
{
    name = id + " "  + app_id;
}

Change *DummyRelease::download()
{
    Change * change = new DummyDownloadChange(this->id);
    return change;
}

Change *DummyRelease::install()
{
    Change * change = new DummyInstallChange(this->id);
    return change;
}

Change *DummyRelease::uninstall()
{
    Change * change = new DummyUninstallChange(this->id);
    return change;
}

Change *DummyRelease::remove()
{
    Change * change = new DummyRemoveChange(this->id);
    return change;
}
