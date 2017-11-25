#include "taskdownloadappimagecontroller.h"

#include "../interactors/downloadappimagereleaseinteractor.h"
#include "../interactors/interactor.h"
#include <QtConcurrent>

TaskDownloadAppImageController::TaskDownloadAppImageController(
    QString appImageId, QString appImageReleaseId,
    QList<Repository *> repositories, DownloadManager *downloadManager,
    Registry *registry, QObject *parent)
    : TaskController(parent) {
  m_interactor = new DownloadAppImageReleaseInteractor(
      appImageId, appImageReleaseId, repositories, downloadManager, registry,
      this);
}

void TaskDownloadAppImageController::run() {
  if (m_state == TASK_CREATED) {
    QtConcurrent::run([=]() { m_interactor->execute(); });
    emit stateChanged(m_state);
  }
}

void TaskDownloadAppImageController::downloadComplete(const QString) {
  m_state = TASK_COMPLETED;
  emit stateChanged(m_state);
}

void TaskDownloadAppImageController::progress(const int progress,
                                              const int total,
                                              const QString statusMessage) {
  if (m_progress != progress) {
    m_progress = progress;
    emit progressChanged(m_progress);
  }

  if (m_total != total) {
    m_total = total;
    emit totalChanged(m_total);
  }

  if (m_message != statusMessage) {
    m_message = statusMessage;
    emit messageChanged(m_message);
  }
}

void TaskDownloadAppImageController::error(const QString &errorMessage) {
  m_state = TASK_FAILED;
  m_message = errorMessage;

  emit stateChanged(m_state);
  emit messageChanged(m_message);
  emit finished();
}
