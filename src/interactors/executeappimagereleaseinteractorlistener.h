#ifndef EXECUTEAPPIMAGERELEASEINTERACTORLISTENER_H
#define EXECUTEAPPIMAGERELEASEINTERACTORLISTENER_H

#include <QString>

class ExecuteAppImageReleaseInteractorListener
{
public:
    virtual ~ExecuteAppImageReleaseInteractorListener() {}

    virtual void success() = 0;
    virtual void fail(QString message) = 0;
};

#endif // EXECUTEAPPIMAGERELEASEINTERACTORLISTENER_H
