#ifndef DUMMYRELEASE_H
#define DUMMYRELEASE_H

#include "../entities/release.h"

class DummyRelease : public Release
{
public:
    DummyRelease(std::string id, std::string app_id);

    virtual Change *download() override;
    virtual Change *install() override;
    virtual Change *uninstall() override;
    virtual Change *remove() override;
};

#endif // DUMMYRELEASE_H
