#pragma once
// system

// libraries

// local
#include "../../utils/nx.h"
#include "../services/ApplicationData.h"
#include "DBHelper.h"

class BundlesDBHelper : public DBHelper {
    Q_OBJECT

    public:
        BundlesDBHelper();

        QList<ApplicationBundle> getBundlesByApp(QString id);
        bool saveOrUpdateBundles(const QString id, const QList<ApplicationBundle> &bundles);
        bool deleteBundles(const QString id, const QList<ApplicationBundle> &bundles);

    private:
        const QString DATE_FORMAT = "yyyy-MM-dd HH:mm:ss.zzz";
        
        const QString QUERY_INSERT_APPLICATION_BUNDLE = "INSERT INTO APPLICATION_BUNDLE (APPLICATION_ID, PATH, SIZE, LAST_MODOFIED, HASH_SUM_MD5, BUNDLE_TYPE) values (:application_id, :path, :size, :last_modified, :hash_sum_md5, :bundle_type) ";
        const QString QUERY_UPDATE_APPLICATION_BUNDLE = "UPDATE APPLICATION_BUNDLE SET SIZE=:size, LAST_MODOFIED=:last_modified, HASH_SUM_MD5=:hash_sum_md5, BUNDLE_TYPE=:bundle_type WHERE APPLICATION_ID=:application_id and PATH=:path ";
        const QString QUERY_DELETE_APPLICATION_BUNDLE = "DELETE FROM APPLICATION_BUNDLE where APPLICATION_ID=? and PATH=?";

        const QString QUERY_SELECT_APPLICATION_BUNDLES_BY_ID = "SELECT * FROM APPLICATION_BUNDLE where APPLICATION_ID=(:application_id)";
};