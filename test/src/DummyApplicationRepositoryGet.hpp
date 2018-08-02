//
// Created by alexis on 7/12/18.
//

#ifndef NX_SOFTWARE_CENTER_DUMMYAPPLICATIONREPOSITORYGET_H
#define NX_SOFTWARE_CENTER_DUMMYAPPLICATIONREPOSITORYGET_H

#include <gateways/ApplicationRepositoryGet.h>

class DummyApplicationRepositoryGet : public ApplicationRepositoryGet {
Q_OBJECT

public:
    void start() override {
        application.id = "echo.desktop";
        application.name["null"] = "Echo Application";
        application.abstract["null"] = "Echoes the input to the output";
        application.description["null"] = "A simple app that echoes the input to the output";
        application.icon = TEST_DATA_DIR"/echo-icon.svg";
        application.categories = QStringList{"Utilities"};
        application.developer.name = "Jon Doe";

        Application::File f;
        f.type = 2;
        f.url = "file://" TEST_DATA_DIR "echo-x86_64-8.25.AppImage";
        f.architecture = "x86_64";
        f.size = 121752;

        Application::Release r;
        r.date = QDateTime::currentDateTime();
        r.version = "8.25";
        r.files = {f};

        application.releases = {r};

        emit completed();

    }

    void stop() override {
        emit completed();
    }
};


#endif //NX_SOFTWARE_CENTER_DUMMYAPPLICATIONREPOSITORYGET_H
