//
// Created by alexis on 7/24/18.
//

#include "DeployedApplicationAbstract.h"

QVariantMap DeployedApplicationAbstract::toVariant() const {
    auto result = ApplicationAbstract::toVariant();
    result["filePath"] = filePath;
    return result;
}
