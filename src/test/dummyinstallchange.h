#ifndef DUMMYINSTALLCHANGE_H
#define DUMMYINSTALLCHANGE_H

#include "dummychange.h"

class DummyInstallChange: public DummyChange
{
public:
    DummyInstallChange(QString appId, QString releaseId);

    virtual void finish() override;
};

#endif // DUMMYINSTALLCHANGE_H
