#include "BundlesDBHelper.h"

BundlesDBHelper::BundlesDBHelper()
{

}

QList<ApplicationBundle> BundlesDBHelper::getBundlesByApp(QString id)
{
    QList<ApplicationBundle> bundles;

    QSqlQuery query(this->_appsDB);
    query.prepare(QUERY_SELECT_APPLICATION_BUNDLES_BY_ID);
    query.bindValue(":application_id", id);

    if ( query.exec() ) {
        int bundlePath = query.record().indexOf("PATH");
        int bundleSize = query.record().indexOf("SIZE");
        int bundleLastModified = query.record().indexOf("LAST_MODOFIED");
        int bundleHashSumMd5 = query.record().indexOf("HASH_SUM_MD5");
        int bundleType = query.record().indexOf("BUNDLE_TYPE");
        
        while (query.next()) {
            ApplicationBundle bundle;
            bundle.path             = query.value(bundlePath).toString();
            bundle.size             = query.value(bundleSize).toInt();
            bundle.lastModified     = QDateTime::fromString(query.value(bundleLastModified).toString(), BundlesDBHelper::DATE_FORMAT);
            bundle.hashSumMD5       = query.value(bundleHashSumMd5).toByteArray();
            
            int _bundleType = query.value(bundleType).toInt();
            if ( _bundleType == ApplicationBundle::AppImage ) {
                bundle.bundleType   = ApplicationBundle::AppImage;
            } else {
                bundle.bundleType   = ApplicationBundle::Unknown;
            }

            bundles << bundle;
        }
    } else {
        qCritical() << "Error executing select application bundles query";
    }

    return bundles;
}

bool BundlesDBHelper::saveOrUpdateBundles(const QString id, const QList<ApplicationBundle> &bundles)
{
    QList<ApplicationBundle> _bundles = this->getBundlesByApp(id);

    foreach (ApplicationBundle _bundle, bundles) {
        // Check if app already exists.
        QSqlQuery query(this->_appsDB);

        if ( _bundles.contains(_bundle) ) {
            // Bundle already exists in DB. Update bundle
            query.prepare(QUERY_UPDATE_APPLICATION_BUNDLE);
            query.bindValue(":size", _bundle.size);
            query.bindValue(":last_modified", _bundle.lastModified.toString(BundlesDBHelper::DATE_FORMAT));
            query.bindValue(":hash_sum_md5", QString(_bundle.hashSumMD5));
            query.bindValue(":bundle_type", _bundle.bundleType);
            query.bindValue(":application_id", id);
            query.bindValue(":path", _bundle.path);

            _bundles.removeOne(_bundle);
        } else {
            // Bundle does not exists in DB. Insert bundle
            query.prepare(QUERY_INSERT_APPLICATION_BUNDLE);
            query.bindValue(":application_id", id);
            query.bindValue(":path", _bundle.path);
            query.bindValue(":size", _bundle.size);
            query.bindValue(":last_modified", _bundle.lastModified.toString(BundlesDBHelper::DATE_FORMAT));
            query.bindValue(":hash_sum_md5", QString(_bundle.hashSumMD5));
            query.bindValue(":bundle_type", _bundle.bundleType);
        }

        if (!query.exec()) {
            qCritical() << "Error saving bundle to DB. " << query.lastError();
        }
	}

    /*  The entries left in _bundles are the entries not available originally.
            Remove these entries from the APPLICATION_BUNDLE table */
    this->deleteBundles(id, _bundles);

    return true;
}

bool BundlesDBHelper::deleteBundles(const QString id, const QList<ApplicationBundle> &bundles)
{
    bool success = false;

    QVariantList appIds;
    QVariantList bundlePaths;
    foreach (ApplicationBundle _bundle, bundles) {
        appIds << id;
        bundlePaths << _bundle.path;
	}

    QSqlQuery query(this->_appsDB);
    query.prepare(QUERY_DELETE_APPLICATION_BUNDLE);
    query.addBindValue(appIds);
    query.addBindValue(bundlePaths);

    if (query.execBatch()) {
        success = true;
    } else {
        qCritical() << "Error deleting bundles from DB. " << query.lastError();
    }

    return success;
}