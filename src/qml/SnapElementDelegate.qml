import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import org.nx.softwarecenter 1.0

import "qrc:/scripts/Utils.js" as Utils

import "parts"
import Snapd 1.0

FocusScope {
    id: root
    width: 160
    height: 245

    focus: true
    property string snap_name
    property string snap_version
    property int snap_size
    property bool isDisabled
    property bool isUpgradeable
    property bool isInstalled

    property bool selected: false
    property bool keyboardFocus: false
    property bool hovered: snapElementArea.containsMouse
                           || toggleSelectionArea.containsMouse || keyboardFocus

    signal clicked

    MouseArea {
        id: snapElementArea
        anchors.fill: parent

        propagateComposedEvents: true
        hoverEnabled: true

        onClicked: root.clicked()
    }

    Card {
        id: background
        anchors.fill: parent

        border.color: hovered ? "#3DAEE9" : "#C3C9D6"
        border.width: hovered ? 2 : 1
        color: selected ? "#aae3ff" : "white"

    }

    Item {
        id: snap_icon
        height: 145
        width: 145
        anchors {
            top: background.top
            horizontalCenter: parent.horizontalCenter
            margins: 15
        }

        PlasmaCore.IconItem {
            visible: !model.icon
            anchors.fill: parent
            //            source: "package-available"
            source: "package-x-generic"
        }

        Image {
            visible: model.icon !== undefined
            anchors.fill: parent
            source: model.icon ? model.icon : ""
        }
    }

    PlasmaCore.IconItem {
        visible: hovered
        source: selected ? "emblem-remove" : "emblem-added"
        anchors.top: background.top
        anchors.left: background.left
        anchors.margins: 12
        opacity: toggleSelectionArea.containsMouse ? 1 : 0.3
        height: 32

        MouseArea {
            id: toggleSelectionArea

            propagateComposedEvents: true
            anchors.fill: parent
            hoverEnabled: true

            onClicked: {
                selected = !selected
            }
        }
    }

    RowLayout {
        id: emblemsLayout

        anchors.top: background.top
        anchors.left: background.left
        anchors.right: background.right
        anchors.topMargin: 4
        anchors.rightMargin: 4

        spacing: 0
        height: 24

        PlasmaCore.IconItem {
            Layout.fillWidth: true
        }
        PlasmaCore.IconItem {
            visible: isDisabled
            Layout.preferredHeight: emblemsLayout.height
            source: "emblem-unavailable"
        }
        PlasmaCore.IconItem {
            visible: isUpgradeable
            Layout.preferredHeight: emblemsLayout.height
            source: "emblem-information"
        }
        PlasmaCore.IconItem {
            visible: isInstalled
            Layout.preferredHeight: emblemsLayout.height
            source: "emblem-success"
        }
    }

    ColumnLayout {
        id: snap_infobox
        anchors {
            //            top: snap_icon.bottom
            bottom: background.bottom
            left: background.left
            right: background.right
            margins: 12
            rightMargin: 6
            bottomMargin: 6
        }
        clip: true
        spacing: 0

        PlasmaComponents.Label {
            text: snap_name
            elide: Text.ElideRight
            maximumLineCount: 1
            font.weight: Font.Medium
            font.pointSize: 12
        }

        PlasmaComponents.Label {
            text: i18n("Version: ") + snap_version
            font.pointSize: 10
            //            font.italic: true
            opacity: 0.7
        }
        PlasmaComponents.Label {
            property string sizeString: Utils.formatSize(snap_size)
            text: snap_size ? sizeString : i18n("Unknown size")
            Layout.fillHeight: true
            Layout.topMargin: 6
        }
    }

    Rectangle {
        width: snap_infobox.height
        height: 70
        anchors.verticalCenter: snap_infobox.verticalCenter
        anchors.right: snap_infobox.right

        //        anchors.rightMargin: height * -1 + 5
        rotation: 90
        //        color: "blue"
        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: background.color
            }
            GradientStop {
                position: 1.0
                color: "transparent"
            }
        }
    }
}
