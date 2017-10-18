#include "appimageuninstalljob.h"

#include <QDir>
#include <QFile>
#include <QtConcurrent/QtConcurrent>
#include <QStandardPaths>

AppImageUninstallJob::AppImageUninstallJob(QString appImageId, QObject *parent) : KJob(parent)
{
    m_appImageId = appImageId;
}

void AppImageUninstallJob::start()
{
    // Do work async
    QtConcurrent::run([=]()
    {
        setStatus("Uninstalling " + m_appImageId);

        QStringList results;
        QString path = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first() + "/.local/share";
        QString query = "appimagekit*" + m_appImageId + "*";
        findRecursion(path, query, &results);

        for (QString filePath: results)
        {
            QFile file(filePath);
            file.remove();
        }
        emitResult();
        setStatus("Uninstalling " + m_appImageId + " finished.");
    });
}

QString AppImageUninstallJob::status() const
{
    return m_status;
}

void AppImageUninstallJob::setStatus(QString status)
{
    if (m_status == status)
        return;

    m_status = status;
    emit statusChanged(m_status);
}

void AppImageUninstallJob::findRecursion(const QString &path, const QString &pattern, QStringList *result)
{
    QDir currentDir(path);
    const QString prefix = path + QLatin1Char('/');
    foreach (const QString &match, currentDir.entryList(QStringList(pattern), QDir::Files | QDir::NoSymLinks))
        result->append(prefix + match);
    foreach (const QString &dir, currentDir.entryList(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot))
        findRecursion(prefix + dir, pattern, result);
}
