import QtQuick 2.7
import QtQuick.Controls 2.15

import org.kde.kirigami 2.7 as Kirigami
import org.mauikit.controls 1.3 as Maui

import NXModels 1.0 as NX

import "ui/views/apps"
import "ui/views/store"
import "ui/views/progress"

Maui.ApplicationWindow
{
    id: root
    readonly property var views: ({store: 0, apps: 1, progress: 2})
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

    Maui.AppViews
    {
        id: _swipeView
        anchors.fill: parent
        showCSDControls: true
        altHeader: Kirigami.Settings.isMobile

        Maui.AppViewLoader
        {
            Maui.AppView.iconName: "nx-software-center"
            Maui.AppView.title: i18n("Store")

            StoreView {}
        }

        Maui.AppViewLoader
        {
            Maui.AppView.iconName: "appimage-store"
            Maui.AppView.title: i18n("Apps")

            AppsView {
                taskManager: taskManagerCtx
            }
        }

        Maui.AppViewLoader
        {
            Maui.AppView.iconName: "document-download"
            Maui.AppView.title: i18n("Progress")

            ProgressView {}
        }
    }
}
