#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QList>
#include <QMap>
#include <QString>

class App;
class Release;

class Repository {
public:
  Repository();
  virtual ~Repository();

  virtual QList<App *> list();
  virtual bool contains(QString appId);
  virtual bool contains(QString appId, QString releaseId);
  virtual App *getApp(QString appId);
  virtual Release *getRelease(QString appId, QString releaseId);

  virtual void add(App *app);
  virtual void clear();

protected:
  QMap<QString, App *> apps;
};

#endif // REPOSITORY_H
