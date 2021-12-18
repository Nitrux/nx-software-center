#include "UpdateCheckData.h"

#include <utility>

UpdateCheckData::UpdateCheckData(ApplicationData application, bool updateAvailable, QDateTime lastCheckDate)
    : updateAvailable(updateAvailable)
    , lastCheckDate(std::move(lastCheckDate))
    , application(std::move(application))
{
}
