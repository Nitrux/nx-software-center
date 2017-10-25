#ifndef DUMMYDOWNLOADCHANGE_H
#define DUMMYDOWNLOADCHANGE_H

#include "dummychange.h"

class DummyDownloadChange: public DummyChange
{
public:
    DummyDownloadChange(QString target_release_id);

    virtual void finish() override;
};

#endif // DUMMYDOWNLOADCHANGE_H
