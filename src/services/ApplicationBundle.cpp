#include "ApplicationBundle.h"

#include "ApplicationData.h"
#include <utility>

ApplicationBundle::ApplicationBundle()
    : app(new ApplicationData())
{
}

ApplicationBundle::ApplicationBundle(QString path, ApplicationData *data)
    : path(std::move(path))
    , app(data)
{
    if (app.isNull())
        app.reset(new ApplicationData(QVariantMap()));
}

bool ApplicationBundle::operator==(const ApplicationBundle &rhs) const
{
    return path == rhs.path;
}
bool ApplicationBundle::operator!=(const ApplicationBundle &rhs) const
{
    return !(rhs == *this);
}
