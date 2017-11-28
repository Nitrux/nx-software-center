#ifndef APPLICATION_H
#define APPLICATION_H

#include <QString>

class Application {
  QString id;
  QString name;
  QString version;

 public:
  explicit Application();
  explicit Application(const QString &name, const QString &version);

  QString getId() const;
  QString getName() const;
  QString getVersion() const;

  int Compare(const Application &a) const;
  bool operator==(const Application &a) const;
  bool operator<(const Application &a) const;

 private:
  QString generateId(const QString &id, const QString &version) const;
};

#endif  // APPLICATION_H
