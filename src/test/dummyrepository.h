#ifndef DUMMYREPOSITORY_H
#define DUMMYREPOSITORY_H

#include "../entities/repository.h"

class DummyRepository : public Repository
{
public:
    DummyRepository();

    virtual QString id();

    virtual void updateCache() override;
};

#endif // DUMMYREPOSITORY_H
