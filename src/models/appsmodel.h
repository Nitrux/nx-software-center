#ifndef APPSMODEL_H
#define APPSMODEL_H

#include <QObject>
#include <QProcess>

#ifdef STATIC_MAUIKIT
#include "fmh.h"
#include "mauilist.h"
#else
#include <MauiKit/fmh.h>
#include <MauiKit/mauilist.h>
#endif

class Store;
class Application;
class AppsModel : public MauiList {
    Q_OBJECT

public:
    explicit AppsModel(QObject *parent = nullptr);

    // QQmlParserStatus interface
    void componentComplete() override final;

    const FMH::MODEL_LIST &items() const override final;

signals:
    void appLaunchSuccess();
    void appLaunchError(int err);
    void appDeleteSuccess();

public slots:
    void launchApp(QString path);
    void removeApp(QString path);

private:
    FMH::MODEL_LIST m_list;
    QHash<QString, Application *> m_app;
    Store *m_store;
};

#endif // APPSMODEL_H
