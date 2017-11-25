#ifndef TASKFETCHAPPSCONTROLLER_H
#define TASKFETCHAPPSCONTROLLER_H

#include <QObject>
#include "taskcontroller.h"

class Repository;
class FetchAppImageHubInteractor;
class TaskFetchAppsController : public TaskController
{
    Q_OBJECT
    Repository * repository;
    FetchAppImageHubInteractor *fetchAppImageHubInteractor;

public:
    explicit TaskFetchAppsController(Repository *repository, QObject *parent = nullptr);
    virtual ~TaskFetchAppsController();
    Q_INVOKABLE virtual void run();

protected slots:
    void handleFetchCompleted();
};

#endif // TASKFETCHAPPSCONTROLLER_H
