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
    property var taskModel: model

    ColumnLayout
    {
        id: _layout
        anchors.fill: parent

        Maui.ListItemTemplate
        {
            Layout.fillWidth: true
            label1.text: model.title
            label2.wrapMode: Text.WrapAnywhere
            label2.text:  model.subtitle
            imageSource: model.icon
            iconSource: "package"
            iconSizeHint: Maui.Style.iconSizes.medium

            Repeater {
                model: taskModel.actions
                delegate: ToolButton {
                    icon.name: model.icon
                    text: model.label
                    onClicked: model.trigger()
                    enabled: model.isActive

                    Component.onCompleted: {console.log(model)}
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


