#pragma once
// system

// libraries
#include <QMetaType>
#include <QVariantMap>

// local

class ApplicationBundle;

/**
 * Contains the information about an Application
 */
class ApplicationData
{
public:
    ApplicationData();
    explicit ApplicationData(QVariantMap data);

    [[nodiscard]] QString getId() const;
    void setId(const QString &id);

    [[nodiscard]] QString getVersion() const;
    void setVersion(const QString &version);

    [[nodiscard]] QString getName() const;
    void setName(const QString &name);

    [[nodiscard]] QString getIcon() const;
    void setIcon(const QString &icon);

    [[nodiscard]] QString getDescription() const;
    void setDescription(const QString &description);

    [[nodiscard]] QList<QUrl> getSnapshots() const;
    void setSnapshots(const QList<QUrl> &snapshots);

    [[nodiscard]] QStringList getXdgCategories() const;
    void setXdgCategories(const QStringList &xdgCategories);

    [[nodiscard]] bool requiresTerminal() const;
    void setRequiresTerminal(bool requiresTerminal);

    void setEntry(const QString &id, const QVariant &value);
    [[nodiscard]] QVariant getEntry(const QString &id) const;
    [[nodiscard]] QStringList entries() const;

    // copies application data without removing unmodified fields
    void copy(const ApplicationData &data);

    QByteArray toJson() const;

    static ApplicationData fromJson(const QByteArray &jsonData);

private:
    QVariantMap _data;
};

Q_DECLARE_METATYPE(ApplicationData)