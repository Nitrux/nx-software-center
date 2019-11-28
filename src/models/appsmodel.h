#ifndef APPSMODEL_H
#define APPSMODEL_H

#include <QObject>
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
    Q_PROPERTY(QString categoryUri READ getCategoryUri WRITE setCategoryUri NOTIFY
               categoryUriChanged)

public:
    explicit AppsModel(QObject *parent = nullptr);

    // QQmlParserStatus interface
    void componentComplete() override final;

    FMH::MODEL_LIST items() const override final;

    QString getCategoryUri() const;
    void setCategoryUri(QString categoryUri);

signals:
    void categoryUriChanged(QString categoryUri);

private:
    FMH::MODEL_LIST m_list;
    QHash<QString, Application *> m_app;
    Store *m_store;

    void setList();

    QString m_categoryUri;
};

#endif // APPSMODEL_H
