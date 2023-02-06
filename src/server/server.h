#pragma once
#include <QObject>

class OrgNxStoreActionsInterface;

namespace AppInstance
{
QSharedPointer<OrgNxStoreActionsInterface> instance();

bool attachToExistingInstance(const QPair<QString, QString>& options);

bool registerService();
}

class Server : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.nx.store.Actions")

public:
    explicit Server(QObject *parent = nullptr);
    void setQmlObject(QObject  *object);

public slots:
    /**
           * Tries to raise/activate the Dolphin window.
           */
    void activateWindow();

    /** Stores all settings and quits Dolphin. */
    void quit();

    void openApp(const QString &appId);
    void openCategory(const QString &category);
    void searchFor(const QString &query);
    void openView(const QString &view);

private:
    QObject* m_qmlObject = nullptr;


};

