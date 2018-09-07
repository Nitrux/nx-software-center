import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import "parts" as Parts

PlasmaCore.FrameSvgItem {
    property alias query: searchField.text

    signal goHome
    signal goStore
    signal goDeployed
    signal goTasks
    signal goSettings
    signal storeQueryTyped(var query)

    imagePath: "opaque/widgets/panel-background"
    enabledBorders: PlasmaCore.FrameSvgItem.BottomBorder

    property alias tasksCount: tasksCount.value

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

            Parts.CounterEmblem {
                id: tasksCount

                anchors.margins: 2
                anchors.right: parent.right
                anchors.top: parent.top
                height: 18
                width: 18
            }
        }

        PlasmaComponents.ToolButton {
            id: deployedButton
            iconName: "update-none"
            Layout.fillHeight: true
            checked: currentView == "deployed"
            onClicked: {
                currentView = "deployed"
                goDeployed()
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
    }
}
