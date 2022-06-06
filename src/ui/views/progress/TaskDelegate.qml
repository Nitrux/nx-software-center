import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3

import org.mauikit.controls 1.0 as Maui
import NXModels 1.0 as NX

Maui.ItemDelegate
{
    id: control
    readonly property color altColor : Maui.Theme.textColor
    implicitHeight: _layout.implicitHeight

    ColumnLayout
    {
        id: _layout
        anchors.fill: parent

        Maui.ListItemTemplate
        {
            Layout.fillWidth: true
            label1.text: title
            label2.wrapMode: Text.WrapAnywhere
            label2.text: subtitle
            imageSource: icon
            iconSource: "package"
            iconSizeHint: Maui.Style.iconSizes.medium

            Repeater {
                model: actions
                delegate: ToolButton {
                    icon.name: modelData.icon
                    text: modelData.label
                    enabled: modelData.active
                }
            }
        }

        ProgressBar
        {
            Layout.fillWidth: true
            value: model.current_progress
            from: 0
            to: model.total_progress
        }
    }
}


