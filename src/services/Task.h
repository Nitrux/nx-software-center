#pragma once
// system

// libraries
#include <QIcon>
#include <QMap>
#include <QObject>
#include <QVariant>

// local
#include "TaskAction.h"

class Task : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString id READ getId)
    Q_PROPERTY(QString title READ getTitle NOTIFY titleChanged)
    Q_PROPERTY(QString subtitle READ getSubtitle NOTIFY subtitleChanged)
    Q_PROPERTY(QString icon READ getIcon NOTIFY iconChanged)
    Q_PROPERTY(int progress READ getProgress NOTIFY progressChanged)
    Q_PROPERTY(int progressTotal READ getProgressTotal NOTIFY progressTotalChanged)
    Q_PROPERTY(Status status READ getStatus NOTIFY statusChanged)
    Q_PROPERTY(QVariant actions READ getActions NOTIFY actionsChanged)

public:
    enum class Status { DRAFT, ACTIVE, COMPLETED, CANCELED, FAILED };
    Q_ENUM(Status)

    Q_SIGNAL void titleChanged(QString title);
    Q_SIGNAL void subtitleChanged(QString subtitle);
    Q_SIGNAL void iconChanged(QString icon);
    Q_SIGNAL void progressChanged(int progress);
    Q_SIGNAL void progressTotalChanged(int progressTotal);
    Q_SIGNAL void statusChanged(Status status);
    Q_SIGNAL void actionsChanged(QVariant actions);

public:
    Task(QString id,
         QString title,
         QString subtitle = QString(),
         QString icon = QString(),
         int progressTotal = -1,
         int progress = -1,
         QObject *parent = nullptr);

    const QString &getId() const;
    const QString &getTitle() const;
    void setTitle(const QString &title);
    const QString &getSubtitle() const;
    void setSubtitle(const QString &subtitle);
    const QString &getIcon() const;
    void setIcon(const QString &icon);
    int getProgressTotal() const;
    void setProgressTotal(int progressTotal);
    int getProgress() const;
    void setProgress(int progress);
    Status getStatus() const;
    void setStatus(Status status);

    QVariant getActions();
    Q_SCRIPTABLE void setActions(QList<TaskAction *> actions);

    Q_SCRIPTABLE TaskAction *addAction(const QString &id, const QString &label, const QString &icon);
    Q_SCRIPTABLE TaskAction *addCancelAction(const QString &label = "Cancel", const QString &icon = "dialog-close");
    Q_SCRIPTABLE void removeAction(TaskAction *action);

    Q_SCRIPTABLE void start();

private:
    QString _id;
    QString _title;
    QString _subtitle;
    QString _icon;

    // progress data, -1 means no progress information
    int _progressTotal{};
    int _progress{};

    // current task status
    Status _status;

    QList<TaskAction *> _actions;
};
