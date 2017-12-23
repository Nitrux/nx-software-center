#ifndef INTERACTOR_H
#define INTERACTOR_H

#include <QObject>

class Interactor : public QObject {
Q_OBJECT
public:
    explicit Interactor(QObject *parent = nullptr) : QObject(parent) {}

    virtual ~Interactor() {}

signals:

    void completed();

public slots:

    virtual void execute() = 0;
};

#endif // INTERACTOR_H
