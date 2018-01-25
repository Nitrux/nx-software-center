//
// Created by alexis on 25/01/18.
//

#ifndef NOMAD_SOFTWARE_CENTER_APPLICATIONSERIALIZER_H
#define NOMAD_SOFTWARE_CENTER_APPLICATIONSERIALIZER_H

#include <QtCore/QJsonArray>

class Application;
class ApplicationSerializer {

public:
    static QByteArray serialize(Application application);

    static const constexpr char *const APP_ID = "id";

    static const constexpr char *const APP_VERSION = "version";

    static const constexpr char *const APP_CODE_NAME = "codeName";

    static const constexpr char *const APP_NAME = "name";

    static const constexpr char *const APP_DESCRIPTION = "description";

    static const constexpr char *const APP_ARCH = "arch";

    static const constexpr char *const APP_DOWNLOAD_URL = "downloadUrl";

    static const constexpr char *const APP_LICENCE = "licence";

    static const constexpr char *const APP_HOME_PAGE = "homePageUrl";

    static const constexpr char *const APP_AUTHORS = "authors";

    static const constexpr char *const APP_CATEGORIES = "categories";

    static const constexpr char *const APP_SCREENSHOTS = "screenshots";

    static Application deserialize(QByteArray json);

    static QStringList getStringListFromJsonArray(const QJsonArray &array);
};


#endif //NOMAD_SOFTWARE_CENTER_APPLICATIONSERIALIZER_H
