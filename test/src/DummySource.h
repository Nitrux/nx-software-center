//
// Created by alexis on 22/12/17.
//

#ifndef NOMAD_SOFTWARE_CENTER_DUMMYSOURCE_H
#define NOMAD_SOFTWARE_CENTER_DUMMYSOURCE_H

#include <QList>
#include "entities/Application.h"
#include "entities/Source.h"

class DummySource : public Source {
    Q_OBJECT
    QList<Application> applications;
public:
    DummySource() {
        applications << Application("Test App 1", "v1");
        applications << Application("Test App 2", "v1");
        applications << Application("Test App 3", "v1");
        applications << Application("Test App 4", "v1");
    }

    void fetchAllApplications() override {
        emit fetchedAllApplications(applications);
    }

    QList<Application> getOutputApplications() {
        return applications;
    }
};


#endif //NOMAD_SOFTWARE_CENTER_DUMMYSOURCE_H
