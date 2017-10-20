#ifndef APPIMAGEREPOSITORY_H
#define APPIMAGEREPOSITORY_H

#include <QObject>
#include <QList>

class AppImage;

typedef QList<AppImage*> AppImageList;

class AppImageRepository : public QObject
{
    Q_OBJECT
    Q_PROPERTY(AppImageList items READ items WRITE setItems NOTIFY itemsChanged)


public:
    explicit AppImageRepository(QObject *parent = nullptr);

    Q_INVOKABLE virtual int count() = 0;
    Q_INVOKABLE virtual AppImageList search(QString query, int offset, int limit) = 0;

    AppImageList items() const;
signals:

    void itemsChanged(AppImageList items);

protected slots:
    void setItems(AppImageList items);

protected:
    QList<AppImage *> m_items;
};

#endif // APPIMAGEREPOSITORY_H
