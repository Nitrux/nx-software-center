#ifndef APPIMAGE_H
#define APPIMAGE_H

#include <QObject>
#include <QVariantHash>

class AppImage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QVariantHash data READ data WRITE setData NOTIFY dataChanged)
    Q_PROPERTY(QVariantHash links READ links WRITE setLinks NOTIFY linksChanged)
    Q_PROPERTY(QVariantList authors READ authors WRITE setAuthors NOTIFY authorsChanged)

public:
    AppImage(QObject *parent = nullptr);

    Q_INVOKABLE QString getName();
    Q_INVOKABLE QString getDescription();

    Q_INVOKABLE QString getGithubLink();
    Q_INVOKABLE QString getInstallLink();
    Q_INVOKABLE QString getDownloadLink(QString architecture);

    QString id() const;
    QVariantHash data() const;
    QVariantHash links() const;
    QVariantList authors() const;

public slots:
    void setId(QString id);
    void setData(QVariantHash data);
    void setLinks(QVariantHash links);
    void setAuthors(QVariantList authors);

signals:
    void idChanged(QString id);
    void dataChanged(QVariantHash data);
    void linksChanged(QVariantHash links);
    void authorsChanged(QVariantList authors);

protected:
    QString m_id;
    QVariantHash m_data;
    QVariantHash m_links;
    QVariantList m_authors;
};

#endif // APPIMAGE_H
