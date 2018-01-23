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

class Interactor;

class Executor : public QObject {
Q_OBJECT
    QMutex lock;

    class InteractorRunnableWrapper;
    QMap<QString, Interactor *> interactors;
    QMap<QString, InteractorRunnableWrapper*> runnables;

public:
    Executor(QObject *parent = 0) : QObject(parent) {}

    void execute(Interactor *interactor);

    QVariantMap getTaskData(const QString &id);

    QStringList getRunningTasks();

    void cancel(const QString &id);

signals:

    void taskStarted(const QString &id);

    void taskCompleted(const QString &id, const QVariantMap &data);

    void taskDataChanged(const QString &id, const QVariantMap &data);

protected slots:

    void handleInteractorComplete();

    void handleInteractorMetadataChanged(const QVariantMap &data);
};


#endif //NOMAD_SOFTWARE_CENTER_EXECUTOR_H
