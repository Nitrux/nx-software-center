//
// Created by alexis on 26/12/17.
//

#ifndef NOMAD_SOFTWARE_CENTER_REGISTRY_H
#define NOMAD_SOFTWARE_CENTER_REGISTRY_H

#include <QMap>
#include <QObject>
#include <QStringList>
#include <QSettings>

class Registry : public QObject {
    Q_OBJECT

    QMap<QString, QStringList> downloads;
    QStringList log;
public:
    explicit Registry(QObject *parent) : QObject(parent) {}

public slots:
    void registerDownload(const QString &app_id, QStringList files);
    void registerRemove(const QString &app_id);

    bool isDownloaded(const QString &app_id);
    QStringList getDownloadedFiles(const QString &app_id);

private:
    void load();

    void save();

    void loadLogs(QSettings &settings);

    void loadApplicationsDownloaded(QSettings &settings);


    void saveApplicationsDownloaded(QSettings &settings);

    void saveLogs(QSettings &settings);
};


#endif //NOMAD_SOFTWARE_CENTER_REGISTRY_H
