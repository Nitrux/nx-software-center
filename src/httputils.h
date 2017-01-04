#ifndef HTTPUTILS_H
#define HTTPUTILS_H

#include <QString>
#include <QJsonDocument>

class HTTPUtils
{

public:
    explicit HTTPUtils();

    static QString buildSimpleJSonGetRequest(QString path);
    static QString buildJSonPostRequest(QString path, QJsonDocument data);

    static QJsonDocument parseJSonResponse(QByteArray response);

private:
    static QString userAgentHeader();
};

#endif // HTTPUTILS_H
