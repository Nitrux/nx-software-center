import QtQuick 2.7
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.7 as Kirigami
import org.kde.mauikit 1.0 as Maui
import org.kde.mauikit 1.1 as MauiLab

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

    readonly property var views: ({store: 0, apps: 1, /*system: 2,*/ search: 2, progress: 3})
    property int currentView: views.store

//    altHeader: true
//    page.headerBackground.color: "transparent"
//    Maui.FloatingButton
//    {
//        id: _overlayButton
//        visible: _progressView.isActive
//        z: 999
//        anchors.right: parent.right
//        anchors.bottom: parent.bottom
//        anchors.margins: Maui.Style.toolBarHeight
//        anchors.bottomMargin: Maui.Style.toolBarHeight
//        height: Maui.Style.toolBarHeight
//        width: height

//        icon.name: "package"
//        icon.source: "qrc:/appimage-store.svg"
//        icon.color: Kirigami.Theme.highlightedTextColor
//        onClicked: _actionGroup.currentIndex = views.progress

//        Maui.Badge
//        {
//            text: String(_progressView.manager.count)
//            anchors
//            {
//                horizontalCenter: parent.right
//                verticalCenter: parent.top
//            }
//        }
//    }

    MauiLab.AppViews
    {
        id: _swipeView
        anchors.fill: parent

        StoreView
        {
            id: _storeView
            MauiLab.AppView.iconName: "nx-software-center"
            MauiLab.AppView.title: qsTr("Store")
//            icon.source: "qrc:/store.svg"

        }

        AppsView
        {
            id: _appsView
            MauiLab.AppView.iconName: "go-home"
            MauiLab.AppView.title: qsTr("Apps")
//            icon.source: "qrc:/nx-home.svg"

        }

        ProgressView
        {
            id: _progressView
            MauiLab.AppView.iconName: "document-download"
            MauiLab.AppView.title: qsTr("Progress")
        }
    }
}
