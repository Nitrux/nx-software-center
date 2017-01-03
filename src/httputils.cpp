#include "httputils.h"

#include <QCoreApplication>
#include <QDebug>

HTTPUtils::HTTPUtils()
{
    userAgent = qApp->applicationName() + "\\" +qApp->applicationVersion();
}

QString HTTPUtils::buildSimpleJSonGetRequest(QString path)
{
    QString query;
    query += "GET " + path + " HTTP/1.1\r\n";
    query += "Host: \r\n";
    query += "User-Agent: User-Agent: " + userAgent + "\r\n";
    query += "Content-Type: application/json; \r\n"
            "\r\n";

    return query;
}

QJsonDocument HTTPUtils::parseJSonResponse(QByteArray response)
{
    QString str(response);
    QStringList lines = str.split("\r\n");
    QString result;
    for (QString line : lines) {
        if (line.startsWith("{"))
            result = line;
        qDebug() << line;
    }


    return QJsonDocument::fromJson(result.toLocal8Bit());
}
