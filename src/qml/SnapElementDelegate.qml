import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import org.nx.softwarecenter 1.0

import "qrc:/scripts/Utils.js" as Utils

import Snapd 1.0

FocusScope {
    id: root
    width: 192
    height: 192

    property string snap_name
    property string snap_version
    property int snap_size
    property bool isDisabled
    property bool isUpgradeable
    property bool isInstalled

    property bool selected: false
    property bool hovered: snapElementArea.containsMouse
                           || toggleSelectionArea.containsMouse

    signal clicked();

    MouseArea {
        id: snapElementArea
        anchors.fill: parent

        propagateComposedEvents: true
        hoverEnabled: true

        onClicked: root.clicked();
    }

    Rectangle {
        id: background
        anchors.fill: parent
        anchors.margins: 6
        border.color: hovered ? "#3DAEE9" : "#C3C9D6"
        border.width: hovered ? 2 : 1
        color: selected ? "#aae3ff" : "white"

        radius: 2
        PlasmaCore.IconItem {
            visible: hovered
            source: selected ? "emblem-remove" : "emblem-added"
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 12
            opacity: toggleSelectionArea.containsMouse ? 1 : 0.3

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
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 6
        spacing: 0

        RowLayout {
            id: emblemsLayout

            spacing: 2
            Layout.alignment: Qt.AlignRight

            Layout.topMargin: 10
            Layout.rightMargin: 12

            PlasmaCore.IconItem {
                Layout.preferredHeight: 24
            }

            PlasmaCore.IconItem {
                visible: isDisabled
                Layout.preferredHeight: 24
                source: "emblem-unavailable"

//                PlasmaCore.ToolTipArea {
//                    anchors.fill: parent
//                    mainText: i18n("Snap disabled")
//                }
            }
            PlasmaCore.IconItem {
                visible: isUpgradeable
                Layout.preferredHeight: 24
                source: "emblem-information"

//                PlasmaCore.ToolTipArea {
//                    anchors.fill: parent
//                    mainText: i18n("New version available")
//                }
            }
            PlasmaCore.IconItem {
                visible: isInstalled
                Layout.preferredHeight: 24
                source: "emblem-success"

//                PlasmaCore.ToolTipArea {
//                    anchors.fill: parent
//                    mainText: i18n("Snap already installed")
//                }
            }
        }

        Item {
            id: snap_icon
            Layout.preferredWidth: 64
            Layout.preferredHeight: 64
            Layout.alignment: Qt.AlignHCenter

            PlasmaCore.IconItem {
                visible: !model.icon
                anchors.fill: parent
                source: "package-available"
            }

            Image {
                visible: model.icon !== undefined
                anchors.fill: parent
                source: model.icon ? model.icon : ""
            }

        }


        PlasmaComponents.Label {
            text: snap_name
            elide: Text.ElideRight
            maximumLineCount: 1
            wrapMode: Text.WrapAnywhere
            Layout.topMargin: 14
            Layout.leftMargin: 12
            Layout.maximumWidth: 140
            //            font.bold: true
        }

        PlasmaComponents.Label {
            text: i18n("Version: ") + snap_version
            Layout.leftMargin: 12
            font.italic: true
        }
        PlasmaComponents.Label {
            property string sizeString: Utils.formatSize(snap_size)
            text: snap_size ? sizeString : i18n("Unknown size")
            Layout.leftMargin: 12
            Layout.fillHeight: true
        }
    }
}
