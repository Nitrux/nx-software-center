import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

import org.kde.plasma.components 2.0 as PlasmaComponents

Item {
    property alias query: searchField.text

    signal goHome
    signal goStore
    signal goSettings
    signal storeQueryTyped(var query)

    property string currentView: "home"

    height: 48
    RowLayout {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter

        height: 34
        spacing: 12

        PlasmaComponents.ToolButton {
            Layout.leftMargin: 20
            Layout.fillHeight: true

            iconName: "go-home"
            checked: currentView == "home"
            onClicked: {
                goHome()
                currentView = "home"
            }
        }

        PlasmaComponents.ToolButton {
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

        PlasmaComponents.ToolButton {
            Layout.alignment: Qt.AlignRight
            Layout.rightMargin: 12
            Layout.fillHeight: true
            iconName: "configure"

            checked: currentView == "settings"
            onClicked: {
                goSettings()
                currentView = "settings"
            }

        }
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        height: 1
        color: "lightgray"
    }
}
