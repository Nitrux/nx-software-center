import QtQuick 2.7
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.7 as Kirigami
import org.kde.mauikit 1.0 as Maui
import QtGraphicalEffects 1.0
import "../templates"

Maui.Page
{
    id: control
    property var deviceInfo : ({})
    //    property alias buttonActions : _actionButtons.data
    signal exit()
    padding: 0
    onGoBackTriggered: control.exit()

    headBar.leftContent: [
        ToolButton
        {
            icon.name: "go-previous"
            onClicked: control.exit()
        },

        Kirigami.Separator
        {
            Layout.preferredHeight: Kirigami.Units.iconSizes.small
            width:  2
        }
    ]

    Kirigami.ScrollablePage
    {
        anchors.fill: parent

        ColumnLayout
        {
            id: _mainLayout
            spacing: Maui.Style.space.huge
            RowLayout
            {
                Layout.fillWidth: true
                Layout.maximumHeight: 200

                Item
                {
                    Layout.preferredWidth: parent.height
                    Layout.fillHeight: true
                    Layout.margins: Maui.Style.space.medium

                    Kirigami.Icon
                    {
                        id: _icon
                        width: Kirigami.Units.iconSizes.huge
                        height: width
                        source: deviceInfo.iconName
                        anchors.centerIn: parent
                    }
                }


                ColumnLayout
                {
                    Layout.fillWidth: true
                    spacing: 0
                    Label
                    {
                        text: deviceInfo.label
                        visible: text.length
                        Layout.fillWidth: visible
                        font.pointSize: fontSizes.big
                        font.bold: true
                        font.weight: Font.Bold
                        elide: Text.ElideMiddle
                        color: Kirigami.Theme.textColor
                    }

                    Label
                    {
                        text: deviceInfo.deviceName
                        visible: text.length
                        Layout.fillWidth: visible
                        font.pointSize: fontSizes.small
                        font.weight: Font.Light
                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                        elide: Text.ElideRight
                        color: Kirigami.Theme.textColor
                    }

                    Label
                    {
                        text: deviceInfo.state
                        visible: text.length
                        Layout.fillWidth: visible
                        font.pointSize: fontSizes.small
                        font.weight: Font.Light
                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                        elide: Text.ElideRight
                        color: Kirigami.Theme.textColor
                    }
                }


                ColumnLayout
                {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignLeft
                    spacing: 0

                    Column
                    {
                        anchors.fill: parent
                        Repeater
                        {
                            model: _sizeBarRepeater.model

                            Row
                            {
                                spacing: Maui.Style.space.medium
                                Rectangle
                                {
                                    height: Kirigami.Units.iconSizes.small
                                    width: height
                                    color: model.color
                                    radius: 2
                                    border.color: Qt.darker(color)
                                }

                                Label
                                {
                                    text: model.name
                                    visible: text.length
                                    horizontalAlignment: Qt.AlignRight
                                    font.pointSize: fontSizes.small
                                    font.weight: Font.Light
                                    wrapMode: Text.WrapAnywhere
                                    elide: Text.ElideMiddle
                                    color: Kirigami.Theme.textColor
                                }
                            }



                        }
                    }

                }


                Item
                {
                    Layout.fillHeight: true
                    Layout.preferredWidth: Math.max(iconSizes.big, _buttonsRow.implicitWidth)
                    Layout.alignment: Qt.AlignRight
                    Layout.margins: space.big

                    Row
                    {
                        id: _buttonsRow
                        anchors.centerIn: parent
                        spacing: Maui.Style.space.medium
                        Button
                        {
                            text: qsTr("Deploy")
                        }
                    }
                }
            }

            Rectangle
            {
                id: _sizeBar
                color: Kirigami.Theme.backgroundColor
                Layout.fillWidth: true
                Layout.preferredHeight: 100
                radius:  radiusV
                border.color: Qt.tint(Kirigami.Theme.textColor, Qt.rgba("#333".r, "#333".g, "#333".b, 0.7))

                Row
                {
                    id: _sizeBarListView
                    anchors.fill: parent
//                    orientation: ListView.Horizontal
//                    interactive: false
                    spacing: 1
                    anchors.leftMargin: _sizeBar.radius
                    anchors.rightMargin: _sizeBar.radius
                    Repeater
                    {
                        id: _sizeBarRepeater
                        model: ListModel
                        {
                            ListElement {name: "Efi"; size: "800"; color: "#673ab7"}
                            ListElement {name: "Root"; size: "1500"; color: "#009688"}
                            ListElement {name: "Free"; size: "82200"; color: "#26c6da"}
                            ListElement {name: "Applications"; size: "8000"; color: "#ec407a"}
                        }

                        Rectangle
                        {
                            color: model.color
                            width: (_sizeBarListView.width * model.size)/deviceInfo.size
                            height: _sizeBarListView.height - 2

                            anchors.verticalCenter: parent.verticalCenter

                            Label
                            {
                                visible: implicitWidth < parent.width
                                anchors.centerIn: parent
                                font.weight: Font.Bold
                                font.bold: true
                                text: model.name + " " + model.size
                            }
                        }
                    }
                }
            }


            ListView
            {
                id: _deployedSystemsListView
                clip: true
                Layout.fillWidth: true
                Layout.preferredHeight: contentHeight
                orientation: ListView.Vertical
                spacing: Maui.Style.space.medium
                section.property: "category"
                section.criteria: ViewSection.FullString
                section.delegate: Maui.LabelDelegate
                {
                    id: delegate
                    label: section
                    labelTxt.font.pointSize: fontSizes.big

                    isSection: true
                    boldLabel: true
                    height: Maui.Style.toolBarHeightAlt
                }
                model: ListModel
                {
                    ListElement{ category: "Deployed Systems Found"; label: "MHDGWUSJDKLL"; iconName: "drive-harddisk";  deviceName: "Samsung 250 GB"; state: "Mounted"; size: 100000}
                    ListElement{ category: "Deployed Systems Found"; label: "MSBSUSIDH82837HDN"; iconName: "drive-harddisk"; deviceName: "Samsung 250 GB"; state: "Mounted"; size: 80000}
                    ListElement{ category: "Deployed Systems Found"; label: "KSHSGDNC-928738NDMD"; iconName: "drive-harddisk"; deviceName: "Samsung 250 GB"; state: "Mounted"; size: 20000}
                }

                delegate: CardDelegate
                {
                    id: _delegate
                    height: 100
                    width: _deployedSystemsListView.width - Maui.Style.space.huge
                    anchors.horizontalCenter: parent.horizontalCenter
                    label1.text: model.label
                    label2.text: model.deviceName
                    label3.text: model.state
                    iconImg.source: model.iconName
                    iconImg.width: Kirigami.Units.iconSizes.huge

                    quickButtons: [
                        Button
                        {
                            text: qsTr("Update")
                            Kirigami.Theme.textColor: Kirigami.Theme.positiveTextColor
                            Kirigami.Theme.backgroundColor: Qt.rgba(Kirigami.Theme.positiveTextColor.r, Kirigami.Theme.positiveTextColor.g, Kirigami.Theme.positiveTextColor.b, 0.2)
                        },

                        Button
                        {
                            text: qsTr("Remove")
                            Kirigami.Theme.textColor: Kirigami.Theme.negativeTextColor
                            Kirigami.Theme.backgroundColor: Qt.rgba(Kirigami.Theme.negativeTextColor.r, Kirigami.Theme.negativeTextColor.g, Kirigami.Theme.negativeTextColor.b, 0.2)

                        }

                    ]

                    Connections
                    {
                        target: _delegate
                        onClicked:
                        {

                        }
                    }

                }
            }

        }
    }

}
