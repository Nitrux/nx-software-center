import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3

import org.kde.kirigami 2.7 as Kirigami
import org.mauikit.controls 1.0 as Maui

Maui.ItemDelegate
{
    id: control
    readonly property color altColor : Kirigami.Theme.textColor
    implicitHeight: _layout.implicitHeight

    signal removeClicked()
    signal stopClicked()

    ColumnLayout
    {
        id: _layout
        anchors.fill: parent

        Maui.ListItemTemplate
        {
            id: meh
            Layout.fillWidth: true
            label1.text: model.item.appSource.info.name
            label2.wrapMode: Text.WrapAnywhere
            label2.text:  model.item.package.name
            imageSource: model.item.appSource.info.smallpic
            iconSource: "package"
            iconSizeHint: Maui.Style.iconSizes.medium

            ToolButton
            {
                icon.name: "process-stop"
                visible: model.item.running
                onClicked: control.stopClicked()
            }

            ToolButton
            {
                icon.name: "tab-close"
                visible: model.item.finished
                onClicked: control.removeClicked()
            }
        }

        ProgressBar
        {
            Layout.fillWidth: true
            value: model.item.progress
            visible: model.item.running
            from: 0
            to: 100
        }
    }
}


