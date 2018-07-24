//
// Created by alexis on 7/24/18.
//

#ifndef NX_SOFTWARE_CENTER_DEPLOYEDAPPLICATIONABSTRACT_H
#define NX_SOFTWARE_CENTER_DEPLOYEDAPPLICATIONABSTRACT_H

#include "ApplicationAbstract.h"

class DeployedApplicationAbstract : public ApplicationAbstract {
public:
    QString filePath;

    QVariantMap toVariant() const override;
};


#endif //NX_SOFTWARE_CENTER_DEPLOYEDAPPLICATIONABSTRACT_H
