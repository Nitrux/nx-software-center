#include "taskfetchappscontroller.h"

#include "../interactors/fetchappimagehubinteractor.h"
TaskFetchAppsController::TaskFetchAppsController(Repository *repository,
                                                 QObject *parent)
    : TaskController(parent) {

  Q_ASSERT(repository != nullptr);
  this->repository = repository;
  fetchAppImageHubInteractor = new FetchAppImageHubInteractor(
      "https://appimage.github.io/feed.json", repository);

  m_state = TASK_CREATED;
}

TaskFetchAppsController::~TaskFetchAppsController() {
  fetchAppImageHubInteractor->deleteLater();
}

void TaskFetchAppsController::run() {
  if (m_state == TASK_CREATED) {
    connect(fetchAppImageHubInteractor, &FetchAppImageHubInteractor::complete,
            this, &TaskFetchAppsController::handleFetchCompleted);
    fetchAppImageHubInteractor->execute();
    m_state = TASK_RUNNING;
    emit stateChanged(m_state);
  }
}

void TaskFetchAppsController::handleFetchCompleted() {
  m_state = TASK_COMPLETED;
  emit stateChanged(m_state);
}
