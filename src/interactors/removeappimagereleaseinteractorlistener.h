#ifndef REMOVEAPPIMAGERELEASEINTERACTORLISTENER_H
#define REMOVEAPPIMAGERELEASEINTERACTORLISTENER_H

#include <QString>

class RemoveAppImageReleaseInteractorListener
{
public:
    virtual ~RemoveAppImageReleaseInteractorListener() {}

    virtual void finished() = 0;
    virtual void error(const QString &errorMessage) = 0;
};

#endif // REMOVEAPPIMAGERELEASEINTERACTORLISTENER_H
