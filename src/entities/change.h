#ifndef CHANGE_H
#define CHANGE_H

#include <list>
#include <string>


class Change
{
    enum Status {
        CREATED = 0,
        RUNNING,
        PAUSED,
        FINISHED,
        FAILED
    };

public:
    Change(std::string id, std::string target_release_id);
    virtual ~Change();

    std::string id;
    std::string target_release_id;

    Status status;
    std::list<std::string> logs;

    virtual std::string description();

    virtual bool execute() = 0;
};

#endif // CHANGE_H
