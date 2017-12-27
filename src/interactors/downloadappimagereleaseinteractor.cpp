#include "downloadappimagereleaseinteractor.h"

#include <QDir>
#include <QFile>

#include "downloadappimagereleaseinteractorlistener.h"
#include <appimage/libappimage.h>

#include "../entities/app.h"
#include "../entities/downloadmanager.h"
#include "../entities/registry.h"
#include "../entities/release.h"
#include "../entities/repository.h"

DownloadAppImageReleaseInteractor::DownloadAppImageReleaseInteractor(
    QString appImageId, QString appImageReleaseId,
    QList<Repository *> repositories, DownloadManager *downloadManager,
    Registry *registry, DownloadAppImageReleaseInteractorListener *listener)
    : Interactor() {
  m_appImageId = appImageId;
  m_appImageReleaseId = appImageReleaseId;
  m_repositories = repositories;

  Q_ASSERT(downloadManager != nullptr);
  m_downloadManager = downloadManager;

  Q_ASSERT(registry != nullptr);
  m_registry = registry;

  Q_ASSERT(listener != nullptr);
  m_listener = listener;
}

DownloadAppImageReleaseInteractor::~DownloadAppImageReleaseInteractor() {}

void DownloadAppImageReleaseInteractor::execute() {
  // check if was downloaded previously
  QString releaseFilePath =
      m_registry->getReleaseFilePath(m_appImageId, m_appImageReleaseId);
  QFile targetFile(releaseFilePath);
  if (!releaseFilePath.isEmpty() && targetFile.exists()) {
    m_listener->downloadComplete(releaseFilePath);
    return;
  } else {
    // There is a file with the same name but is not registered
    // let's remove it and download it again in order to be sure.
    if (targetFile.exists())
      targetFile.remove();

    m_registry->registerReleaseRemove(m_appImageId, m_appImageReleaseId);
  }

  Release *release = nullptr;
  for (Repository *repository : m_repositories) {
    if (repository->contains(m_appImageId, m_appImageReleaseId)) {
      release = repository->getRelease(m_appImageId, m_appImageReleaseId);
      break;
    }
  }

  if (release == nullptr)
    m_listener->error("Release not found.");
  else if (release->download_link.isEmpty())
    m_listener->error("No download link.");
  else {
    QString newFileName = QString("%1-%2-%3-%4.AppImage")
                              .arg(release->app_id)
                              .arg(release->version)
                              .arg(release->id)
                              .arg(release->arch);

    QDir destination(storagePath.replace("$HOME", QDir::homePath()));
    QString targetFilePath = destination.absoluteFilePath(newFileName);

    destination.mkpath(storagePath);

    m_downloadManager->download(release->download_link, targetFilePath,
                                m_listener);
    QFile file(targetFilePath);
    if (file.exists()) {
      file.setPermissions(QFileDevice::ExeUser | QFileDevice::ReadOwner);
      appimage_register_in_system(targetFilePath.toLocal8Bit(), 0);
      m_registry->registerReleaseDownload(m_appImageId, m_appImageReleaseId,
                                          targetFilePath);
      m_listener->downloadComplete(targetFilePath);
    } else
      m_listener->error("Unable to download, somethig whent wrong.");
  }
}
