#pragma once
// system

// libraries

// local

#include "services/ApplicationData.h"

/**
 * Data transfer object for Applications update information
 */
class ApplicationUpdateData
{
public:
    explicit ApplicationUpdateData(ApplicationData application = {}, QDateTime checkDate = QDateTime::currentDateTime());

    bool updateAvailable;
    QString source;
    QDateTime checkDate;
    QDateTime updateDate;

    QString version;
    QString changesLog;
    ApplicationData application;
};

Q_DECLARE_METATYPE(ApplicationUpdateData)