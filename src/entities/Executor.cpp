//
// Created by alexis on 8/01/18.
//

#include <QThreadPool>
#include <QMutexLocker>
#include <QDebug>
#include <QCoreApplication>

#include "Executor.h"
#include "interactors/Interactor.h"

class Executor::InteractorRunnableWrapper : public QRunnable {
    Interactor *interactor;
public:
    InteractorRunnableWrapper(Interactor *interactor) : interactor(interactor) {}

    virtual ~InteractorRunnableWrapper() {}

    void run() override {
        interactor->execute();
    }
};


void Executor::execute(Interactor *interactor) {
    QMutexLocker locker(&lock);
    const QString id = interactor->getId();
    InteractorRunnableWrapper *runnableWrapper = new InteractorRunnableWrapper(interactor);
    runnableWrapper->setAutoDelete(true);

    interactors.insert(id, interactor);
    runnables.insert(id, runnableWrapper);


    connect(interactor, &Interactor::completed, this, &Executor::handleInteractorComplete);
    connect(interactor, &Interactor::metadataChanged, this, &Executor::handleInteractorMetadataChanged);

    QThreadPool::globalInstance()->start(runnableWrapper);

    emit taskStarted(id);
}

void Executor::handleInteractorComplete() {
    QMutexLocker locker(&lock);
    Interactor *i = dynamic_cast<Interactor *>(sender());
    if (i != nullptr) {
        const QString id = i->getId();

        if (runnables.contains(id))
            runnables.remove(id);

        Interactor *interactor = interactors.value(id);
        QVariantMap resume;
        if (interactor != nullptr) {
            resume = interactor->getMetadata();
            interactors.remove(id);
            interactor->deleteLater();
        }

        emit taskCompleted(id, resume);
    }
}


QStringList Executor::getRunningTasks() {
    QMutexLocker locker(&lock);
    return interactors.keys();
}

void Executor::cancel(const QString &id)
{
    QMutexLocker locker(&lock);
    Interactor *i = interactors.value(id, nullptr);
    if (i)
        i->cancel();
}

void Executor::handleInteractorMetadataChanged(const QVariantMap &data) {
    QMutexLocker locker(&lock);
    Interactor *i = dynamic_cast<Interactor *>(sender());
    if (i != nullptr)
            emit taskDataChanged(i->getId(), data);
}

QVariantMap Executor::getTaskData(const QString &id) {
    QMutexLocker locker(&lock);
    Interactor *i = interactors.value(id, nullptr);
    if (i == nullptr)
        return QVariantMap();
    else
        return i->getMetadata();
}


