#ifndef PACKAGE_H
#define PACKAGE_H

#include <QObject>
#include "app.h"

namespace FMH {
class Downloader;
}

class Package : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl link READ getLink NOTIFY linkChanged FINAL)
    Q_PROPERTY(QUrl path READ getPath NOTIFY pathChanged FINAL)
    Q_PROPERTY(QVariantMap package READ getPackage NOTIFY packageChanged FINAL)
    Q_PROPERTY(int progress MEMBER m_progress NOTIFY progressChanged)
    Q_PROPERTY(bool running READ isRunning NOTIFY isRunningChanged FINAL)
    Q_PROPERTY(bool finished READ isFinished NOTIFY isFinishedChanged FINAL)
    Q_PROPERTY(int packageIndex READ getPackageIndex NOTIFY packagedIndexChanged FINAL)
    Q_PROPERTY(App *appSource READ appSource FINAL CONSTANT)

public:
    explicit Package(App *appSource, QObject *parent = nullptr);
    void stop();

    void setPackageIndex(const int &index);
    void setProgress(const int &progress);

    int getPackageIndex() const;
    QUrl getLink() const;
    QVariantMap getPackage() const;

    QUrl getPath() const;

    App *appSource() const;

public slots:
    void integratePackage(const QString &path);
    void updatePackage();
    void removePackage();
    void installPackage();
    void launchPackage();
    void buyPackage();
    bool isRunning() const;
    bool isFinished() const;

private:
    QVariantMap m_package; //the actual package from the app to perform action upon

    QUrl m_link; //download link of the actual package
    int m_progress = 0; //percent from 0 to 100 on the current action(mode) being performed on the package
    int m_packageIndex; //the index of the actual package from the app

    App *m_appSource;
    FMH::Downloader *m_downloader;

    QUrl m_path;

    void setPath(const QString &path);

signals:
    void progressChanged(int percent);
    void packagedIndexChanged(int packageIndex);
    void linkChanged(QUrl link);
    void packageChanged(QVariantMap package);
    void progressFinished();
    void progressError(QString error);
    void pathChanged(QUrl path);
    void isRunningChanged();
    void isFinishedChanged();
};


#endif // PACKAGE_H
