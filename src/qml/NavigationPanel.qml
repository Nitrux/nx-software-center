import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

PlasmaCore.FrameSvgItem {
    property alias query: searchField.text

    signal goHome
    signal goStore
    signal goSettings
    signal storeQueryTyped(var query)

    imagePath: "opaque/widgets/panel-background"
    enabledBorders: PlasmaCore.FrameSvgItem.BottomBorder

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
            iconName: "nx-software-center"
            Layout.fillHeight: true
            checked: currentView == "store"
            onClicked: {
                goStore()
                currentView = "store"
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
