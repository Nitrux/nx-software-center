import QtQuick 2.7
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.7 as Kirigami
import org.kde.mauikit 1.0 as Maui
import QtGraphicalEffects 1.0

import "ui/views/apps"
import "ui/views/store"
import "ui/views/system"
import "ui/views/search"
import "ui/views/progress"
import "ui/templates"
import "ui"

Maui.ApplicationWindow
{
    id: root

    Maui.App.iconName: "qrc:/nx-software-center.svg"
    Maui.App.description : "NX Software Center distributes AppImages for GNU Linux and APKS for Android"
    Maui.App.webPage: "https://nxos.org/"
    Maui.App.donationPage: "https://www.patreon.com/nitrux"
    Maui.App.reportPage: "https://github.com/Nitrux/nx-software-center/issues/"

    readonly property var views: ({store: 0, apps: 1, system: 2, search: 3, progress: 4})
    property int currentView: views.store

    headBar.middleContent: Maui.ActionGroup
    {
        id: _actionGroup
        Layout.fillHeight: true
        //        Layout.fillWidth: true
        Layout.minimumWidth: implicitWidth
        currentIndex : _swipeView.currentIndex
        onCurrentIndexChanged: _swipeView.currentIndex = currentIndex

        Action
        {
            text: qsTr("Store")
            icon.name: "download"
            icon.source: "qrc:/store.svg"
         //            display: isWide ? ToolButton.TextBesideIcon : ToolButton.TextUnderIcon
        }

        Action
        {
            text: qsTr("Apps")
            icon.name: "go-home"
            icon.source: "qrc:/nx-home.svg"
        }

        Action
        {
            text: qsTr("System")
            icon.name: "drive-harddisk"
            icon.source: "qrc:/start-here.svg"
        }
    }

    Maui.FloatingButton
    {
        id: _overlayButton
        visible: _progressView.isActive
        z: 999
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: Maui.Style.toolBarHeight
        anchors.bottomMargin: Maui.Style.toolBarHeight
        height: Maui.Style.toolBarHeight
        width: height

        icon.name: "package"
        icon.source: "qrc:/appimage-store.svg"
        icon.color: Kirigami.Theme.highlightedTextColor
        onClicked: _actionGroup.currentIndex = views.progress

        Maui.Badge
        {
            text: String(_progressView.manager.count)
            anchors
            {
                horizontalCenter: parent.right
                verticalCenter: parent.top
            }
        }
    }


    sideBar: CategoriesSidebar
    {
        id: _categoriesSidebar
    }

    SwipeView
    {
        id: _swipeView
        anchors.fill: parent
        currentIndex: _actionGroup.currentIndex
        onCurrentIndexChanged: _actionGroup.currentIndex = currentIndex

        interactive: isMobile        

        StoreView
        {
            id: _storeView
        }

        AppsView
        {
            id: _appsView
        }

        SystemView
        {
            id: _systemView
        }

        SearchView
        {
            id: _searchView
        }

        ProgressView
        {
            id: _progressView
        }
    }
}
