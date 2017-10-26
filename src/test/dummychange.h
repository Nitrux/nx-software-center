#ifndef DUMMYCHANGE_H
#define DUMMYCHANGE_H

#include "../entities/change.h"

class DummyChange : public Change
{
public:
    DummyChange(QString appId, QString releaseId);

    virtual bool execute() override;
    virtual void finish() = 0;

    virtual void progress(int &current_progress, int &total_progress, QString &message) override;
};

#endif // DUMMYCHANGE_H
