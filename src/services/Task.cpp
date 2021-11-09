#include "Task.h"

#include <utility>

Task::Task(QString id, QString title, QString subtitle, QString icon, int progressTotal, int progress, QObject *parent)
    : QObject(parent)
    , _id(std::move(id))
    , _title(std::move(title))
    , _subtitle(std::move(subtitle))
    , _icon(std::move(icon))
    , _progressTotal(progressTotal)
    , _progress(progress)
    , _status(Status::DRAFT)
{
}

const QString &Task::getId() const
{
    return _id;
}

const QString &Task::getTitle() const
{
    return _title;
}

void Task::setTitle(const QString &title)
{
    _title = title;
    emit titleChanged(_title);
}

const QString &Task::getSubtitle() const
{
    return _subtitle;
}

void Task::setSubtitle(const QString &subtitle)
{
    _subtitle = subtitle;
    emit subtitleChanged(_subtitle);
}

const QString &Task::getIcon() const
{
    return _icon;
}

void Task::setIcon(const QString &icon)
{
    _icon = icon;
    emit iconChanged(icon);
}

int Task::getProgressTotal() const
{
    return _progressTotal;
}
void Task::setProgressTotal(int progressTotal)
{
    _progressTotal = progressTotal;
    emit progressTotalChanged(_progressTotal);
}
int Task::getProgress() const
{
    return _progress;
}
void Task::setProgress(int progress)
{
    _progress = progress;
    emit progressChanged(_progress);
}
Task::Status Task::getStatus() const
{
    return _status;
}
void Task::setStatus(Status status)
{
    _status = status;
    emit statusChanged(_status);
}
TaskAction *Task::addAction(const QString &id, const QString &label, const QString &icon)
{
    auto action = new TaskAction(id, label, icon, this);
    _actions.push_back(action);

    emit actionsChanged(getActions());

    return action;
}
QVariant Task::getActions()
{
    return QVariant::fromValue(_actions);
}

TaskAction *Task::addCancelAction(const QString &label, const QString &icon)
{
    auto action = new TaskAction("stop", label, icon, this);
    _actions.push_back(action);

    emit actionsChanged(getActions());
    return action;
}
void Task::removeAction(TaskAction *action)
{
    int idx = _actions.indexOf(action);
    if (idx >= 0) {
        _actions.removeAt(idx);
        action->deleteLater();

        emit actionsChanged(getActions());
    }
}
