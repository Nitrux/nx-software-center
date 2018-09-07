//
// Created by alexis on 7/10/18.
//

#include "ApplicationRepositoryGet.h"

void ApplicationRepositoryGet::setId(const QString &id) {
    ApplicationRepositoryGet::id = id;
}

Application ApplicationRepositoryGet::getApplication() const {
    return application;
}
