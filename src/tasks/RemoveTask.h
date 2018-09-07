//
// Created by alexis on 7/25/18.
//

#ifndef NX_SOFTWARE_CENTER_REMOVETASK_H
#define NX_SOFTWARE_CENTER_REMOVETASK_H


#include "entities/Task.h"

class RemoveTask : public Task {

public:
    explicit RemoveTask(const AppImageInfo &appImageInfo);

    void start() override;

    void stop() override;

};


#endif //NX_SOFTWARE_CENTER_REMOVETASK_H
