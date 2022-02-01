#include "ApplicationBundle.h"
#include "Application.h"

// system

// libraries
#include <QVersionNumber>

ApplicationBundle::ApplicationBundle()
    : size(0)
    , bundleType(Unknown)
{
}

ApplicationBundle::ApplicationBundle(QString path, ApplicationData data)
    : path(std::move(path))
    , size(0)
    , bundleType(Unknown)
    , data(std::move(data))
{
}

bool ApplicationBundle::operator==(const ApplicationBundle &rhs) const
{
    QVersionNumber v1 = QVersionNumber::fromString(data.getVersion());
    QVersionNumber v2 = QVersionNumber::fromString(rhs.data.getVersion());

    return path == rhs.path && v1 == v2;
}
bool ApplicationBundle::operator!=(const ApplicationBundle &rhs) const
{
    return !(rhs == *this);
}
bool ApplicationBundle::operator<(const ApplicationBundle &rhs) const
{
    QVersionNumber v1 = QVersionNumber::fromString(data.getVersion());
    QVersionNumber v2 = QVersionNumber::fromString(rhs.data.getVersion());

    return v1 < v2;
}
bool ApplicationBundle::operator>(const ApplicationBundle &rhs) const
{
    QVersionNumber v1 = QVersionNumber::fromString(data.getVersion());
    QVersionNumber v2 = QVersionNumber::fromString(rhs.data.getVersion());

    return v1 > v2;
}
