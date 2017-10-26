#ifndef DUMMYREMOVECHANGE_H
#define DUMMYREMOVECHANGE_H

#include "dummychange.h"

class DummyRemoveChange: public DummyChange
{
public:
    DummyRemoveChange(QString appId, QString releaseId);

    virtual void finish() override;
};

#endif // DUMMYREMOVECHANGE_H
