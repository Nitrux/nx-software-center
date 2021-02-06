#ifndef APPSMODEL_H
#define APPSMODEL_H

#include <QObject>
#include <QProcess>

#include <MauiKit/fmh.h>
#include <MauiKit/mauilist.h>

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
    void launchApp(const int &index);
    void removeApp(const int &index);
    void resfresh();

private:
    FMH::MODEL_LIST m_list;
    QHash<QString, Application *> m_app;
    Store *m_store;
    void setList();
};

#endif // APPSMODEL_H
