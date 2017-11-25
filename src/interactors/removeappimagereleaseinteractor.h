#ifndef REMOVEAPPIMAGERELEASEINTERACTOR_H
#define REMOVEAPPIMAGERELEASEINTERACTOR_H

#include "interactor.h"

#include <QException>
#include <QString>

class Registry;
class FileNotRemovedException;
class ReleaseFileFoundException;
class RemoveAppImageReleaseInteractor : public Interactor {
  Q_OBJECT

  QString m_appId;
  QString m_releaseId;
  Registry *m_registry;

public:
  RemoveAppImageReleaseInteractor(QString appId, QString releaseId,
                                  Registry *registry);

signals:
  void finished();

public slots:
  virtual void execute() override;

private:
  void removeFile(const QString &releaseFilePath);
};

class FileNotRemovedException : public QException {
  QString path;

public:
  FileNotRemovedException(const QString &path);
  void raise() const;
  FileNotRemovedException *clone() const;
  const QString getPath();
};

class ReleaseFileFoundException : public QException {
  QString appId;
  QString releaseId;

public:
  ReleaseFileFoundException(const QString &appId, const QString &releaseId);
  void raise() const;
  ReleaseFileFoundException *clone() const;
  const QString getAppId();
  const QString getReleaseId();
};

#endif // REMOVEAPPIMAGERELEASEINTERACTOR_H
