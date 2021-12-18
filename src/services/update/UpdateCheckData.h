#pragma once
// system

// libraries

// local

#include "services/ApplicationData.h"

class UpdateCheckData
{
public:
    explicit UpdateCheckData(ApplicationData application = {}, bool updateAvailable = false, QDateTime lastCheckDate = QDateTime::currentDateTime());

    bool updateAvailable;
    QString source;
    QDateTime lastCheckDate;
    ApplicationData application;
};

Q_DECLARE_METATYPE(UpdateCheckData)