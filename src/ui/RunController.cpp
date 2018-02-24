//
// Created by alexis on 2/24/18.
//

#include <interactors/RunInteractor.h>
#include <QtCore/QFileInfo>

#include "RunController.h"


void RunController::run(const QString &id) {
    auto files = registry->getInstalledApplicationFiles(id);

    QString executable = getExecutable(files);

    spawnRunInteractor(executable);
}

void RunController::spawnRunInteractor(const QString &executable) {
    RunInteractor *i = new RunInteractor(executable, this);

    connect(i, &RunInteractor::completed, [i]() { i->deleteLater(); });
    i->execute();
}

QString RunController::getExecutable(const QStringList &files) const {
    QString executable;
    for (const QString &file: files) {
        QFileInfo fi(file);
        if (fi.isExecutable())
            executable = fi.absoluteFilePath();
    }
    return executable;
}

RunController::RunController(Registry *registry, QObject *parent) : QObject(parent), registry(registry) {}
