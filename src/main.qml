import QtQuick 2.7
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.7 as Kirigami
import org.mauikit.controls 1.3 as Maui

import "ui/views/apps"
import "ui/views/store"
import "ui/views/progress"
import "ui/templates"
import "ui"

Maui.ApplicationWindow
{
    id: root
    readonly property var views: ({store: 0, apps: 1, progress: 2})
    headBar.visible: false

    Maui.AppViews
    {
        id: _swipeView
        anchors.fill: parent
        showCSDControls: true
        altHeader: Kirigami.Settings.isMobile

        headBar.leftContent: Maui.ToolButtonMenu
        {
            icon.name: "application-menu"

            MenuItem
            {
                text: i18n("Settings")
                icon.name: "settings-configure"
                onTriggered:
                {
                }
            }

            MenuItem
            {
                text: i18n("About")
                icon.name: "documentinfo"
                onTriggered: root.about()
            }
        }

        StoreView
        {
            id: _storeView
            Maui.AppView.iconName: "nx-software-center"
            Maui.AppView.title: i18n("Store")
        }

        AppsView
        {
            id: _appsView
            Maui.AppView.iconName: "appimage-store"
            Maui.AppView.title: i18n("Apps")
        }

        ProgressView
        {
            id: _progressView
            Maui.AppView.iconName: "document-download"
            Maui.AppView.title: i18n("Progress")
        }
    }
}
