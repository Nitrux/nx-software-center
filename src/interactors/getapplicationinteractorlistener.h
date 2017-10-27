#ifndef GETAPPLICATIONINTERACTORLISTENER_H
#define GETAPPLICATIONINTERACTORLISTENER_H

#include <QVariantMap>

class GetApplicationInteractorListener
{
public:
    virtual ~GetApplicationInteractorListener() {}
    virtual void handleApplicationDetails(QVariantMap details) = 0;
    virtual void handleNoApplicationFound() = 0;
};

#endif // GETAPPLICATIONINTERACTORLISTENER_H
