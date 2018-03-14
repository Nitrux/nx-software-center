import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

PlasmaCore.FrameSvgItem {
    property alias query: searchField.text

    signal goHome
    signal goStore
    signal goTasks
    signal goSettings
    signal storeQueryTyped(var query)

    imagePath: "opaque/widgets/panel-background"
    enabledBorders: PlasmaCore.FrameSvgItem.BottomBorder

    property alias tasksCount: tasksCount.text

    function enable() {
        storeButton.enabled = true
        searchField.enabled = true
    }

    function disable() {
        storeButton.enabled = false
        searchField.enabled = false
    }

    property string currentView: "store"

    height: 48
    RowLayout {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 20
        anchors.verticalCenter: parent.verticalCenter

        height: 34
        spacing: 12

//        PlasmaComponents.ToolButton {
//            Layout.fillHeight: true

//            iconName: "nx-home"
//            checked: currentView == "home"
//            onClicked: {
//                goHome()
//                currentView = "home"
//            }
//        }

        PlasmaComponents.ToolButton {
            id: storeButton
            iconName: "appimage-store"
            Layout.fillHeight: true
            checked: currentView == "store"
            onClicked: {
                currentView = "store"
                goStore()
            }
        }

        PlasmaComponents.ToolButton {
            id: tasksButton
            iconName: "document-download"
            Layout.fillHeight: true
            checked: currentView == "tasks"
            onClicked: {
                currentView = "tasks"
                goTasks()
            }

            Item {
                height: 18
                width: 18

                anchors.top: parent.top
                anchors.right: parent.right
                anchors.margins: 2
                visible: tasksCount.text != "0"

                Rectangle {
                    anchors.fill: parent
                    anchors.margins: 1
                    radius: width

                    color: "#3DAEE9"
//                    border.color: "#3DAEE9"
//                    border.width: 1

                    SequentialAnimation on color {
                        id: glowAnimation
                        ColorAnimation { to: "white"; duration: 500 }
                        ColorAnimation { to: "#3DAEE9"; duration: 500 }
                    }
                }

                PlasmaComponents.Label {
                    id: tasksCount
                    anchors.centerIn: parent
                    anchors.horizontalCenterOffset: -1
                    anchors.verticalCenterOffset: -1

                    font.bold: true
                    font.pointSize: 8
                    onTextChanged: glowAnimation.start()
                }
            }
        }

        PlasmaComponents.TextField {
            id: searchField
            Layout.fillWidth: true
            Layout.leftMargin: parent.width / 10
            Layout.rightMargin: parent.width / 14
            Layout.minimumWidth: 200
            Layout.alignment: Qt.AlignHCenter
            placeholderText: "Search"
            focus: true

            Keys.onEnterPressed: storeQueryTyped(text)
            Keys.onReturnPressed: storeQueryTyped(text)
        }

//        PlasmaComponents.ToolButton {
//            Layout.alignment: Qt.AlignRight
//            Layout.rightMargin: 12
//            Layout.fillHeight: true
//            iconName: "nx-configure"

//            checked: currentView == "settings"
//            onClicked: {
//                goSettings()
//                currentView = "settings"
//            }

//        }
    }
}
