//
// Created by alexis on 7/10/18.
//

#ifndef NX_SOFTWARE_CENTER_APPLICATIONSEARCH_H
#define NX_SOFTWARE_CENTER_APPLICATIONSEARCH_H

#include <QObject>
#include <entities/ApplicationAbstract.h>

class ApplicationsRepositorySearch : public QObject {
Q_OBJECT

protected:
    QString query;
    QString category;
    int offset;
    int limit;
    QList<ApplicationAbstract> results;
    bool running;
public:
    ApplicationsRepositorySearch(QObject *parent = nullptr);
    virtual ~ApplicationsRepositorySearch();

    bool isRunning() const;

    void setQuery(const QString &query);

    void setCategory(const QString &category);

    void setOffset(const int &offset);

    void setLimit(const int &limit);

    virtual void start() = 0;

    virtual void stop() = 0;

    QList<ApplicationAbstract> getResults() const;

signals:

    void completed();

    void failed(const QString &reason);

};


#endif //NX_SOFTWARE_CENTER_APPLICATIONSEARCH_H
