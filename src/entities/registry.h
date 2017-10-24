#ifndef REGISTRY_H
#define REGISTRY_H

#include <list>

class Change;

class Registry
{
public:
    Registry();

    std::list<Change *> changes();

protected:
    std::list<Change *> m_changes;
};

#endif // REGISTRY_H
