#include "registry.h"

Registry::Registry()
{

}

std::list<Change *> Registry::changes()
{
    return m_changes;
}
