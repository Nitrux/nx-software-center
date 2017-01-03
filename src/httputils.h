#ifndef HTTPUTILS_H
#define HTTPUTILS_H

#include <QString>
#include <QJsonDocument>

class HTTPUtils
{

public:
    explicit HTTPUtils();

    QString buildSimpleJSonGetRequest(QString path);

    QJsonDocument parseJSonResponse(QByteArray response);

private:
    QString userAgent;
};

#endif // HTTPUTILS_H
