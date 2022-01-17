#include "AppsDBHelper.h"

AppsDBHelper::AppsDBHelper()
    : _bundlesDBHelper(new BundlesDBHelper())
{
}

QList<ApplicationData> AppsDBHelper::getApps() {
    QList<ApplicationData> apps;

    QSqlQuery query(QUERY_SELECT_APPLICATION_DATA, this->_appsDB);

    int appId = query.record().indexOf("ID");
    int appVersion = query.record().indexOf("VERSION");
    int appName = query.record().indexOf("NAME");
    int appIcon = query.record().indexOf("ICON");
    int appDescription = query.record().indexOf("DESCRIPTION");
    int appSnapshots = query.record().indexOf("SNAPSHOTS");
    int appCategories = query.record().indexOf("CATEGORIES");

    while (query.next()) {
        ApplicationData app;
        app.setId(query.value(appId).toString());
        app.setVersion(query.value(appVersion).toString());
        app.setName(query.value(appName).toString());
        app.setIcon(query.value(appIcon).toString());
        app.setDescription(query.value(appDescription).toString());

        QJsonDocument snapshotsDoc = QJsonDocument::fromJson(query.value(appSnapshots).toByteArray());
        QJsonArray snapshotsJson = snapshotsDoc.array();
        QJsonDocument categoriesDoc = QJsonDocument::fromJson(query.value(appCategories).toByteArray());
        QJsonArray categoriesJson = categoriesDoc.array();

        app.setSnapshots(this->snapshotsFromJsonArray(snapshotsJson));
        app.setXdgCategories(this->categoriesFromJsonArray(categoriesJson));
        
        app.setBundles(_bundlesDBHelper->getBundlesByApp(app.getId()));

        apps << app;
    }

    return apps;
}

QMap<QString, ApplicationData> AppsDBHelper::getAppsMap()
{
    QList<ApplicationData> _apps = this->getApps();
    QMap<QString, ApplicationData> _applications;

    foreach (ApplicationData app, _apps) {
        QString appId = app.getId();
        _applications.insert(appId, app);
	}

    return _applications;
} 

ApplicationData *AppsDBHelper::getAppById(QString appId) {
    ApplicationData *app;

    QSqlQuery query(this->_appsDB);
    query.prepare(QUERY_SELECT_APPLICATION_DATA_BY_ID);
    query.bindValue(":appId", appId);

    if ( query.exec() ) {
        int appId = query.record().indexOf("ID");
        int appVersion = query.record().indexOf("VERSION");
        int appName = query.record().indexOf("NAME");
        int appIcon = query.record().indexOf("ICON");
        int appDescription = query.record().indexOf("DESCRIPTION");
        int appSnapshots = query.record().indexOf("SNAPSHOTS");
        int appCategories = query.record().indexOf("CATEGORIES");
        
        if ( query.next() ) {
            app = new ApplicationData();
            app->setId(query.value(appId).toString());
            app->setVersion(query.value(appVersion).toString());
            app->setName(query.value(appName).toString());
            app->setIcon(query.value(appIcon).toString());
            app->setDescription(query.value(appDescription).toString());

            QJsonDocument snapshotsDoc = QJsonDocument::fromJson(query.value(appSnapshots).toByteArray());
            QJsonArray snapshotsJson = snapshotsDoc.array();
            QJsonDocument categoriesDoc = QJsonDocument::fromJson(query.value(appCategories).toByteArray());
            QJsonArray categoriesJson = categoriesDoc.array();

            app->setSnapshots(this->snapshotsFromJsonArray(snapshotsJson));
            app->setXdgCategories(this->categoriesFromJsonArray(categoriesJson));

            app->setBundles(_bundlesDBHelper->getBundlesByApp(app->getId()));

            return app;
        }
    }

    return nullptr;
}

ApplicationData *AppsDBHelper::getAppByName(QString appName) {
    ApplicationData *app;

    QSqlQuery query(this->_appsDB);
    query.prepare(QUERY_SELECT_APPLICATION_DATA_BY_NAME);
    query.bindValue(":appName", appName);

    if ( query.exec() ) {
        int appId = query.record().indexOf("ID");
        int appVersion = query.record().indexOf("VERSION");
        int appName = query.record().indexOf("NAME");
        int appIcon = query.record().indexOf("ICON");
        int appDescription = query.record().indexOf("DESCRIPTION");
        int appSnapshots = query.record().indexOf("SNAPSHOTS");
        int appCategories = query.record().indexOf("CATEGORIES");
        
        if ( query.next() ) {
            app = new ApplicationData();
            app = new ApplicationData();
            app->setId(query.value(appId).toString());
            app->setVersion(query.value(appVersion).toString());
            app->setName(query.value(appName).toString());
            app->setIcon(query.value(appIcon).toString());
            app->setDescription(query.value(appDescription).toString());

            QJsonDocument snapshotsDoc = QJsonDocument::fromJson(query.value(appSnapshots).toByteArray());
            QJsonArray snapshotsJson = snapshotsDoc.array();
            QJsonDocument categoriesDoc = QJsonDocument::fromJson(query.value(appCategories).toByteArray());
            QJsonArray categoriesJson = snapshotsDoc.array();

            app->setSnapshots(this->snapshotsFromJsonArray(snapshotsJson));
            app->setXdgCategories(this->categoriesFromJsonArray(categoriesJson));

            app->setBundles(_bundlesDBHelper->getBundlesByApp(app->getId()));

            return app;
        }
    }

    return nullptr;
}

bool AppsDBHelper::saveOrUpdateApp(const ApplicationData &app)
{
    bool success = false;

    QJsonArray snapshotsJsonArray = QJsonArray::fromStringList(this->toStringList(app.getSnapshots()));
    QJsonDocument snapshotsDoc;
    snapshotsDoc.setArray(snapshotsJsonArray);
    QString snapshots(snapshotsDoc.toJson(QJsonDocument::Compact));

    QJsonArray categoriesJsonArray = QJsonArray::fromStringList(app.getXdgCategories());
    QJsonDocument categoriesDoc;
    categoriesDoc.setArray(categoriesJsonArray);
    QString categories(categoriesDoc.toJson(QJsonDocument::Compact));

    // Check if app already exists.
    QSqlQuery query(this->_appsDB);

    if (this->getAppById(app.getId()) == nullptr) {
        query.prepare(QUERY_INSERT_APPLICATION_DATA);
        query.bindValue(":id", app.getId());
        query.bindValue(":version", app.getVersion());
        query.bindValue(":name", app.getName());
        query.bindValue(":icon", app.getIcon());
        query.bindValue(":description", app.getDescription());
        query.bindValue(":snapshots", snapshots);
        query.bindValue(":categories", categories);
    } else {
        query.prepare(QUERY_UPDATE_APPLICATION_DATA);
        query.bindValue(":version", app.getVersion());
        query.bindValue(":name", app.getName());
        query.bindValue(":icon", app.getIcon());
        query.bindValue(":description", app.getDescription());
        query.bindValue(":snapshots", snapshots);
        query.bindValue(":categories", categories);
        query.bindValue(":id", app.getId());
    }

    // Save the bundles
    _bundlesDBHelper->saveOrUpdateBundles(app.getId(), app.getBundles());

    if (query.exec()) {
        success = true;
    } else {
        qCritical() << "Error saving app to DB. " << query.lastError();
    }

    return success;
}

bool AppsDBHelper::deleteApp(const ApplicationData &app)
{
    bool success = false;

    QSqlQuery query(this->_appsDB);
    query.prepare(QUERY_DELETE_APPLICATION_DATA);
    query.bindValue(":appId", app.getId());

    if (query.exec()) {
        success = true;
    } else {
        qCritical() << "Error deleting app from DB. " << query.lastError();
    }

    return success;
}

QStringList AppsDBHelper::toStringList(QList<QUrl> list) {
    QStringList result;

    for (int i = 0; i < list.size(); i++) {
        result << list.value(i).toString();
    }

    return result;
}

QList<QUrl> AppsDBHelper::snapshotsFromJsonArray(QJsonArray data) {
    QList<QUrl> result;

    for (int i = 0; i < data.size(); i++) {
        result << QUrl::fromUserInput(data.at(i).toString());
    }

    return result;
}

QStringList AppsDBHelper::categoriesFromJsonArray(QJsonArray data) {
    QStringList result;

    for (int i = 0; i < data.size(); i++) {
        result << data.at(i).toString();
    }

    return result;
}