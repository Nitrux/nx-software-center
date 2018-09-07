import QtQuick 2.0
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

PlasmaComponents.ListItem {
    id: itemRoot
    height: 48
    width: parent.width

    property string id: task_id
    property string type: task_type

    property alias app_icon: icon.source
    property alias app_name: labelName.text
    property alias app_author: labelAuthor.text

//    property string description: task_description
    property string status: task_status

    property alias progress_message: labelProgress.text
    property alias progress_value: progressBar.value
    property alias progress_total: progressBar.maximumValue

    signal requestCancel

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
            Layout.margins: 2
            Layout.maximumWidth: 28

            visible: source != "" && status == Image.Ready
            asynchronous: true
            cache: false
            fillMode: Image.PreserveAspectFit
        }

        PlasmaCore.IconItem {
            id: placeHolderIconImage
            Layout.rowSpan: 2
            Layout.fillHeight: true
            Layout.maximumWidth: 28
            Layout.margins: 0

            source: "package-x-generic"
            visible: !icon.visible
        }

        PlasmaComponents.Label {
            id: labelName
            Layout.leftMargin: 6
            Layout.fillWidth: true
            elide: Text.ElideRight

            font.bold: true
            font.pointSize: 9
        }

        PlasmaComponents.Label {
            id: labelAuthor
            Layout.fillWidth: true
            elide: Text.ElideRight

            font.bold: true
            font.pointSize: 9
        }


        PlasmaComponents.ProgressBar {
            id: progressBar
            Layout.fillHeight: true
        }

        PlasmaComponents.Label {
            id: labelProgress
            Layout.maximumWidth: 300
            Layout.alignment: Qt.AlignRight
            elide: Text.ElideRight
            font.pointSize: 9

            text: task_progress_message
        }


        PlasmaComponents.Button {
            text: i18n("Cancel")
            Layout.alignment: Qt.AlignVCenter
            Layout.maximumWidth: 72
            Layout.maximumHeight: 24
            Layout.rowSpan: 2
            Layout.leftMargin: 18
            Layout.rightMargin: 12

            font.pointSize: 9

            onClicked: {
                enabled = false
                requestCancel()
            }
        }
    }
}
