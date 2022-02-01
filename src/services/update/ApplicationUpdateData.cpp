#include "ApplicationUpdateData.h"

#include <utility>

ApplicationUpdateData::ApplicationUpdateData(Application application, QDateTime checkDate)
    : updateAvailable(false)
    , checkDate(std::move(checkDate))
    , application(std::move(application))
{
}
