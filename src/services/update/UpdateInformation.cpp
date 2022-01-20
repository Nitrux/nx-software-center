#include "UpdateInformation.h"

#include <utility>

UpdateInformation::UpdateInformation(ApplicationData application, QDateTime checkDate)
    : updateAvailable(false)
    , checkDate(std::move(checkDate))
    , application(std::move(application))
{
}
