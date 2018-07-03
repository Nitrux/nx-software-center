//
// Created by alexis on 7/3/18.
//

#include "ApplicationListModel.h"
#include <QDebug>
#include "LocalizationUtils.h"
QVariant LocalizationUtils::getLocalizedValue(const QMap<QString, QVariant>& lcField)
{
    auto locale = QLocale::system();
    auto lcName = locale.bcp47Name();

    QVariant value;
    for (QString k: lcField.keys()) {
            if (k.startsWith(lcName)) {
                value = lcField[k];
                break;
            }
        }
    if (value.isNull() && lcField.contains("null"))
            value = lcField["null"];

    if (value.isNull() && !lcField.isEmpty())
            value = lcField.values().first();
    return value;
}