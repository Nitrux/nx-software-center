#ifndef RELEASE_H
#define RELEASE_H

#include <list>
#include <string>
#include <map>

class Change;

class Release
{
public:
    Release(std::string id, std::string app_id);
    virtual ~Release();

    std::string id;
    std::string app_id;
    std::string arch;

    std::string name;
    std::string description;

    std::list<std::string> authors;

    std::string licence;

    std::string download_link;
    std::string icon_link;
    std::list<std::string> screenshot_links;

    virtual Change* download() = 0;
    virtual Change* install() = 0;
    virtual Change* uninstall() = 0;
    virtual Change* remove() = 0;

    bool operator == (const Release& r) const;
    bool operator < (const Release& r) const;

    int Compare (const Release& d) const;
};

#endif // RELEASE_H
