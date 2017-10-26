#include "dummychange.h"

#include <QDebug>
#include <QTimer>

#include "../entities/registry.h"

DummyChange::DummyChange(QString appId, QString releaseId): Change("dummy_change", appId, releaseId)
{

}

bool DummyChange::execute()
{
    status = RUNNING;
    logs.push_front("Executing change");

    return true;
}


void DummyChange::progress(int &current_progress, int &total_progress, QString &message)
{
    if (status == CREATED)
    {
        current_progress = 0;
        total_progress = 100;
        message = "Just created";
    }

    if (status == RUNNING)
    {
        current_progress = 50;
        total_progress = 100;
        message = "Running";
    }

    if (status == FINISHED)
    {
        current_progress = 100;
        total_progress = 100;
        message = "Complete successfuly ";
    }
}
