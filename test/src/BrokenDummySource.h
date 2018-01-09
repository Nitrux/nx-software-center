//
// Created by alexis on 22/12/17.
//

#ifndef NOMAD_SOFTWARE_CENTER_DUMMYBROKENSOURCE_H
#define NOMAD_SOFTWARE_CENTER_DUMMYBROKENSOURCE_H

#include <QList>
#include <QStringList>
#include "entities/Source.h"

class BrokenDummySource : public Source {
    Q_OBJECT

public:
    void fetchAllApplications() override {
        QString e = "This a Dummy Broken Source";
        emit fetchError(e);
    }
};


#endif //NOMAD_SOFTWARE_CENTER_DUMMYBROKENSOURCE_H
