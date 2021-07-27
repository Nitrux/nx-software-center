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
    headBar.visible: false

    StackView
    {
        id: _stackView
        anchors.fill: parent

        initialItem: StoreView
        {
            id: _storeView

            frontPage.headBar.rightContent: ToolButton
            {
                icon.name: "download"
                onClicked: _stackView.push(_appsView)
            }
        }

        AppsView
        {
            id: _appsView
           visible: StackView.status === StackView.Active
           headBar.farLeftContent: ToolButton
           {
               icon.name: "go-previous"
               onClicked: _stackView.pop()
           }
        }

        ProgressView
        {
            id: _progressView
            visible: StackView.status === StackView.Active
        }
    }
}
