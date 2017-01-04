#include "httputils.h"

#include <QCoreApplication>
#include <QDebug>

HTTPUtils::HTTPUtils()
{
}

QString HTTPUtils::buildSimpleJSonGetRequest(QString path)
{
    QString userAgent = qApp->applicationName() + "\\" +qApp->applicationVersion();
    QString query;
    query += "GET " + path + " HTTP/1.1\r\n";
    query += "Host: 127.0.0.1\r\n";
    query += QString("User-Agent: %1 \r\n").arg(userAgent);
    query += "Content-Type: application/json; \r\n"
            "\r\n";

    return query;
}

QString HTTPUtils::buildJSonPostRequest(QString path, QJsonDocument data)
{
    QStringList query;
    query << QString("POST %1 HTTP/1.1").arg(path);
    query << "Host: 127.0.0.1";
    query << userAgentHeader();
    query << "Content-Type: application/json;";

    QByteArray content = data.toJson();
    query <<  QString("Content-Length: %1").arg(content.length());
    query << "";
    query << content;
    query << "";

    return query.join("\r\n");
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

QString HTTPUtils::userAgentHeader()
{
    return QString("User-Agent: %1").arg(qApp->applicationName());
}
