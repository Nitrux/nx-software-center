import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3

import org.kde.kirigami 2.7 as Kirigami
import org.mauikit.controls 1.0 as Maui
import NXModels 1.0 as NX

Maui.ItemDelegate
{
    id: control
    readonly property color altColor : Kirigami.Theme.textColor
    implicitHeight: _layout.implicitHeight
    visible: modelData.status != NX.Task.DRAFT

    ColumnLayout
    {
        id: _layout
        anchors.fill: parent

        Maui.ListItemTemplate
        {
            id: meh
            Layout.fillWidth: true
            label1.text: modelData.title
            label2.wrapMode: Text.WrapAnywhere
            label2.text:  modelData.subtitle
            imageSource: modelData.icon
            iconSource: "package"
            iconSizeHint: Maui.Style.iconSizes.medium

            Repeater {
                model: modelData.actions
                delegate: ToolButton {
                    icon.name: modelData.icon
                    text: modelData.label
                    onClicked: modelData.trigger()
                    enabled: modelData.isActive
                }
            }
        }

        ProgressBar
        {
            Layout.fillWidth: true
            value: modelData.progress
            visible: modelData.status == NX.Task.ACTIVE
            from: 0
            to: modelData.progressTotal
        }
    }
}


