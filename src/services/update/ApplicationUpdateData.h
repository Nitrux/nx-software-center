#pragma once
// system

// libraries

// local

#include "services/Application.h"

/**
 * Data transfer object for Applications update information
 */
class ApplicationUpdateData
{
public:
    explicit ApplicationUpdateData(Application application = {}, QDateTime checkDate = QDateTime::currentDateTime());

    bool updateAvailable;
    QString source;
    QDateTime checkDate;
    QDateTime updateDate;

    QString version;
    QString changesLog;
    Application application;
};

Q_DECLARE_METATYPE(ApplicationUpdateData)