import QtQuick 2.0
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

Item {
    height: 48
    width: parent.width

    property alias record_timestamp: labelTimestamp.text
    property alias record_message: labelMessage.text

    property alias app_icon: icon.source
    property alias app_name: labelName.text
    property alias app_version: labelVersion.text

    PlasmaCore.FrameSvgItem {
        imagePath: "opaque/widgets/panel-background"
        enabledBorders: PlasmaCore.FrameSvgItem.TopBorder

        id: background
        anchors.fill: parent
    }

    GridLayout {
        id: innerLayout
        anchors.fill: parent
        anchors.topMargin: 6
        anchors.bottomMargin: 4

        columns: 3
        rows: 2

        rowSpacing: 0
        columnSpacing: 0

        PlasmaCore.IconItem {
            id: icon
            Layout.rowSpan: 2
            Layout.fillHeight: true
            Layout.margins: 0
        }

        PlasmaComponents.Label {
            id: labelName
            Layout.fillWidth: true
            elide: Text.ElideRight

            font.bold: true
            font.pointSize: 9
        }

        PlasmaComponents.Label {
            id: labelMessage
            Layout.alignment: Qt.AlignRight
            elide: Text.ElideRight
            font.pointSize: 9

            visible: text != ""
        }

        PlasmaComponents.Label {
            id: labelVersion
            Layout.fillWidth: true
            elide: Text.ElideRight
            font.pointSize: 9

            visible: text != ""
        }

        PlasmaComponents.Label {
            id: labelTimestamp
            Layout.alignment: Qt.AlignRight
            elide: Text.ElideRight
            font.pointSize: 9
        }
    }
}
