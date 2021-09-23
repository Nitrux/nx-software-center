import QtQuick 2.7
import QtQuick.Controls 2.5

import org.kde.kirigami 2.7 as Kirigami
import org.mauikit.controls 1.3 as Maui
import NXModels 1.0 as NX
import "../../templates"

Maui.Page
{
    id: control

    property bool isActive: true
    property alias manager : _progressManager

    headBar.visible: false

    NX.ProgressManager
    {
        id: _progressManager
        onWarning:
        {
            notify("package-x-generic", "Whoops.", message)
            _packageError.title= i18n("Whoops!.")
            _packageError.message = message
            _packageError.iconName = "emblem-warning"
            _packageError.send()
        }
    }

    Maui.NotifyAction
    {
        id: _launchPackageAction
        text: i18n("Open")
        onTriggered:
        {
            //            console.log("Launch package?". notify.urls[0])
            control.goToApps()
        }
    }

    Maui.Notify
    {
        id: _packageReady
        componentName: "org.nx.softwarecenter"
        eventId: "packageReady"
    }

    Maui.Notify
    {
        id: _packageError
        componentName: "org.nx.softwarecenter"
        eventId: "packageError"
    }

    Maui.Holder
    {
        anchors.fill: parent
        title: i18n("Oops!")
        body: i18n("No apps queued")
        visible: _listView.count === 0
        emoji: "qrc:/download.svg"
        emojiSize: Maui.Style.iconSizes.huge
    }

    Maui.ListBrowser
    {
        id: _listView
        anchors.fill: parent
        orientation: ListView.Vertical
        spacing: Maui.Style.space.medium
        model: _progressManager

        delegate: PackageDelegate
        {
            id: _delegate
            width: ListView.view.width

            onClicked:  model.item.launchPackage()
            onRemoveClicked:
            {
                _progressManager.removePackage(index)
            }

            onStopClicked:
            {
                _progressManager.stopPackage(index)
            }

            Connections
            {
                target: model.item
                function onProgressFinished()
                {
                    root.notify(target.appSource.info.smallpic, target.appSource.info.name, i18n("Your app is ready."),  goToApps, 9500, i18n("Dismiss"))

                    _packageReady.title= target.appSource.info.name
                    _packageReady.message = i18n("Your app is ready.")
                    _packageReady.defaultAction = _launchPackageAction
                    _packageReady.imageSource = target.appSource.info.smallpic
                    _packageReady.send()
                }

                function onProgressError(error)
                {
                    root.notify(target.appSource.info.smallpic, target.appSource.info.name, error,  goToApps, 9500)

                    _packageError.title= target.appSource.info.name
                    _packageError.message = error
                    _packageError.iconSource = "emblem-warning"
                    _packageError.send()
                }
            }
        }
    }

    function goToApps()
    {
        _swipeView.currentIndex = views.apps
    }
}

