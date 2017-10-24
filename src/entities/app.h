#ifndef APP_H
#define APP_H

#include <map>
#include <list>
#include <string>

class Release;

class App
{
public:
    App(std::string id);

    std::string id;


    Release* lastRelease();
    void addRelease(Release* release);
    std::list<Release*> releases();

    std::map<std::string, std::string> links;

protected:
    std::list<Release *> m_releases;
};

#endif // APP_H
