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
        descriptions.remove(id);

        emit taskCompleted(id);
        i->deleteLater();
    }
}

QString Executor::getTaskDescription(const QString &id) {
    return descriptions.value(id, QString());
}

void Executor::execute(Interactor *i, const QString &description) {
    descriptions.insert(i->getId(), description);
    execute(i);
}

QStringList Executor::getRunningTasks() {
    return tasks.keys();
}

void Executor::handleInteractorMetadataChanged(const QVariantMap &data) {
    Interactor* i = dynamic_cast<Interactor*>(sender());
    if (i != nullptr)
        emit taskDataChanged(i->getId(), data);
}

Executor::InteractorRunnableWrapper::InteractorRunnableWrapper(Interactor *i) : i(i){
}

void Executor::InteractorRunnableWrapper::run() {
    i->execute();
}
