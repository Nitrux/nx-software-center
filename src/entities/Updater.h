//
// Created by alexis on 23/01/18.
//

#ifndef NOMAD_SOFTWARE_CENTER_UPDATER_H
#define NOMAD_SOFTWARE_CENTER_UPDATER_H

#include <QObject>
#include <QDateTime>

#include "Source.h"
#include "Repository.h"

class FetchApplicationsInteractor;
class Updater : public QObject {
Q_OBJECT
    Repository *repository;
    QList<Source *> sources;

    QDateTime lastUpdateDate;
    bool isWorking;
    bool isReady;
    QStringList sourceErrors;

    FetchApplicationsInteractor *fetchApplicationsInteractor;
    Q_PROPERTY(bool isWorking MEMBER isWorking NOTIFY isWorkingChanged)
    Q_PROPERTY(bool isReady MEMBER isReady NOTIFY isReadyChanged)
public:
    Updater(Repository *repository, const QList<Source *> &sources, QObject *parent = nullptr);

    bool isUpdateRequired();

    QStringList getErrors();

    bool getIsReady();
    bool getIsWorking();
signals:

    void isWorkingChanged(bool isWorking);

    void isReadyChanged(bool isWorking);

public slots:
    void update();

protected slots:
    void handleFetchApplicationsCompleted();

private:
    void setIsWorking();

    void unsetIsWorking();

    void setIsReady();

    void unsetIsReady();
};


#endif //NOMAD_SOFTWARE_CENTER_UPDATER_H
