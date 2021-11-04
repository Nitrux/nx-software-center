#pragma once
// system

// libraries
#include <QObject>
#include <QString>

// local

class TaskAction : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString id MEMBER _id CONSTANT)
    Q_PROPERTY(QString label MEMBER _label CONSTANT)
    Q_PROPERTY(QString icon MEMBER _icon CONSTANT)
    Q_PROPERTY(bool isActive MEMBER _isActive)
public:
    TaskAction(const QString &id, const QString &label, const QString &icon, QObject *parent);

    Q_SIGNAL void triggered();
    Q_SCRIPTABLE void trigger();

    const QString &getId() const;
    const QString &getLabel() const;
    const QString &getIcon() const;
    bool isIsActive() const;

private:
    QString _id;
    QString _label;
    QString _icon;
    bool _isActive;
};
