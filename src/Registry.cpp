//
// Created by alexis on 26/12/17.
//

#include "Registry.h"

#include <QDate>

void Registry::registerDownload(const QString &app_id, QStringList files) {
    QString l = QString("%1: %2 downloaded.").arg(app_id).arg(QDate::currentDate().toString());
    log.append(l);
    downloads.insert(app_id, files);
}

void Registry::registerRemove(const QString &app_id) {
    QString l = QString("%1: %2 removed.").arg(app_id).arg(QDate::currentDate().toString());
    log.append(l);
    downloads.remove(app_id);
}

bool Registry::isDownloaded(const QString &app_id) {
    return downloads.contains(app_id);
}

QStringList Registry::getDownloadedFiles(const QString &app_id) {
    return downloads.value(app_id, QStringList());
}

void Registry::loadLogs(QSettings &settings)
{
    int size = settings.beginReadArray("Logs");
    for (int i = 0; i < size; i++) {
        settings.setArrayIndex(i);
        QString l = settings.value("text").toString();
        log.append(l);
    }
    settings.endArray();
}

void Registry::loadApplicationsDownloaded(QSettings &settings)
{
    settings.beginGroup("Applications_Downloaded");
    QStringList keys = settings.allKeys();
    for (QString key: keys)
        downloads[key] = settings.value(key).toStringList();
    settings.endGroup();
}

void Registry::load() {
    QSettings settings;
    loadApplicationsDownloaded(settings);
    loadLogs(settings);
}

void Registry::saveApplicationsDownloaded(QSettings &settings)
{
    settings.beginGroup("Applications_Downloaded");
    settings.remove("");
    QStringList keys = downloads.keys();
    for (QString key: keys)
        settings.setValue(key, downloads[key]);
    settings.endGroup();
}

void Registry::saveLogs(QSettings &settings)
{
    settings.beginWriteArray("Logs");

    for (int i = 0; i < log.size(); i ++) {
        settings.setArrayIndex(i);
        settings.setValue("text", log.at(i));
    }
    settings.endArray();
}

void Registry::save()
{
    QSettings settings;
    saveApplicationsDownloaded(settings);
    saveLogs(settings);
}
