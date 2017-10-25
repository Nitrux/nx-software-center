#ifndef DUMMYCHANGE_H
#define DUMMYCHANGE_H

#include "../entities/change.h"

class DummyChange : public Change
{
public:
    DummyChange(QString target_release_id);

    virtual bool execute() override;
    void finish();

    virtual void progress(int &current_progress, int &total_progress, QString &message) override;
};

#endif // DUMMYCHANGE_H
