//
// Created by alexis on 8/01/18.
//

#include <QThreadPool>
#include <QMutexLocker>
#include <QDebug>
#include <QCoreApplication>

#include "Executor.h"
#include "interactors/Interactor.h"


void Executor::execute(Interactor *interactor) {
    QMutexLocker locker(&lock);
    wrapInteractor(interactor);

    workerThread.start();
    QMetaObject::invokeMethod(interactor, "execute", Qt::QueuedConnection);

    emit taskStarted(interactor->getId(), interactor->getMetadata());
}

void Executor::wrapInteractor(Interactor *interactor) {
    interactors.insert(interactor->getId(), interactor);
    tasksData.insert(interactor->getId(), interactor->getMetadata());

    connect(interactor, &Interactor::completed, this, &Executor::handleInteractorComplete);
    connect(interactor, &Interactor::metadataChanged, this, &Executor::handleInteractorMetadataChanged);
    interactor->moveToThread(&workerThread);
}

void Executor::handleInteractorComplete() {
    QMutexLocker locker(&lock);
    Interactor *i = dynamic_cast<Interactor *>(sender());
    if (i != nullptr) {
        const QString id = i->getId();

        QVariantMap resume;
        resume = i->getMetadata();
        interactors.remove(id);
        tasksData.remove(id);

        if (i->isAutoDelete())
            i->deleteLater();

        emit taskCompleted(id, resume);
    }
}


QStringList Executor::getRunningTasks() {
    QMutexLocker locker(&lock);
    return interactors.keys();
}

void Executor::cancel(const QString &id) {
    QMutexLocker locker(&lock);
    Interactor *i = interactors.value(id, nullptr);
    if (i)
        i->cancel();
}

void Executor::handleInteractorMetadataChanged(const QVariantMap &data) {
    QMutexLocker locker(&lock);
    Interactor *i = dynamic_cast<Interactor *>(sender());
    if (i != nullptr) {
        tasksData.insert(i->getId(), i->getMetadata());
        emit taskDataChanged(i->getId(), data);
    }

}

QVariantMap Executor::getTaskData(const QString &id) {
    QMutexLocker locker(&lock);
    Interactor *i = interactors.value(id, nullptr);
    if (i == nullptr)
        return QVariantMap();
    else
        return tasksData.value(i->getId(), QVariantMap());
}

Executor::~Executor() {
    workerThread.quit();
    workerThread.wait();
}


