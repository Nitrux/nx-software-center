//
// Created by alexis on 7/4/18.
//

#ifndef NX_SOFTWARE_CENTER_APPLICATIONABSTRACT_H
#define NX_SOFTWARE_CENTER_APPLICATIONABSTRACT_H

#include <QtCore/QString>
#include <ostream>
#include "AppImageInfo.h"

class ApplicationAbstract {
public:
    QString id;
    QString icon;
    AppImageInfo::LocalizedQString name;
    AppImageInfo::LocalizedQString abstract;
    int fileSize;
    QString latestReleaseVersion;

    ApplicationAbstract();
    friend std::ostream& operator<<(std::ostream& os, const ApplicationAbstract& abstract);
};

#endif //NX_SOFTWARE_CENTER_APPLICATIONABSTRACT_H
