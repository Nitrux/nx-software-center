#ifndef DUMMYUNINSTALLCHANGE_H
#define DUMMYUNINSTALLCHANGE_H

#include "dummychange.h"

class DummyUninstallChange: public DummyChange
{
public:
    DummyUninstallChange(QString appId, QString releaseId);

    virtual void finish() override;
};

#endif // DUMMYINSTALLCHANGE_H
