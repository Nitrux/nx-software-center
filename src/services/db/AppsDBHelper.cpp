#include "AppsDBHelper.h"

AppsDBHelper::AppsDBHelper() {
    initDB();
}

void AppsDBHelper::initDB() {
    QString appDataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString appDBName = NX::appDBName;
    QString appDBPath = appDataLocation + appDBName;

    if (!FMH::fileExists(QUrl::fromUserInput(appDBPath))) {
        qDebug() << "Apps DB not available. Trying to create db ";

        QDir appDataLocation_dir(appDataLocation);
        if (!appDataLocation_dir.exists())
            appDataLocation_dir.mkpath(".");

        openDB(QUuid::createUuid().toString(), appDBPath);
        prepareAppsDB();
    } else {
        qDebug() << "Apps DB already available. Establishing connection.";

        openDB(QUuid::createUuid().toString(), appDBPath);
    }
}

void AppsDBHelper::openDB(const QString &name, const QString &appDBPath)
{
    if (!QSqlDatabase::contains(name)) {
        this->_appsDB = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), name);
        this->_appsDB.setDatabaseName(appDBPath);
    }

    if (!this->_appsDB.isOpen()) {
        if (!this->_appsDB.open()) {
            qDebug() << "ERROR OPENING DB" << this->_appsDB.lastError().text() << _appsDB.connectionName();
        } else {
            qDebug() << "DB connection established.";
        }
    }
}

void AppsDBHelper::prepareAppsDB() {
    qDebug() << "Preparing DB";

    QSqlQuery createTableQuery(this->_appsDB);
    createTableQuery.prepare(QUERY_CREATE_APPLICATION_DATA);
    
    if ( !createTableQuery.exec() ) {
        qWarning("%s", createTableQuery.lastError().text().toLocal8Bit().data());

        return ;
    }
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

    if (query.exec()) {
        success = true;
    } else {
        qDebug() << "Error saving app to DB. " << query.lastError();
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
        qDebug() << "Error deleting app from DB. " << query.lastError();
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

AppsDBHelper::~AppsDBHelper() {
    if (this->_appsDB.isOpen()) {
        this->_appsDB.close();
    }
}