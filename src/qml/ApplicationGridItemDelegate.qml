import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

Item {
    property alias icon: iconImage.source
    property alias name: labelName.text
    property alias version: labelVersion.text
    property alias size: labelSize.text

    property bool deployed: false
    property bool upgradable: false
    property bool hasPendingAction: false

    signal requestRun
    signal requestRemove
    signal requestGet
    signal requestUpgrade
    signal requestView

    height: 200
    width: 200

    PlasmaCore.FrameSvgItem {
        height: 180
        width: 180
        anchors.centerIn: parent

        imagePath: "opaque/widgets/panel-background"
    }

    MouseArea {
        anchors.fill: parent
        onReleased: requestView()
    }

    ColumnLayout {
        id: contents
        height: 180
        width: 180
        anchors.centerIn: parent

        Image {
            id: iconImage
            Layout.maximumWidth: 120
            Layout.preferredHeight: 100
            Layout.maximumHeight: 120
            Layout.alignment: Qt.AlignCenter
            Layout.margins: 10

            visible: source != "" && status == Image.Ready
            asynchronous: true
            cache: false
            fillMode: Image.PreserveAspectFit
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

            PlasmaComponents.Label {
                id: labelName
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignBottom

                elide: Text.ElideRight

                font.pointSize: 9
                font.bold: true
            }

            Loader {
                id: loader
                Layout.rowSpan: 3
                Layout.maximumWidth: 72
                Layout.preferredHeight: 20
                sourceComponent: deployed ? removeButton : getButton
            }

            PlasmaComponents.Label {
                id: labelVersion
                Layout.fillWidth: true
                elide: Text.ElideRight

                font.pointSize: 8
                visible: text
            }

            PlasmaComponents.Label {
                id: labelSize
                Layout.fillWidth: true
                elide: Text.ElideRight

                font.pointSize: 8
                visible: text
            }
        }

        Component {
            id: getButton
            PlasmaComponents.Button {
                enabled: !hasPendingAction
                text: upgradable ? i18n("Upgrade") : i18n("Get")
                font.pointSize: 8

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
                PlasmaComponents.Button {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    enabled: !hasPendingAction
                    text: i18n("Run")

                    onClicked: requestRun()
                    font.pointSize: 8
                }

                PlasmaComponents.ToolButton {
                    id: menuButton
                    Layout.maximumWidth: 20
                    Layout.fillHeight: true
                    flat: false

                    enabled: !hasPendingAction

                    PlasmaCore.SvgItem {
                        anchors.fill: parent
                        anchors.margins: 2

                        elementId: "down-arrow"
                        svg: PlasmaCore.Svg {
                            id: svg
                            imagePath: "widgets/arrows"
                        }
                    }

                    onClicked: actionsMenu.open(0, height)

                    Menu {
                        id: actionsMenu

                        MenuItem {
                            text: i18n("Remove")
                            onClicked: requestRemove()
                        }

                        MenuItem {
                            text: i18n("Upgrade")
                            onClicked: requestUpgrade()
                            enabled: upgradable
                        }
                    }
                }

            }
        }
    }
}
