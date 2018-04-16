//
// Created by alexis on 8/01/18.
//

#ifndef NOMAD_SOFTWARE_CENTER_EXECUTOR_H
#define NOMAD_SOFTWARE_CENTER_EXECUTOR_H

#include <QMap>
#include <QObject>
#include <QRunnable>
#include <QString>
#include <QStringList>
#include <QMutex>
#include <QThread>

class Interactor;

class Executor : public QObject {
Q_OBJECT
    QMutex lock;

    QThread workerThread;
    QMap<QString, Interactor *> interactors;
    QMap<QString, QVariantMap> tasksData;
public:
    Executor(QObject *parent = 0) : QObject(parent) {}

    virtual ~Executor();

    void execute(Interactor *interactor);

    QVariantMap getTaskData(const QString &id);

    QStringList getRunningTasks();

    void cancel(const QString &id);

signals:

    void taskStarted(const QString &id, const QVariantMap &data);

    void taskCompleted(const QString &id, const QVariantMap &data);

    void taskDataChanged(const QString &id, const QVariantMap &data);

protected slots:

    void handleInteractorComplete();

    void handleInteractorMetadataChanged(const QVariantMap &data);

    void wrapInteractor(Interactor *interactor);
};


#endif //NOMAD_SOFTWARE_CENTER_EXECUTOR_H
