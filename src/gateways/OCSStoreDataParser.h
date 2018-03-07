//
// Created by alexis on 3/6/18.
//

#ifndef NOMAD_SOFTWARE_CENTER_OCSSTOREPARSER_H
#define NOMAD_SOFTWARE_CENTER_OCSSTOREPARSER_H

#include <QObject>

#include <entities/Application.h>
#include <QtXml/QDomNode>
#include <QtXmlPatterns/QXmlItem>
#include <QtXmlPatterns/QXmlResultItems>
#include <QtCore/QUrl>

class OCSStoreDataParser : public QObject {
Q_OBJECT
    QUrl target;
    int itemsPerPage;
    int totalItems;
    QList<Application> results;
public:
    OCSStoreDataParser(const QUrl &target, QObject *parent = nullptr);

    void extractApplications();

    const QList<Application> &getResults() const;

    int getItemsPerPage() const;

    int getTotalItems() const;

signals:

    void completed();

    void error(const QString &msg);

private:
    int countContentItems();

    void parseContentTag(int i);

    QString getContentStringField(int idx, QString field);

    QString evaluateQuery(QString query);


    int getAppimageDownloadIdx(int contentIdx) const;

    QStringList getScreenShots(int contentIdx) const;
};


#endif //NOMAD_SOFTWARE_CENTER_OCSSTOREPARSER_H
