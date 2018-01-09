//
// Created by alexis on 8/01/18.
//

#include <QThreadPool>

#include "Executor.h"
#include "interactors/Interactor.h"

void Executor::execute(Interactor *i) {
    Q_ASSERT(i);

    InteractorRunnableWrapper *w = new InteractorRunnableWrapper(i);
    w->setAutoDelete(true);

    tasks.insert(i->getId(), w);
    connect(i, &Interactor::completed, this, &Executor::handleInteractorComplete);
    connect(i, &Interactor::metadataChanged, this, &Executor::handleInteractorMetadataChanged);
    QThreadPool::globalInstance()->start(w);

    emit taskStarted(i->getId());
}

void Executor::handleInteractorComplete() {
    Interactor* i = dynamic_cast<Interactor*>(sender());
    if (i != nullptr) {
        const QString id = i->getId();
        tasks.remove(id);

        emit taskCompleted(id);
        i->deleteLater();
    }
}


QStringList Executor::getRunningTasks() {
    return tasks.keys();
}

void Executor::handleInteractorMetadataChanged(const QVariantMap &data) {
    Interactor* i = dynamic_cast<Interactor*>(sender());
    if (i != nullptr)
        emit taskDataChanged(i->getId(), data);
}

QVariantMap Executor::getTaskData(const QString &id) {
    InteractorRunnableWrapper *w = tasks.value(id, nullptr);
    if (w) {
        Interactor *i = w->getIntereactor();
        if (i)
            return i->getMetadata();
    }

    return QVariantMap();
}

Executor::InteractorRunnableWrapper::InteractorRunnableWrapper(Interactor *i) : i(i){
}

void Executor::InteractorRunnableWrapper::run() {
    i->execute();
}
