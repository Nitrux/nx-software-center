#pragma once
// system

// libraries

// local

#include "services/ApplicationData.h"

/**
 * Data transfer object for Applications update information
 */
class UpdateInformation
{
public:
    explicit UpdateInformation(ApplicationData application = {}, QDateTime checkDate = QDateTime::currentDateTime());

    bool updateAvailable;
    QString source;
    QDateTime checkDate;

    QString version;
    QString changesLog;
    ApplicationData application;
};

Q_DECLARE_METATYPE(UpdateInformation)