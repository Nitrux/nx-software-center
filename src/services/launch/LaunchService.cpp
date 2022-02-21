#include "LaunchService.h"

// libraries
#include <QDir>
#include <QProcess>

// local
#include <services/Application.h>

void LaunchService::launch(const QVariant &applicationVariant, int bundleIdx) const
{
    const Application &application = qvariant_cast<Application>(applicationVariant);
    if (bundleIdx == -1)
        bundleIdx = application.getMainBundleIndex();

    const auto &bundles = application.getBundles();
    if (bundleIdx >= 0 && bundleIdx < bundles.length()) {
        const auto &bundle = bundles[bundleIdx];

        const auto &appData = application.getData();
        QProcess *process = appData.requiresTerminal() ? createCLIProcess(bundle) : createGUIProcess(bundle);

        bool startSucceed = process->startDetached();
        if (!startSucceed) {
            emit(launchError(QVariant::fromValue(application), "Unable to execute " + bundle.path));
        }

        process->deleteLater();
    } else {
        emit(launchError(QVariant::fromValue(application), "Invalid bundle index " + QString::number(bundleIdx)));
    }
}

QProcess *LaunchService::createGUIProcess(const ApplicationBundle &bundle) const
{
    auto *process = new QProcess();
    process->setProgram(bundle.path);
    process->setWorkingDirectory(QDir::homePath());
    process->setStandardOutputFile(QProcess::nullDevice());
    process->setStandardErrorFile(QProcess::nullDevice());
    return process;
}

QVariantList LaunchService::listInstalledBundles(const QVariant &applicationVariant)
{
    QVariantList result;
    const Application &application = qvariant_cast<Application>(applicationVariant);
    const auto &bundles = application.getBundles();
    for (const auto &bundle : bundles) {
        const auto pathUrl = QUrl::fromLocalFile(bundle.path);
        result.push_back(pathUrl.fileName());
    }
    return result;
}
QProcess *LaunchService::createCLIProcess(const ApplicationBundle &bundle) const
{
    auto *process = new QProcess();
    process->setProgram("xterm");
    process->setArguments({bundle.path});
    process->setWorkingDirectory(QDir::homePath());
    process->setStandardOutputFile(QProcess::nullDevice());
    process->setStandardErrorFile(QProcess::nullDevice());
    return process;
}
