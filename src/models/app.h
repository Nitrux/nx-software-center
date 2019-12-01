#ifndef APP_H
#define APP_H

#include <QObject>

class App : public QObject
{
    Q_OBJECT
public:
    explicit App(QObject *parent = nullptr);

signals:

public slots:
};

#endif // APP_H
