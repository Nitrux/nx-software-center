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

    Q_PROPERTY(bool isCanceled
                       MEMBER
                       isCanceled
                       NOTIFY
                       isCanceledChanged)
protected:
    bool isCanceled;
    QVariantMap metadata;

public:
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
    void setMetadata(const QVariantMap &data) {
        readWriteLock.lockForWrite();
        for (const QString &k : data.keys())
            metadata.insert(k, data[k]);

        readWriteLock.unlock();
        emit metadataChanged(data);
    }
};

#endif // INTERACTOR_H
