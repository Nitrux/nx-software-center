import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.7 as Kirigami
import org.kde.mauikit 1.0 as Maui

import "views/apps"
import "views/store"
import "views/system"
import "templates"

Maui.ApplicationWindow
{
    id: root

    Maui.App.iconName: "qrc:/nx-software-center.svg"
    Maui.App.description : "NX Software Center distributes AppImages for GNU Linux and APKS for Android"

    readonly property var views: ({apps: 0, store: 1, system: 2, search: 3, progress: 4})
    property int currentView: views.apps

    headBar.middleContent: [
        ToolButton
        {
            text: qsTr("Apps")
            icon.name: "nx-home"
            checkable: true
            checked: currentView === views.apps
            autoExclusive: true
            onClicked: root.currentView = views.apps
        },

        ToolButton
        {
            text: qsTr("Store")
            icon.name: "nx-software-center"
            checkable: true
            checked: currentView === views.store
            autoExclusive: true
            onClicked: root.currentView = views.store
        },

        ToolButton
        {
            text: qsTr("System")
            icon.name: "start-here"
            checkable: true
            checked: currentView === views.system
            autoExclusive: true
            onClicked: root.currentView = views.system
        }
    ]

    Rectangle
    {
        z: 999
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: toolBarHeight
        anchors.bottomMargin: toolBarHeight
        height: toolBarHeight
        width: height

        color: Kirigami.Theme.highlightColor
        radius: radiusV

        Maui.PieButton
        {
            id: addButton
            anchors.fill : parent
            icon.name: "appimage-store"
            icon.color: Kirigami.Theme.highlightedTextColor
            barHeight: parent.height
            alignment: Qt.AlignLeft
            content: [
                ToolButton
                {
                    icon.name: "app-update"
                },
                ToolButton
                {
                    icon.name: "app-download"
                }
            ]
        }
    }

    globalDrawer: CategoriesSidebar
    {
    }

    SwipeView
    {
        anchors.fill: parent
        currentIndex: root.currentView
        onCurrentIndexChanged: root.currentView = currentIndex
        interactive: isMobile

        AppsView
        {
            id: _appsView
        }

        StoreView
        {
            id: _storeView
        }

        SystemView
        {
            id: _systemView
        }

        Maui.Page
        {
            id: _searchView
        }

        Maui.Page
        {
            id: _progressView
        }
    }
}
