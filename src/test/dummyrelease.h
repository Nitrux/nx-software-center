#ifndef DUMMYRELEASE_H
#define DUMMYRELEASE_H

#include "../entities/release.h"

class DummyRelease : public Release
{
public:
    DummyRelease(QString id, QString app_id);

    virtual Change *download();
    virtual Change *install();
    virtual Change *uninstall();
    virtual Change *remove();
};

#endif // DUMMYRELEASE_H
