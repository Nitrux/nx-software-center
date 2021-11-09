#include "TaskAction.h"
TaskAction::TaskAction(const QString &id, const QString &label, const QString &icon, QObject *parent)
    : QObject(parent)
    , _id(id)
    , _label(label)
    , _icon(icon)
{
}
const QString &TaskAction::getId() const
{
    return _id;
}
const QString &TaskAction::getLabel() const
{
    return _label;
}
const QString &TaskAction::getIcon() const
{
    return _icon;
}

void TaskAction::trigger()
{
    emit triggered();
}
bool TaskAction::isIsActive() const
{
    return _isActive;
}
