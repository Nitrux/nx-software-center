//
// Created by alexis on 2/24/18.
//

#ifndef NOMAD_SOFTWARE_CENTER_RUNCONTROLLER_H
#define NOMAD_SOFTWARE_CENTER_RUNCONTROLLER_H

#include <QObject>
#include <entities/Registry.h>

class RunController : public QObject {
    Q_OBJECT
    Registry *registry;
public:
    RunController(Registry *registry, QObject *parent = nullptr);

public slots:
    void run(const QString &id);

private:
    QString getExecutable(const QStringList &files) const;

    void spawnRunInteractor(const QString &executable);
};


#endif //NOMAD_SOFTWARE_CENTER_RUNCONTROLLER_H
