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

class Interactor;

class Executor : public QObject {
Q_OBJECT
public:
    Executor(QObject *parent = 0) : QObject(parent) {}

    void execute(Interactor *i);

    QVariantMap getTaskData(const QString &id);

    QStringList getRunningTasks();

signals:

    void taskStarted(const QString &id);

    void taskCompleted(const QString &id);

    void taskDataChanged(const QString &id, const QVariantMap &data);

protected slots:

    void handleInteractorComplete();
    void handleInteractorMetadataChanged(const QVariantMap &data);

private:
    class InteractorRunnableWrapper : public QRunnable {
        Interactor *i;
    public:
        InteractorRunnableWrapper(Interactor *i);

        void run() override;
        Interactor* getIntereactor() { return i; }
    };

    QMap<QString, InteractorRunnableWrapper *> tasks;
};


#endif //NOMAD_SOFTWARE_CENTER_EXECUTOR_H
