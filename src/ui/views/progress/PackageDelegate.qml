import QtQuick 2.10
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.7 as Kirigami
import org.mauikit.controls 1.0 as Maui

Maui.ItemDelegate
{
    id: control
    readonly property color altColor : Kirigami.Theme.textColor
    implicitHeight: _layout.implicitHeight

    ColumnLayout
    {
        id: _layout
        width: parent.width

        Maui.ListItemTemplate
        {
            Layout.fillWidth: true

            label1.text: model.item.info.name
            label2.wrapMode: Text.WrapAnywhere
            label2.text:  model.item.package.name
            imageSource: model.item.images[0].pic

            ToolButton
            {
                icon.name: "process-stop"
                visible: model.item.progress < 100
            }

            ToolButton
            {
                icon.name: "tab-close"
                visible: model.item.progress === 100
                onClicked: {

                }
            }
        }

        ProgressBar
        {
            Layout.fillWidth: true
            value: model.item.progress
            visible: model.item.progress < 100
            from: 0
            to: 100
        }
    }
}


