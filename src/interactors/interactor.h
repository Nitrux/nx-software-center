#ifndef INTERACTOR_H
#define INTERACTOR_H

#include <QObject>

class Interactor : public QObject {
  Q_OBJECT
public:
  Interactor(QObject *parent = nullptr);
  virtual ~Interactor();

signals:
  void complete();

public slots:
  virtual void execute() = 0;
};

#endif // INTERACTOR_H
