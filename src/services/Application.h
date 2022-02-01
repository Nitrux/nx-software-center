#pragma once
// system

// libraries
#include <QMap>
#include <QObject>
#include <QUrl>
#include <QVariant>

// local
#include "ApplicationBundle.h"

/**
 * @brief Represents an application from the software center perspective.
 *
 * An application may contain several bundles. One of them (usually the latest) will be used as source of the application data.
 */
class Application
{
public:
    Application();
    explicit Application(ApplicationData data);
    explicit Application(const ApplicationBundle &bundle);

    QString getId() const;
    const ApplicationData &getData() const;

    [[nodiscard]] QList<ApplicationBundle> getBundles() const;
    void setBundles(QList<ApplicationBundle> bundles);

    // add the bundle to the list and updates the current data if the bundle as a higher version
    void addBundle(const ApplicationBundle &bundle);

    // removes a bundle from the list and updates the current data if the bundle is the one with the higher version
    void removeBundle(const ApplicationBundle &bundle);

    // define the main bundle for this application
    [[nodiscard]] ApplicationBundle getMainBundle() const;
    void setMainBundle(const ApplicationBundle &bundle);

    [[nodiscard]] int getMainBundleIndex() const;
    void setMainBundleIndex(int i);

    bool operator==(const Application &rhs) const;
    bool operator!=(const Application &rhs) const;



private:
    // find the insert index for a new bundle using the version for comparing
    int findInsertIndex(const ApplicationBundle &bundle);

    QString _id;
    ApplicationData _data;
    ApplicationBundle _mainBundle;
    QList<ApplicationBundle> _bundles;
};

typedef QVector<Application> ApplicationsList;

Q_DECLARE_METATYPE(Application)
Q_DECLARE_METATYPE(ApplicationsList)