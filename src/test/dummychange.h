#ifndef DUMMYCHANGE_H
#define DUMMYCHANGE_H

#include "../entities/change.h"

class DummyChange : public Change
{
public:
    DummyChange(std::string id, std::string target_release_id);

    virtual bool execute() override;
};

#endif // DUMMYCHANGE_H
