import QtQuick 2.0
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

PlasmaComponents.ListItem {
    id: itemRoot
    height: 48
    width: parent.width

    property alias changeslog_message: changesLogMessage.text

    property alias icon: icon.source
    property alias name: labelName.text
    property alias version: labelVersion.text
    property alias dontHavePendingTasks: itemRoot.visible

    signal requestUpgrade()

    PlasmaCore.FrameSvgItem {
        id: background
        imagePath: "opaque/widgets/panel-background"
        enabledBorders: PlasmaCore.FrameSvgItem.TopBorder

        anchors.fill: parent
    }

    GridLayout {
        id: innerLayout
        anchors.fill: parent
        anchors.topMargin: 8
        anchors.bottomMargin: 4

        columns: 4
        rows: 2

        flow: GridLayout.TopToBottom

        Image {
            id: icon
            Layout.rowSpan: 2
            Layout.fillHeight: true
            Layout.margins: 0
            Layout.maximumHeight: 32
            Layout.maximumWidth: 32
            Layout.leftMargin: 12
            Layout.rightMargin: 12
        }


        PlasmaComponents.Label {
            id: labelName
            Layout.fillWidth: true
            elide: Text.ElideRight

            font.bold: true
            font.pointSize: 9
        }

        PlasmaComponents.Label {
            id: labelVersion
            Layout.fillWidth: true
            elide: Text.ElideRight

            font.bold: true
            font.pointSize: 9
        }


        PlasmaComponents.Label {
            id: changesLogMessage
            Layout.alignment: Qt.AlignRight
            Layout.rowSpan: 2

            font.pointSize: 9

            elide: Text.ElideRight
            wrapMode: Text.WordWrap
        }


        PlasmaComponents.Button {
            text: i18n("Update")
            Layout.alignment: Qt.AlignVCenter
            Layout.maximumWidth: 72
            Layout.maximumHeight: 24
            Layout.rowSpan: 2
            Layout.leftMargin: 18
            Layout.rightMargin: 12

            font.pointSize: 9

            onClicked: requestUpgrade()
        }
    }
}
