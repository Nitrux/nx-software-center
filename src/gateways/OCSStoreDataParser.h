//
// Created by alexis on 3/6/18.
//

#ifndef NOMAD_SOFTWARE_CENTER_OCSSTOREPARSER_H
#define NOMAD_SOFTWARE_CENTER_OCSSTOREPARSER_H

#include <QObject>

#include <entities/Application.h>
#include <QtXml/QDomNode>
#include <QtXmlPatterns/QXmlQuery>

#include <QtCore/QUrl>

class OCSStoreDataParser : public QObject {
Q_OBJECT
    QUrl target;
    int itemsPerPage;
    int totalItems;
    QList<Application> results;
    bool failed;
    QXmlQuery query;
public:
    OCSStoreDataParser(QObject *parent = nullptr);

    const QList<Application> &getResults() const;

    int getItemsPerPage() const;

    int getTotalItems() const;

    const QUrl &getTarget() const;

    void setTarget(const QUrl &target);

    bool isFailed() const;

signals:

    void completed();

    void error(const QString &msg);

public slots:
    void extractApplications();

private:
    int countContentItems();

    void parseContentTag(int i);

    QString getContentStringField(int idx, QString field);

    QString evaluateQuery(QString query);


    int getAppimageDownloadIdx(int contentIdx);

    QStringList getScreenShots(int contentIdx);
};


#endif //NOMAD_SOFTWARE_CENTER_OCSSTOREPARSER_H
