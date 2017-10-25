#ifndef DUMMYREMOVECHANGE_H
#define DUMMYREMOVECHANGE_H

#include "dummychange.h"

class DummyRemoveChange: public DummyChange
{
public:
    DummyRemoveChange(QString target_release_id);

    virtual void finish() override;
};

#endif // DUMMYREMOVECHANGE_H
