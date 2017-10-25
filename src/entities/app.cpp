#include "app.h"

#include <algorithm>

#include "release.h"

App::App(QString id)
{
    this->id = id;
}

Release *App::lastRelease()
{
    return m_releases.last();
}

void App::addRelease(Release *release)
{
    m_releases.insert(release->id, release);
}

Release *App::getRelease(QString releaseId)
{
    return m_releases.value(releaseId, nullptr);
}

QList<Release *> App::releases()
{
    return m_releases.values();
}
