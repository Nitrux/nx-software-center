#ifndef APPIMAGE_H
#define APPIMAGE_H

#include <QObject>
#include <QVariantHash>

class AppImage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString licence READ licence WRITE setLicence NOTIFY licenceChanged)
    Q_PROPERTY(QStringList categories READ categories WRITE setCategories NOTIFY categoriesChanged)
    Q_PROPERTY(QStringList screenshots READ screenshots WRITE setScreenshots NOTIFY screenshotsChanged)
    Q_PROPERTY(QVariantHash links READ links WRITE setLinks NOTIFY linksChanged)
    Q_PROPERTY(QVariantList authors READ authors WRITE setAuthors NOTIFY authorsChanged)

public:
    AppImage(QObject *parent = nullptr);

    QString id() const;
    QVariantHash links() const;
    QVariantList authors() const;
    QString description() const;
    QStringList categories() const;
    QStringList screenshots() const;
    QString licence() const;

    Q_INVOKABLE QString getDescription();
    Q_INVOKABLE QString getGithubLink();
    Q_INVOKABLE QString getInstallLink();
    Q_INVOKABLE QString getDownloadLink(QString architecture);

public slots:
    void setId(QString id);
    void setLinks(QVariantHash links);
    void setAuthors(QVariantList authors);

    void setDescription(QString description);
    void setCategories(QStringList categories);
    void setScreenshots(QStringList screenshots);
    void setLicence(QString licence);

signals:
    void idChanged(QString id);
    void linksChanged(QVariantHash links);
    void authorsChanged(QVariantList authors);
    void descriptionChanged(QString description);
    void categoriesChanged(QStringList categories);
    void screenshotsChanged(QStringList screenshots);

    void licenceChanged(QString licence);

protected:
    QString m_id;
    QVariantHash m_links;
    QVariantList m_authors;
    QString m_description;
    QString m_licence;
    QStringList m_categories;
    QStringList m_screenshots;
};

#endif // APPIMAGE_H
