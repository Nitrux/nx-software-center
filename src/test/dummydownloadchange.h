#ifndef DUMMYDOWNLOADCHANGE_H
#define DUMMYDOWNLOADCHANGE_H

#include "dummychange.h"

class DummyDownloadChange: public DummyChange
{
public:
    DummyDownloadChange(QString appId, QString releaseId);

    virtual void finish() override;
};

#endif // DUMMYDOWNLOADCHANGE_H
