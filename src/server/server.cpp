#include "server.h"

#include <QGuiApplication>
#include <QQuickWindow>
#include <QQmlApplicationEngine>
#include <QtDBus/QDBusConnectionInterface>

#include "serverinterface.h"
#include "storeadaptor.h"

QSharedPointer<OrgNxStoreActionsInterface> AppInstance::instance()
{
    // Look for dolphin instances among all available dbus services.
    QDBusConnectionInterface *sessionInterface = QDBusConnection::sessionBus().interface();
    const QStringList dbusServices = sessionInterface ? sessionInterface->registeredServiceNames().value() : QStringList();
    // Don't match the service without trailing "-" (unique instance)
    const QString service = QStringLiteral("org.nx.store");

    if (dbusServices.contains(service))
    {
        qDebug() << "EXISTING INTANCES" << service;

        // Check if instance can handle our URLs
        QSharedPointer<OrgNxStoreActionsInterface> interface(
                    new OrgNxStoreActionsInterface(service,
                                                   QStringLiteral("/Actions"),
                                                   QDBusConnection::sessionBus()));

        if (interface->isValid() && !interface->lastError().isValid())
        {
            return interface;
        }
    }

    return nullptr;
}

bool AppInstance::attachToExistingInstance(const QPair<QString, QString>& options)
{
    bool attached = false;

    if (!instance())
    {
        return attached;
    }

    auto interface = instance();

    const auto op = options.first;
    const auto value = options.second;

    attached = true;

    if(!op.isEmpty())
    {
        QDBusPendingReply reply;

        if(op == "oa")
        {
            reply = interface->openApp(value);
        }else if(op == "oc")
        {
            reply = interface->openCategory(value);
        }else if(op == "ov")
        {
            qDebug() << "ASKED TO OPEN A VIEW << " << value ;
            reply = interface->openView(value);
        }else if(op == "s")
        {
            reply = interface->searchFor(value);
        }

        reply.waitForFinished();
    }

    interface->activateWindow();
    return attached;
}

bool AppInstance::registerService()
{
    QDBusConnectionInterface *iface = QDBusConnection::sessionBus().interface();

    auto registration = iface->registerService(QStringLiteral("org.nx.store"),
                                               QDBusConnectionInterface::ReplaceExistingService,
                                               QDBusConnectionInterface::DontAllowReplacement);

    if (!registration.isValid())
    {
        qDebug() << "ERROR REGISTER << org.nx.store";

        qWarning("2 Failed to register D-Bus service \"%s\" on session bus: \"%s\"",
                 qPrintable("org.nx.store"),
                 qPrintable(registration.error().message()));
        return false;
    }

    return true;
}


Server::Server(QObject *parent) : QObject(parent)
  , m_qmlObject(nullptr)
{
    qDebug() << "TRYIGN TO REGISTER << org.nx.store/Actions";

    new ActionsAdaptor(this);
    if(!QDBusConnection::sessionBus().registerObject(QStringLiteral("/Actions"), this))
    {
        qDebug() << "FAILED TO REGISTER NX-SC ACTIONS DBUS OBJECT";
        return;
    }else
    {
        qDebug() << "SUCESSFULL REGISTER << org.nx.store/Actions";

    }
}

void Server::setQmlObject(QObject *object)
{
    if(!m_qmlObject)
    {
        m_qmlObject = object;
    }
}

void Server::activateWindow()
{
    if(m_qmlObject)
    {
        qDebug() << "ACTIVET WINDOW FROM C++";
        auto window = qobject_cast<QQuickWindow *>(m_qmlObject);
        if (window)
        {
            qDebug() << "Trying to raise wndow";
            window->raise();
            window->requestActivate();
        }
    }
}

void Server::quit()
{
    QCoreApplication::quit();
}

void Server::openApp(const QString &appId)
{
    if(m_qmlObject)
    {
        if(!appId.isEmpty())
        {
            QMetaObject::invokeMethod(m_qmlObject, "openApp",
                                      Q_ARG(QVariant, appId));
        }

    }
}

void Server::openCategory(const QString &category)
{
    if(m_qmlObject)
    {
        if(!category.isEmpty())
        {
            QMetaObject::invokeMethod(m_qmlObject, "openCategory",
                                      Q_ARG(QVariant, category));
        }

    }
}

void Server::searchFor(const QString &query)
{
    if(m_qmlObject)
    {
        if(!query.isEmpty())
        {
            QMetaObject::invokeMethod(m_qmlObject, "searchFor",
                                      Q_ARG(QVariant, query));
        }

    }
}

void Server::openView(const QString &view)
{
    qDebug() << "ASKED TO OPEN A VIEW FOR QML OBJECT << " << view  ;

    if(m_qmlObject)
    {
        if(!view.isEmpty())
        {

            QMetaObject::invokeMethod(m_qmlObject, "openView",
                                      Q_ARG(QVariant, view));
        }

    }
}

