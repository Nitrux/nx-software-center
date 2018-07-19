//
// Created by alexis on 7/12/18.
//

#ifndef NX_SOFTWARE_CENTER_DUMMYAPPLICATIONREPOSITORYSEARCH_H
#define NX_SOFTWARE_CENTER_DUMMYAPPLICATIONREPOSITORYSEARCH_H

#include <gateways/ApplicationsRepositorySearch.h>

class DummyApplicationRepositorySearch : public ApplicationsRepositorySearch {
Q_OBJECT
public:
    void start() override {
        ApplicationAbstract applicationAbstract;
        applicationAbstract.id = "echo.desktop";
        applicationAbstract.latestReleaseVersion = "8.25";
        applicationAbstract.fileSize = 121752;
        applicationAbstract.name["null"] = "Echo Application";
        applicationAbstract.abstract["null"] = "Echoes the input to the output";

        results = {applicationAbstract};
        emit completed();
    }

    void stop() override {
        emit completed();
    }


};


#endif //NX_SOFTWARE_CENTER_DUMMYAPPLICATIONREPOSITORYSEARCH_H
