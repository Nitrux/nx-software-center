#include "DBHelper.h"

DBHelper::DBHelper()
{
    initDB();
}

void DBHelper::initDB() {
    QString appDataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString appDBName = NX::appDBName;
    QString appDBPath = appDataLocation + appDBName;

    if (!FMH::fileExists(QUrl::fromUserInput(appDBPath))) {
        qWarning() << "Apps DB not available. Trying to create db ";

        QDir appDataLocation_dir(appDataLocation);
        if (!appDataLocation_dir.exists())
            appDataLocation_dir.mkpath(".");
    } else {
        qDebug() << "Apps DB already available. Establishing connection.";
    }

    openDB(QUuid::createUuid().toString(), appDBPath);
    prepareAppsDB();
}

void DBHelper::openDB(const QString &name, const QString &appDBPath)
{
    if (!QSqlDatabase::contains(name)) {
        this->_appsDB = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), name);
        this->_appsDB.setDatabaseName(appDBPath);
    }

    if (!this->_appsDB.isOpen()) {
        if (!this->_appsDB.open()) {
            qCritical() << "ERROR OPENING DB" << this->_appsDB.lastError().text() << _appsDB.connectionName();
        } else {
            qDebug() << "DB connection established.";
        }
    }
}

void DBHelper::prepareAppsDB() {
    qDebug() << "Preparing DB";

    QSqlQuery createTableQuery(this->_appsDB);
    
    // Check if table APPLICATION_DATA already exists
    if ( !this->_appsDB.tables().contains(TABLE_NAME_APPLICATION_DATA) ) {
        qDebug() << "Table APPLICATION_DATA does not exists. Creating table...";

        // Create table APPLICATION_DATA
        if ( !createTableQuery.exec(QUERY_CREATE_APPLICATION_DATA) ) {
            qCritical("%s", createTableQuery.lastError().text().toLocal8Bit().data());

            return ;
        }
    }

    // Check if table APPLICATION_BUNDLE already exists
    if ( !this->_appsDB.tables().contains(TABLE_NAME_APPLICATION_BUNDLE) ) {
        qDebug() << "Table APPLICATION_BUNDLE does not exists. Creating table...";

        // Create table APPLICATION_BUNDLE
        if ( !createTableQuery.exec(QUERY_CREATE_APPLICATION_BUNDLE) ) {
            qCritical("%s", createTableQuery.lastError().text().toLocal8Bit().data());

            return ;
        }
    }
}

DBHelper::~DBHelper() {
    if (this->_appsDB.isOpen()) {
        this->_appsDB.close();
    }
}