#include "RegistryListModel.h"

#include <QDateTime>

#include "entities/TaskLogger.h"
#include "tasks/TaskMetadata.h"

RegistryListModel::RegistryListModel(QObject* parent)
    : QAbstractListModel(parent) {}

QHash<int, QByteArray> RegistryListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles.insert(Type, TaskMetadata::KEY_TYPE);
    roles.insert(Message, "message");
    roles.insert(TimeStamp, TaskLogger::KEY_TIME_STAMP);
    roles.insert(Persistence, TaskLogger::KEY_IS_PERSISTENT);
    roles.insert(Result, TaskMetadata::KEY_STATUS);
    roles.insert(APP_ID, TaskMetadata::KEY_APP_ID);
    roles.insert(APP_NAME, TaskMetadata::KEY_APP_NAME);
    roles.insert(APP_VERSION, TaskMetadata::KEY_APP_VERSION);
    return roles;
}

int RegistryListModel::rowCount(const QModelIndex& parent) const {
    // For list models only the root node (an invalid parent) should return the
    // list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not
    // become a tree model.
    if (parent.isValid())
        return 0;

    return records.size();
}

QString RegistryListModel::createRecordMessage(const QVariantMap& record) const {
    QString message;
    if (record.value(TaskMetadata::KEY_STATUS)
            .toString()
            .compare(TaskMetadata::VALUE_STATUS_COMPLETED) == 0) {

        if (record.value(TaskMetadata::KEY_TYPE)
                .toString()
                .compare(TaskMetadata::VALUE_TYPE_DELPOY) == 0) {

            message = "Application installed successfully.";
        }

        if (record.value(TaskMetadata::KEY_TYPE)
                .toString()
                .compare(TaskMetadata::VALUE_TYPE_REMOVE) == 0) {

            message = "Application removed successfully.";
        }
    } else {
        if (record.value(TaskMetadata::KEY_TYPE)
                .toString()
                .compare(TaskMetadata::VALUE_TYPE_DELPOY) == 0) {

            message = "Installation failed.";
        }

        if (record.value(TaskMetadata::KEY_TYPE)
                .toString()
                .compare(TaskMetadata::VALUE_TYPE_REMOVE) == 0) {

            message = "Remove failed.";
        }
    }
    return message;
}

QVariant RegistryListModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= records.count())
        return QVariant();

    const QVariantMap& record = records.at(index.row());
    QVariant ret;
    switch (role) {
    case Type:
        return record.value(TaskMetadata::KEY_TYPE);
    case Message:
        return createRecordMessage(record);
    case TimeStamp:
    {
        QDateTime dt  = record.value(TaskLogger::KEY_TIME_STAMP).toDateTime();
        return dt.toString(Qt::SystemLocaleLongDate);
    }
    case Persistence:
        return record.value(TaskLogger::KEY_IS_PERSISTENT);
    case Result:
        return record.value(TaskMetadata::KEY_STATUS);
    case APP_ID:
        return record.value(TaskMetadata::KEY_APP_ID);
    case APP_NAME:
        return record.value(TaskMetadata::KEY_APP_NAME);
    case APP_VERSION:
        return record.value(TaskMetadata::KEY_APP_VERSION);
    default:
        break;
    }

    return ret;
}

void RegistryListModel::setRecords(const QList<QVariantMap>& records) {
    beginResetModel();
    this->records = records;
    endResetModel();
}
