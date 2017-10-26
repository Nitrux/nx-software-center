#ifndef CHANGE_H
#define CHANGE_H

#include <QList>
#include <QString>

class Registry;

class Change
{
public:
    enum Status {
        CREATED = 0,
        RUNNING,
        PAUSED,
        FINISHED,
        FAILED
    };

    Change(QString id, QString appId, QString releaseId);
    virtual ~Change();

    QString id;
    QString appId;
    QString releaseId;

    Status status;
    QList<QString> logs;

    virtual void progress(int &current_progress, int &total_progress, QString &message) = 0;
    virtual QString description();

    virtual bool execute() = 0;

    void attachRegistry(Registry * registry);

protected:
    Registry * m_registry = nullptr;
};


#endif // CHANGE_H
