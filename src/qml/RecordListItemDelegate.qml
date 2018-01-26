import QtQuick 2.0
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

PlasmaComponents.ListItem {
    height: Math.max(38, innerLayout.height)

    property alias record_timestamp: labelTimestamp.text
    property alias record_message: labelMessage.text

    property alias app_icon: icon.source
    property alias app_name: labelName.text
    property alias app_version: labelVersion.text

    RowLayout {
        id: innerLayout

        PlasmaCore.IconItem {
            id: icon
            Layout.maximumWidth: 180
            Layout.preferredHeight: 100
            Layout.maximumHeight: 120
            Layout.alignment: Qt.AlignCenter
        }

        ColumnLayout {
            Layout.maximumWidth: 300
            Layout.minimumWidth: 300
            Layout.leftMargin: 8
            Layout.fillWidth: true
            PlasmaComponents.Label {
                id: labelName
                Layout.fillWidth: true
                elide: Text.ElideRight

                font.bold: true
            }

            PlasmaComponents.Label {
                id: labelVersion
                Layout.fillWidth: true
                elide: Text.ElideRight

                visible: text != ""
            }
        }

        ColumnLayout {
            Layout.maximumWidth: 300
            Layout.leftMargin: 8
            Layout.fillWidth: true
            PlasmaComponents.Label {
                id: labelTimestamp
                Layout.fillWidth: true
                elide: Text.ElideRight
            }

            PlasmaComponents.Label {
                id: labelMessage
                Layout.fillWidth: true
                elide: Text.ElideRight

                visible: text != ""
            }
        }
    }
}
