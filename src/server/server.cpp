#include "server.h"

#include <QGuiApplication>
#include <QQuickWindow>
#include <QQmlApplicationEngine>

#include <MauiKit/FileBrowsing/fmstatic.h>

#if (defined Q_OS_LINUX || defined Q_OS_FREEBSD) && !defined Q_OS_ANDROID
#include "vvaveinterface.h"
#include "vvaveadaptor.h"

QVector<QPair<QSharedPointer<OrgKdeVvaveActionsInterface>, QStringList>> AppInstance::appInstances(const QString& preferredService)
{
    QVector<QPair<QSharedPointer<OrgKdeVvaveActionsInterface>, QStringList>> dolphinInterfaces;

    if (!preferredService.isEmpty())
    {
        QSharedPointer<OrgKdeVvaveActionsInterface> preferredInterface(
                    new OrgKdeVvaveActionsInterface(preferredService,
                                                   QStringLiteral("/Actions"),
                                                   QDBusConnection::sessionBus()));

        qDebug() << "IS PREFRFRED INTERFACE VALID?" << preferredInterface->isValid() << preferredInterface->lastError().message();
        if (preferredInterface->isValid() && !preferredInterface->lastError().isValid()) {
            dolphinInterfaces.append(qMakePair(preferredInterface, QStringList()));
        }
    }

    // Look for dolphin instances among all available dbus services.
    QDBusConnectionInterface *sessionInterface = QDBusConnection::sessionBus().interface();
    const QStringList dbusServices = sessionInterface ? sessionInterface->registeredServiceNames().value() : QStringList();
    // Don't match the service without trailing "-" (unique instance)
    const QString pattern = QStringLiteral("org.kde.vvave-");

    // Don't match the pid without leading "-"
    const QString myPid = QLatin1Char('-') + QString::number(QCoreApplication::applicationPid());

    for (const QString& service : dbusServices)
    {
        if (service.startsWith(pattern) && !service.endsWith(myPid))
        {
            qDebug() << "EXISTING INTANCES" << service;

            // Check if instance can handle our URLs
            QSharedPointer<OrgKdeVvaveActionsInterface> interface(
                        new OrgKdeVvaveActionsInterface(service,
                                                       QStringLiteral("/Actions"),
                                                       QDBusConnection::sessionBus()));
            if (interface->isValid() && !interface->lastError().isValid())
            {
                dolphinInterfaces.append(qMakePair(interface, QStringList()));
            }
        }
    }

    return dolphinInterfaces;
}

bool AppInstance::attachToExistingInstance(const QList<QUrl>& inputUrls, const QString& preferredService)
{
    bool attached = false;

    auto dolphinInterfaces = appInstances(preferredService);
    if (dolphinInterfaces.isEmpty())
    {
        return attached;
    }

    QStringList newUrls;

    // check to see if any instances already have any of the given URLs open
    const auto urls = QUrl::toStringList(inputUrls);
    for (const QString& url : urls)
    {
        bool urlFound = false;

        for (auto& interface: dolphinInterfaces)
        {
            auto isUrlOpenReply = interface.first->isUrlOpen(url);
            isUrlOpenReply.waitForFinished();

            if (!isUrlOpenReply.isError() && isUrlOpenReply.value())
            {
                interface.second.append(url);
                urlFound = true;
                break;
            }
        }

        if (!urlFound)
        {
            newUrls.append(url);
        }
    }

    for (const auto& interface: qAsConst(dolphinInterfaces))
    {
        auto reply = interface.first->openFiles(newUrls);
        reply.waitForFinished();

        if (!reply.isError())
        {
            interface.first->activateWindow();
            attached = true;
            break;
        }
    }

    return attached;
}

bool AppInstance::registerService()
{
    QDBusConnectionInterface *iface = QDBusConnection::sessionBus().interface();

    auto registration = iface->registerService(QStringLiteral("org.kde.vvave-%1").arg(QCoreApplication::applicationPid()),
                                               QDBusConnectionInterface::ReplaceExistingService,
                                               QDBusConnectionInterface::DontAllowReplacement);

    if (!registration.isValid())
    {
        qWarning("2 Failed to register D-Bus service \"%s\" on session bus: \"%s\"",
                 qPrintable("org.kde.vvave"),
                 qPrintable(registration.error().message()));
        return false;
    }

    return true;
}

#endif


Server::Server(QObject *parent) : QObject(parent)
  , m_qmlObject(nullptr)
{
#if (defined Q_OS_LINUX || defined Q_OS_FREEBSD) && !defined Q_OS_ANDROID
    new ActionsAdaptor(this);
    if(!QDBusConnection::sessionBus().registerObject(QStringLiteral("/Actions"), this))
    {
        qDebug() << "FAILED TO REGISTER BACKGROUND DBUS OBJECT";
        return;
    }
#endif
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

void Server::openFiles(const QStringList &urls)
{
    if(m_qmlObject)
    {
        auto files = filterFiles(urls);
        if(!files.isEmpty())
        {
            QMetaObject::invokeMethod(m_qmlObject, "openFiles",
                                      Q_ARG(QVariant, files));
        }

    }
}

bool Server::isUrlOpen(const QString &url)
{
    bool value = false;

    if(m_qmlObject)
    {
        QMetaObject::invokeMethod(m_qmlObject, "isUrlOpen",
                                  Q_RETURN_ARG(bool, value),
                                  Q_ARG(QString, url));
    }
    return value;
}

QStringList Server::filterFiles(const QStringList &urls)
{
    qDebug() << "REQUEST FILES" << urls;
    QStringList res;
    for (const auto &url : urls) {
        const auto url_ = QUrl::fromUserInput(url);
        qDebug() << "REQUEST FILES" << url_.toString() << FMStatic::getMime(url_);

        if (FMStatic::checkFileType(FMStatic::FILTER_TYPE::AUDIO, FMStatic::getMime(url_)))
            res << url_.toString();
    }

    qDebug() << "REQUEST FILES" << res;

    return res;
}
