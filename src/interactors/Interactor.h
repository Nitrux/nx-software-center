#ifndef INTERACTOR_H
#define INTERACTOR_H

#include <QVariantMap>
#include <QUuid>
#include <QObject>
#include <QReadWriteLock>

class Interactor : public QObject {
Q_OBJECT
    QString id;
    QReadWriteLock readWriteLock;

public:
    static constexpr const char* META_KEY_STATUS = "status";
    static constexpr const char* META_KEY_TYPE = "type";
    static constexpr const char* META_KEY_DESCRIPTION = "description";
    static constexpr const char* META_KEY_PROGRESS_VALUE = "progress_value";
    static constexpr const char* META_KEY_PROGRESS_TOTAL = "progress_total";
    static constexpr const char* META_KEY_PROGRESS_MESSAGE = "progress_message";

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


signals:
    void completed();

    void metadataChanged(const QVariantMap &changes);

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
