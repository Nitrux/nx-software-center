#ifndef APPLICATION_H
#define APPLICATION_H

#include <QString>

class Application {
    QString id;
    QString version;

    QString name;
    QString description;

public:
    explicit Application();

    explicit Application(const QString &name, const QString &version);

    QString getId() const;

    QString getVersion() const;

    QString getName() const;

    QString getDescription() const;

    void setDescription(const QString &description);

    int compare(const Application &a) const;

    int compare_by_name(const Application &a) const;

    int compare_by_version(const Application &a) const;

    bool operator==(const Application &a) const;

    bool operator!=(const Application &a) const;

    bool operator<(const Application &a) const;

private:
    QString generateId(const QString &id, const QString &version) const;
};

#endif  // APPLICATION_H
