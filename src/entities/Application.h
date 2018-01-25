#ifndef APPLICATION_H
#define APPLICATION_H

#include <QString>
#include <QStringList>

class Application {
    QString id;
    QString codeName;
    QString version;

    QString name;
    QString arch;
    QString description;
    QString licence;
    QStringList authors;
    QStringList categories;
    QStringList screenshots;

    QString downloadUrl;
    QString homePageUrl;

public:
    explicit Application();

    explicit Application(const QString &codeName, const QString &version);

    QString getId() const;

    QString getVersion() const;

    QString getCodeName() const;

    QString getDescription() const;

    void setDescription(const QString &description);

    const QString &getName() const;

    void setName(const QString &name);

    const QString &getLicence() const;

    void setLicence(const QString &licence);

    const QStringList &getAuthors() const;

    void setAuthors(const QStringList &authors);

    const QStringList &getCategories() const;

    void setCategories(const QStringList &categories);

    const QString &getArch() const;

    void setArch(const QString &arch);

    const QStringList &getScreenshots() const;

    void setScreenshots(const QStringList &screenshots);

    const QString &getDownloadUrl() const;

    void setDownloadUrl(const QString &downloadUrl);

    const QString &getHomePageUrl() const;

    void setHomePageUrl(const QString &webPortalUrl);

    int compare(const Application &a) const;

    int compare_by_name(const Application &a) const;

    int compare_by_version(const Application &a) const;

    bool operator==(const Application &a) const;

    bool operator!=(const Application &a) const;

    bool operator<(const Application &a) const;

    bool isEmpty();

private:
    QString generateId(const QString &id, const QString &version) const;
};


#endif  // APPLICATION_H
