#ifndef PROGRESSLISTENER_H
#define PROGRESSLISTENER_H

#include <QString>

class ProgressListener
{
public:
    virtual ~ProgressListener() {}

    virtual void progress(const int progress, const int total, const QString statusMessage) = 0;
    virtual void error(const QString &errorMessage) = 0;
    virtual void finished() = 0;
};

#endif // PROGRESSLISTENER_H
