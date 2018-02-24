//
// Created by alexis on 2/24/18.
//

#include "RunInteractor.h"

#include <QFileInfo>
#include <QProcess>

RunInteractor::RunInteractor(const QString &target, QObject *parent) : Interactor(parent), target(target) {}

void RunInteractor::execute() {
    QFileInfo fi(target);
    QProcess::startDetached(fi.absoluteFilePath(), {}, fi.absolutePath());
    emit completed();
}
