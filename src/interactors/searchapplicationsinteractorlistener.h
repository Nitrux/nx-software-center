#ifndef SEARCHAPPLICATIONSINTERACTORLISTENER_H
#define SEARCHAPPLICATIONSINTERACTORLISTENER_H

#include <QVariant>

class SearchApplicationsInteractorListener
{
public:
    virtual ~SearchApplicationsInteractorListener() {}
    virtual void handleApplicationsList(const QVariantList &applicationsList) = 0;
    virtual void handleNoApplicationsFound() = 0;
};

#endif // SEARCHAPPLICATIONSINTERACTORLISTENER_H
