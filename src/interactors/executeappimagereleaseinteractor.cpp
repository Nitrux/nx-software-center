#include "executeappimagereleaseinteractor.h"

#include <QProcess>

#include "../entities/registry.h"
#include "executeappimagereleaseinteractorlistener.h"

ExecuteAppImageReleaseInteractor::ExecuteAppImageReleaseInteractor(QString appId, QString releaseId, Registry *registry, ExecuteAppImageReleaseInteractorListener *listener)
{
    m_appId = appId;
    m_releaseId = releaseId;

    Q_ASSERT(registry != nullptr);
    m_registry = registry;

    Q_ASSERT(listener != nullptr);
    m_listener = listener;
}

void ExecuteAppImageReleaseInteractor::execute()
{

    QString path = m_registry->getReleaseFilePath(m_appId, m_releaseId);
    if (path.isEmpty())
        m_listener->fail("Release not found");
    else
    {
        QProcess process;
        // Start the process (uses new working dir)
        bool success = process.startDetached(path);
        if (success)
            m_listener->success();
        else
            m_listener->fail("Unable to start: " + path);
    }
}
