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

        openDB(QUuid::createUuid().toString(), appDBPath);
        prepareAppsDB();
    } else {
        qDebug() << "Apps DB already available. Establishing connection.";

        openDB(QUuid::createUuid().toString(), appDBPath);
    }
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
    
    if ( !createTableQuery.exec(QUERY_CREATE_APPLICATION_DATA) ) {
        qCritical("%s", createTableQuery.lastError().text().toLocal8Bit().data());

        return ;
    }

    if ( !createTableQuery.exec(QUERY_CREATE_APPLICATION_BUNDLE) ) {
        qCritical("%s", createTableQuery.lastError().text().toLocal8Bit().data());

        return ;
    }
}

DBHelper::~DBHelper() {
    if (this->_appsDB.isOpen()) {
        this->_appsDB.close();
    }
}