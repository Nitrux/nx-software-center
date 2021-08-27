import QtQuick 2.7
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.7 as Kirigami
import org.mauikit.controls 1.3 as Maui

import QtGraphicalEffects 1.0

import "ui/views/apps"
import "ui/views/store"
import "ui/views/progress"
import "ui/templates"
import "ui"

Maui.ApplicationWindow
{
    id: root
    altHeader: Kirigami.Settings.isMobile
    headBar.visible: false

    StoreView
    {
        id: _storeView
        anchors.fill: parent

        frontPage.headBar.rightContent: [
            ToolButton
            {
                icon.name: "package"
                onClicked: _storeView.push(_appsView)
            },
            ToolButton
            {
                id: _progressViewButton
                icon.name: "download"
                onClicked: _storeView.push(_progressView)
            }
        ]

        AppsView
        {
            id: _appsView
            visible: StackView.status === StackView.Active
            headBar.farLeftContent: ToolButton
            {
                icon.name: "go-previous"
                onClicked: _storeView.pop()
                text: _storeView.get(_appsView.StackView.index-1, StackView.DontLoad).title
                display: isWide ? ToolButton.TextBesideIcon : ToolButton.IconOnly
            }
        }

        ProgressView
        {
            id: _progressView
            visible: StackView.status === StackView.Active
            headBar.farLeftContent: ToolButton
            {
                icon.name: "go-previous"
                onClicked: _storeView.pop()
                text: _storeView.get(_progressView.StackView.index-1, StackView.DontLoad).title
                display: isWide ? ToolButton.TextBesideIcon : ToolButton.IconOnly
            }
        }
    }
}
