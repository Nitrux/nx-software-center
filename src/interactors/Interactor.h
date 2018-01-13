#ifndef INTERACTOR_H
#define INTERACTOR_H

#include <QVariantMap>
#include <QUuid>
#include <QObject>
#include <QReadWriteLock>

class Interactor : public QObject {
Q_OBJECT
    QString id;
    bool stoped;
    QReadWriteLock readWriteLock;

    Q_PROPERTY(bool isCanceled MEMBER isCanceled NOTIFY isCanceledChanged)
    bool isCanceled;

public:
    static constexpr const char* META_KEY_STATUS = "status";
    static constexpr const char* META_KEY_TYPE = "type";
    static constexpr const char* META_KEY_DESCRIPTION = "description";
    static constexpr const char* META_KEY_PROGRESS_VALUE = "progress_value";
    static constexpr const char* META_KEY_PROGRESS_TOTAL = "progress_total";
    static constexpr const char* META_KEY_PROGRESS_MESSAGE = "progress_message";

    static constexpr const char* META_KEY_APP_ID = "task_application_id";
    static constexpr const char* META_KEY_APP_NAME = "task_application_name";
    static constexpr const char* META_KEY_APP_AUTHOR = "task_application_author";

    static constexpr const char* STATUS_CREATED = "created";
    static constexpr const char* STATUS_RUNNING = "running";
    static constexpr const char* STATUS_COMPLETED = "completed";
    static constexpr const char* STATUS_FAILED = "failed";

    explicit Interactor(QObject *parent = nullptr) : QObject(parent) { id = QUuid::createUuid().toString(); }

    explicit Interactor(const QString &id, QObject *parent = nullptr) : QObject(parent), id(id) {}

    virtual ~Interactor() {}

    QString getId() { return id; }

    const QVariantMap getMetadata() {
        readWriteLock.lockForRead();

        QVariantMap ret = metadata;
        readWriteLock.unlock();

        return ret;
    }

    void cancel() {
        isCanceled = true;
        emit isCanceledChanged(isCanceled);
    }


signals:
    void completed();

    void metadataChanged(const QVariantMap &changes);

    void isCanceledChanged(bool isCanceled);
public slots:
    virtual void execute() = 0;

protected:
    QVariantMap metadata;
    void setMetadata(const QVariantMap &data) {
        readWriteLock.lockForWrite();
        for (const QString &k : data.keys())
            metadata.insert(k, data[k]);

        readWriteLock.unlock();
        emit metadataChanged(data);
    }
};

#endif // INTERACTOR_H
