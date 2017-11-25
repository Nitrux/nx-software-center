#include "removeappimagereleaseinteractor.h"

#include <QFile>

#include "appimage/appimage.h"

#include "../entities/registry.h"
#include "removeappimagereleaseinteractorlistener.h"

RemoveAppImageReleaseInteractor::RemoveAppImageReleaseInteractor(
    QString appId, QString releaseId, Registry *registry) {
  m_appId = appId;
  m_releaseId = releaseId;

  Q_ASSERT(registry != nullptr);
  m_registry = registry;
}

void RemoveAppImageReleaseInteractor::removeFile(
    const QString &releaseFilePath) {
  QFile file(releaseFilePath);
  if (file.exists() && !file.remove()) {
    // The file exist but was imposible to remove it.
    FileNotRemovedException ex(releaseFilePath);
    ex.raise();
  }
}

void RemoveAppImageReleaseInteractor::execute() {
  QString releaseFilePath =
      m_registry->getReleaseFilePath(m_appId, m_releaseId);

  if (!releaseFilePath.isEmpty()) {
    appimage_unregister_in_system(releaseFilePath.toLocal8Bit(), 0);

    removeFile(releaseFilePath);

    m_registry->registerReleaseRemove(m_appId, m_releaseId);
    emit finished();
  } else {
    ReleaseFileFoundException ex(m_appId, m_releaseId);
    ex.raise();
  }
}

FileNotRemovedException::FileNotRemovedException(const QString &path) {
  this->path = path;
}

void FileNotRemovedException::raise() const { throw * this; }

FileNotRemovedException *FileNotRemovedException::clone() const {
  return new FileNotRemovedException(*this);
}

const QString FileNotRemovedException::getPath() { return this->path; }

ReleaseFileFoundException::ReleaseFileFoundException(const QString &appId,
                                                     const QString &releaseId) {
  this->appId = appId;
  this->releaseId = releaseId;
}

void ReleaseFileFoundException::raise() const { throw * this; }

ReleaseFileFoundException *ReleaseFileFoundException::clone() const {
  return new ReleaseFileFoundException(*this);
}

const QString ReleaseFileFoundException::getAppId() { return appId; }

const QString ReleaseFileFoundException::getReleaseId() { return releaseId; }
