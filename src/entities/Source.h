//
// Created by alexis on 22/12/17.
//

#ifndef NOMAD_SOFTWARE_CENTER_SOURCE_H
#define NOMAD_SOFTWARE_CENTER_SOURCE_H

#include <QList>
#include <QObject>

class Application;

class Source : public QObject {
Q_OBJECT
public:
    explicit Source(QObject *parent = 0);

signals:

    void fetchedAllApplications(QList<Application> applications);

    void fetchError(const QString &details);

public slots:

    virtual void fetchAllApplications() = 0;

};


#endif //NOMAD_SOFTWARE_CENTER_SOURCE_H
