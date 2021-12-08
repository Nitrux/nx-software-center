#include "ApplicationBundle.h"
#include "ApplicationData.h"

ApplicationBundle::ApplicationBundle(const QString &path, ApplicationData *data)
    : path(path)
    , app(data)
{
    if (app.isNull())
        app.reset(new ApplicationData());
}

bool ApplicationBundle::operator==(const ApplicationBundle &rhs) const
{
    return path == rhs.path;
}

bool ApplicationBundle::operator!=(const ApplicationBundle &rhs) const
{
    return !(rhs == *this);
}
