import QtQuick 2.7
import QtQuick.Controls 2.5

import org.kde.kirigami 2.7 as Kirigami
import org.mauikit.controls 1.3 as Maui
import org.maui.nxsc 1.0

import "../../templates"

Maui.Page
{
    id: control

    property bool isActive: true

    headBar.visible: false

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
        model: TasksModel

        delegate: TaskDelegate
        {
            id: _delegate
            width: ListView.view.width
        }
    }

    function goToApps()
    {
        _swipeView.currentIndex = views.apps
    }
}

