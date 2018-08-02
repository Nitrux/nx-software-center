//
// Created by alexis on 7/4/18.
//

#ifndef NX_SOFTWARE_CENTER_APPLICATIONABSTRACT_H
#define NX_SOFTWARE_CENTER_APPLICATIONABSTRACT_H

#include <ostream>
#include <QString>
#include "Application.h"

class ApplicationAbstract {
public:
    virtual ~ApplicationAbstract();

    QString id;
    QString icon;
    Application::LocalizedQString name;
    Application::LocalizedQString abstract;
    int fileSize;
    QString latestReleaseVersion;

    ApplicationAbstract();
    friend std::ostream& operator<<(std::ostream& os, const ApplicationAbstract& abstract);

    virtual QVariantMap toVariant() const;
};

#endif //NX_SOFTWARE_CENTER_APPLICATIONABSTRACT_H
