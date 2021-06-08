import QtQuick 2.7
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.7 as Kirigami
import org.mauikit.controls 1.3 as Maui

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
    altHeader: Kirigami.Settings.isMobile
    readonly property var views: ({store: 0, apps: 1, progress: 2})
    property int currentView: views.store

    mainMenu: Action
    {
        text: i18n("Settings")
    }

    Maui.AppViews
    {
        id: _swipeView
        anchors.fill: parent

        StoreView
        {
            id: _storeView
            Maui.AppView.iconName: "nx-software-center"
            Maui.AppView.title: qsTr("Store")
        }

        AppsView
        {
            id: _appsView
            Maui.AppView.iconName: "go-home"
            Maui.AppView.title: qsTr("Apps")
        }

        ProgressView
        {
            id: _progressView
            Maui.AppView.iconName: "document-download"
            Maui.AppView.title: qsTr("Progress")
        }
    }
}
