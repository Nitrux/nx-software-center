import QtQuick 2.7
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.7 as Kirigami
import org.mauikit.controls 1.0 as Maui

import "../templates"

StackView
{
    id: control

    Component
    {
        id: _devicePageComponent

        DevicePage
        {
            onExit: control.pop()
        }
    }

    initialItem: Maui.Page
    {
        padding: control.depth === 2 ? 0 : Maui.Style.space.big

        headBar.middleContent: [
            Maui.PathBar
            {
                Layout.fillWidth: true
                Layout.leftMargin: Maui.Style.space.big
                Layout.rightMargin: Maui.Style.space.big
                url: "Devices/"

                Maui.TextField
                {
                    visible: false
                }
            }
        ]

        headBar.rightContent: ToolButton
        {
            icon.name: "view-sort"
        }

        ListView
        {
            id: _devicesListview
            anchors.fill: parent
            orientation: ListView.Vertical
            spacing: Maui.Style.space.medium
            section.property: "category"
            section.criteria: ViewSection.FullString
            section.delegate: Maui.LabelDelegate
            {
                id: delegate
                label: section
                labelTxt.font.pointSize: Maui.Style.fontSizes.big

                isSection: true
                labelTxt.font.bold: true
                height: Maui.Style.toolBarHeightAlt
                width: parent.width
            }
            model: ListModel
            {
                ListElement{ category: "Available Devices"; label: "MHDGWUSJDKLL"; iconName: "drive-harddisk";  deviceName: "Samsung 250 GB"; state: "Mounted"; size: 100000}
                ListElement{ category: "Available Devices"; label: "MSBSUSIDH82837HDN"; iconName: "drive-harddisk"; deviceName: "Samsung 250 GB"; state: "Mounted"; size: 80000}
                ListElement{ category: "Available Devices"; label: "KSHSGDNC-928738NDMD"; iconName: "drive-harddisk"; deviceName: "Samsung 250 GB"; state: "Mounted"; size: 20000}
            }

            delegate: CardDelegate
            {
                id: _delegate
                height: 100
                width: control.width - Maui.Style.space.huge
                anchors.horizontalCenter: parent.horizontalCenter
                label1.text: model.label
                label2.text: model.deviceName
                label3.text: model.state
                iconImg.source: model.iconName
                iconImg.width: Maui.Style.iconSizes.huge

                quickButtons: [
                    Button
                    {
                        text: qsTr("Manage")
                        Kirigami.Theme.textColor: "#37474F"
                        Kirigami.Theme.backgroundColor: Qt.rgba("#546E7A".r, "#546E7A".g, "#546E7A".b, 0.2)
                    },

                    Button
                    {
                        text: qsTr("Initialize")
                        Kirigami.Theme.textColor: Kirigami.Theme.negativeTextColor
                        Kirigami.Theme.backgroundColor: Qt.rgba(Kirigami.Theme.negativeTextColor.r, Kirigami.Theme.negativeTextColor.g, Kirigami.Theme.negativeTextColor.b, 0.2)

                    }

                ]

                Connections
                {
                    target: _delegate
                    onClicked:
                    {
                        control.push(_devicePageComponent)

                        //for testing the model this sia custom info model
                        var deviceInfo = _devicesListview.model.get(_devicesListview.currentIndex)
                        control.currentItem.deviceInfo = deviceInfo
                    }
                }

            }
        }
    }

}
