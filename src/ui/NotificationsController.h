//
// Created by alexis on 8/02/18.
//

#ifndef NOMAD_SOFTWARE_CENTER_NOTIFICATIONSCONTROLLER_H
#define NOMAD_SOFTWARE_CENTER_NOTIFICATIONSCONTROLLER_H

#include <QObject>

class NotificationsController : public QObject {
    Q_OBJECT
public:
    explicit NotificationsController(QObject *parent = nullptr): QObject(parent) {}

};


#endif //NOMAD_SOFTWARE_CENTER_NOTIFICATIONSCONTROLLER_H
