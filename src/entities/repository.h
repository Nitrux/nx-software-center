#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <list>
#include <string>

class App;

class Repository
{
public:
    Repository();
    virtual ~Repository();

    virtual void updateCache() = 0;
    virtual std::list<App*> list(std::string filter = "",
                                 std::string arch = "",
                                 int offset = 0,
                                 int limit = -1);
};

#endif // REPOSITORY_H
