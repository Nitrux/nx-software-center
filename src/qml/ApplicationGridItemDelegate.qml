import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3

import org.kde.plasma.core 2.0 as PlasmaCore

Item {
    property alias icon: iconImage.source
    property alias name: labelName.text
    property alias version: labelVersion.text
    property alias size: labelSize.text

    property bool installed: false
    property bool upgradable: false
    property bool hasPendingAction: false

    signal requestRun
    signal requestRemove
    signal requestGet
    signal requestUpgrade

    height: 200
    width: 200

    PlasmaCore.FrameSvgItem {
        height: 180
        width: 180
        anchors.centerIn: parent

        imagePath: "opaque/widgets/panel-background"
    }

    ColumnLayout {
        id: contents
        height: 180
        width: 180
        anchors.centerIn: parent

        Image {
            id: iconImage
            Layout.maximumWidth: 180
            Layout.preferredHeight: 100
            Layout.maximumHeight: 120
            Layout.alignment: Qt.AlignCenter
            Layout.margins: 10

            visible: source != "" && status == Image.Ready
        }

        PlasmaCore.IconItem {
            id: placeHolderIconImage
            Layout.maximumWidth: 180
            Layout.preferredHeight: 100
            Layout.maximumHeight: 120
            Layout.alignment: Qt.AlignCenter
            Layout.margins: 10

            source: "package-x-generic"
            visible: !iconImage.visible
        }

        GridLayout {
            columns: 2
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignBottom
            Layout.bottomMargin: 18
            Layout.leftMargin: 12
            Layout.rightMargin: 8

            Label {
                id: labelName
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignBottom
                Layout.columnSpan: 2

                elide: Text.ElideRight

                font.bold: true
                font.pointSize: 8
            }

            Label {
                id: labelVersion
                Layout.fillWidth: true
                elide: Text.ElideRight

                font.pointSize: 8
            }

            Loader {
                id: loader
                Layout.rowSpan: 2
                Layout.maximumWidth: 72
                Layout.preferredHeight: 20
                sourceComponent: installed ? removeButton : getButton
            }

            Label {
                id: labelSize
                Layout.fillWidth: true
                elide: Text.ElideRight

                font.pointSize: 8
            }
        }

        Component {
            id: getButton
            Button {
                enabled: !hasPendingAction
                text: upgradable ? i18n("Upgrade") : i18n("Get")
                font.pointSize: 7

                onClicked: {
                    if (upgradable)
                        requestUpgrade()
                    else
                        requestGet()
                }
            }
        }

        Component {
            id: removeButton
            RowLayout {
                spacing: 0
                Button {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    enabled: !hasPendingAction
                    text: i18n("Run")

                    onClicked: requestRun()
                    font.pointSize: 7
                }
                Button {
                    id: menuButton
                    Layout.maximumWidth: 20
                    Layout.fillHeight: true
                    font.pointSize: 7

                    enabled: !hasPendingAction

                    onClicked: actionsMenu.open()

                    display: AbstractButton.IconOnly
                    padding: 4
                    spacing: 0

                    icon.name: "down-arrow"
                    icon.source: "qrc:/images/down-arrow.png"

                    Menu {
                        id: actionsMenu
                        y: menuButton.height

                        MenuItem {
                            text: i18n("Remove")
                            onTriggered: requestRemove()
                            font.pointSize: 8
                        }
                    }
                }
            }
        }
    }
}
