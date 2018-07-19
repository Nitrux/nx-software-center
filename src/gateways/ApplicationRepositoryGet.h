//
// Created by alexis on 7/10/18.
//

#ifndef NX_SOFTWARE_CENTER_APPLICATIONGET_H
#define NX_SOFTWARE_CENTER_APPLICATIONGET_H

#include <QObject>
#include <entities/ApplicationFull.h>

class ApplicationRepositoryGet : public QObject {
Q_OBJECT
protected:
    QString id;
    ApplicationFull application;
public:
    virtual void start() = 0;

    virtual void stop() = 0;

    void setId(const QString &id);

    ApplicationFull getApplication() const;

signals:
    void completed();
    void failed(const QString &reason);
};


#endif //NX_SOFTWARE_CENTER_APPLICATIONGET_H
