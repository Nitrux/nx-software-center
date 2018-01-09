#ifndef INTERACTOR_H
#define INTERACTOR_H

#include <QVariantMap>
#include <QUuid>
#include <QObject>

class Interactor : public QObject {
Q_OBJECT
    QString id;
    QVariantMap metadata;

public:
    explicit Interactor(QObject *parent = nullptr) : QObject(parent) { id = QUuid::createUuid().toString(); }

    explicit Interactor(const QString &id, QObject *parent = nullptr) : QObject(parent), id(id) {}

    virtual ~Interactor() {}

    QString getId() { return id; }

    const QVariantMap getMetadata() const {
        return metadata;
    }


signals:
    void completed();

    void metadataChanged(const QVariantMap &changes);

public slots:
    virtual void execute() = 0;

protected:
    void setMetadata(const QVariantMap &data) {
        for (const QString &k : data.keys())
            metadata.insert(k, data[k]);

        emit metadataChanged(data);
    }
};

#endif // INTERACTOR_H
