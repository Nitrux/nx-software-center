#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QDebug>
#include <QLocalSocket>
#include <QJsonDocument>
#include <QJsonObject>

#include "snapsindex.h"


QString createRawHTTPRequest(QString method, QString path) {
    QString query;
    query += "" + method + " " + path + " HTTP/1.1\r\n";
    query +=
            "Host: \r\n"
            "User-Agent: User-Agent: nx-software-center\r\n"
            "Content-Type: application/json; \r\n"
            "\r\n";

    return query;
}

QJsonDocument parseRawHTTPResponse(QByteArray response) {
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

QByteArray query() {
    QLocalSocket *socket = new QLocalSocket();

    socket->connectToServer("/run/snapd.socket");
    QByteArray response ;
    for (;;) { // try catch at dirty style
        if (socket->waitForConnected(1000))
            qDebug("Connected to socket!");
        else
            break;


        qDebug() << "Writing !";
        socket->write(createRawHTTPRequest("GET", "/v2/icons/hello-world/icon").toLocal8Bit());
        if (socket->waitForBytesWritten(1000))
            qDebug("Wrote!");
        else
            break;


        if (socket->waitForReadyRead(1000))
            qDebug("Response ready!");
        else
            break;

        response = socket->readAll();
        break;
    }
    socket->close();
    delete socket;

    return response;
}

int main(int argc, char *argv[])
{




//    qDebug() << parseRawHTTPResponse(query()).object().value("result");
//    return 0;


        QGuiApplication app(argc, argv);

        QCoreApplication::addLibraryPath("./");

        QQmlApplicationEngine engine;

        qmlRegisterType<SnapsIndex>("org.nx", 1, 0, "Local");
        // qmlRegisterType<RadioPlayer>(uri, 1, 0, "Store");

        engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

        return app.exec();

}


