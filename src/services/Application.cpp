#include "Application.h"
#include "ApplicationBundle.h"

#include <QDebug>
#include <QVariant>
#include <QVersionNumber>
#include <QtCore>
#include <utility>

Application::Application()
    : _id()
    , _mainBundle(){};

Application::Application(ApplicationData data)
    : _id(data.getId())
    , _data(std::move(data))
    , _mainBundle(){};

Application::Application(const ApplicationBundle &bundle)
    : _id(bundle.data.getId())
    , _data(bundle.data)
    , _mainBundle(bundle)
    , _bundles({bundle})
{
}

bool Application::operator==(const Application &rhs) const
{
    return _id == rhs._id;
}

bool Application::operator!=(const Application &rhs) const
{
    return !(rhs.getId() == this->getId());
}

ApplicationBundle Application::getMainBundle() const
{
    return _mainBundle;
}

QString Application::getId() const
{
    return _id;
}
const ApplicationData &Application::getData() const
{
    return _data;
}
void Application::addBundle(const ApplicationBundle &bundle)
{
    const int mainBundleIdx = _bundles.indexOf(_mainBundle);
    const int i = findInsertIndex(bundle);

    if (i >= 0 && i < _bundles.length() && _bundles[i].path == bundle.path)
        _bundles.removeAt(i);

    _bundles.insert(i, bundle);

    if (i == mainBundleIdx || mainBundleIdx == -1)
        setMainBundle(bundle);
}
void Application::removeBundle(const ApplicationBundle &bundle)
{
    _bundles.removeOne(bundle);

    // update main bundle and data
    if (_mainBundle == bundle) {
        if (_bundles.length() == 0)
            setMainBundle({});
        else
            setMainBundle(_bundles[0]);
    }
}
QList<ApplicationBundle> Application::getBundles() const
{
    return _bundles;
}
void Application::setBundles(QList<ApplicationBundle> bundles, int mainBundleIdx)
{
    if (!bundles.empty()) {
        _bundles = std::move(bundles);
        std::sort(_bundles.begin(), _bundles.end());

        // fix mainBundleIdx
        if (mainBundleIdx < 0 || mainBundleIdx >= _bundles.length()) {
            qWarning() << __FUNCTION__ << " invalid main bundle index. Reset to default";
            mainBundleIdx = 0;
        }

        // update main bundle and data
        const auto &mainBundle = _bundles[mainBundleIdx];
        _data.copy(mainBundle.data);
        _id = _data.getId();
    }
}
void Application::setMainBundle(const ApplicationBundle &bundle)
{
    _mainBundle = bundle;
    _data.copy(_mainBundle.data);

    if (_id.isEmpty())
        _id = _data.getId();
}

int Application::getMainBundleIndex() const
{
    return _bundles.indexOf(_mainBundle);
}
void Application::setMainBundleIndex(int i)
{
    if (i >= 0 && i < _bundles.length())
        setMainBundle(_bundles[i]);
}
int Application::findInsertIndex(const ApplicationBundle &bundle)
{
    int i = 0;
    while (i < _bundles.length() && _bundles[i] > bundle)
        i++;

    return i;
}
