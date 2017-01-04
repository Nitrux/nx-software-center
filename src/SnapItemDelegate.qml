import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import QtQuick.Controls.Styles.Plasma 2.0 as Styles
import org.kde.plasma.extras 2.0 as PlasmaExtras

import org.nx.softwarecenter 1.0 as SoftwareCenter

Item {
    height: 152
    width: 152

    Rectangle {
        anchors.fill: parent
        color: "lightblue"
        opacity: 0.3
    }

    GridLayout {
        anchors.fill: parent
        columns: 2
        rowSpacing: 0
        columnSpacing: 0

        PlasmaCore.IconItem {
            Layout.columnSpan: 2
            Layout.alignment: Qt.AlignRight
            Layout.topMargin: 6
            Layout.rightMargin: 6
            Layout.bottomMargin: 0
            source: {
                if (_status == "active")
                    return "emblem-success"
            }

            Layout.preferredHeight: 24
            Layout.preferredWidth: 24
        }

        PlasmaCore.IconItem {
            Layout.preferredHeight: 64
            Layout.preferredWidth: 64

            Layout.alignment: Qt.AlignHCenter
            Layout.columnSpan: 2
            source: _icon != "" ? _icon : "package-x-generic"
        }

        ColumnLayout {
            Layout.leftMargin: 4
            spacing: 0
            PlasmaComponents.Label {
                text: _name
                font.bold: true
                font.italic: true
                font.pointSize: 8
            }
            PlasmaComponents.Label {
                text: "Version: " + _version
                font.pointSize: 8
            }
            PlasmaComponents.Label {
                text: (_installed_size / 1024 * 1024) + " MiB"
                font.pointSize: 8
            }
        }

        PlasmaComponents.Button {
            text: "Remove"
            Layout.preferredHeight: 28
            Layout.preferredWidth: 64

            style: Styles.ButtonStyle {
                label: Item {
                    Label {
                        anchors.centerIn: parent
                        text: control.text
                        font.pointSize: 8
                    }
                }
            }
            onClicked: {
                SoftwareCenter.Snaps.remove(_name)
            }
        }
    }
}
