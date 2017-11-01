#include "removeappimagereleaseinteractor.h"

#include <QFile>
#include "../entities/registry.h"
#include "removeappimagereleaseinteractorlistener.h"

RemoveAppImageReleaseInteractor::RemoveAppImageReleaseInteractor(QString appId, QString releaseId, Registry *registry, RemoveAppImageReleaseInteractorListener *listener)
{
    m_appId = appId;
    m_releaseId = releaseId;

    Q_ASSERT(registry != nullptr);
    m_registry = registry;
    Q_ASSERT(listener != nullptr);
    m_listener = listener;
}

void RemoveAppImageReleaseInteractor::execute()
{
    QString releaseFilePath = m_registry->getReleaseFilePath(m_appId, m_releaseId);

    if (!releaseFilePath.isEmpty())
    {
        QFile file(releaseFilePath);
        if (file.exists() && !file.remove())
        {
            // The file exist but was imposible to remove it.
            m_listener->error(QString("Unable to remove file: %1").arg(releaseFilePath));
            return;
        }
        m_registry->registerReleaseRemove(m_appId, m_releaseId);
    }

    m_listener->finished();
}
