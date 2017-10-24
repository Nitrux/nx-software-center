#include "app.h"

#include <algorithm>

#include "release.h"

App::App(std::string id)
{
    this->id = id;
}

Release *App::lastRelease()
{
    return m_releases.back();
}

void App::addRelease(Release *release)
{
    auto position = std::lower_bound(m_releases.begin(),m_releases.end(), release);
    m_releases.insert(position, release);
}

std::list<Release *> App::releases()
{
    return m_releases;
}
