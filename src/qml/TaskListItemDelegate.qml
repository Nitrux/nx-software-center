import QtQuick 2.0
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

PlasmaComponents.ListItem {
    height: Math.max(38, innerLayout.height)

    property string id: task_id
    property string type: task_type

    property alias app_icon: icon.source
    property alias app_name: labelName.text
    property alias app_author: labelAuthor.text

    property string description: task_description
    property string status: task_status

    property alias progress_message: labelProgress.text
    property alias progress_value: progressBar.value
    property alias progress_total: progressBar.maximumValue

    signal requestCancel

    RowLayout {
        id: innerLayout
        anchors.left: parent.left
        anchors.leftMargin: 18
        anchors.right: parent.right
        anchors.rightMargin: 12

        anchors.verticalCenter: parent.verticalCenter

        PlasmaCore.IconItem {
            id: icon
            Layout.maximumWidth: 180
            Layout.preferredHeight: 100
            Layout.maximumHeight: 120
            Layout.alignment: Qt.AlignCenter
        }

        ColumnLayout {
            Layout.leftMargin: 8
            Layout.fillWidth: true
            PlasmaComponents.Label {
                id: labelName
                Layout.fillWidth: true
                elide: Text.ElideRight

                font.bold: true
            }

            PlasmaComponents.Label {
                id: labelAuthor
                Layout.fillWidth: true
                elide: Text.ElideRight

                visible: text != ""
            }
        }

        ColumnLayout {
            Layout.rightMargin: 12
            PlasmaComponents.ProgressBar {
                id: progressBar
            }

            PlasmaComponents.Label {
                id: labelProgress
                Layout.alignment: Qt.AlignHCenter
                Layout.maximumWidth: 300
                wrapMode: Text.WordWrap

                text: task_progress_message ? task_progress_message : ""
            }
        }

        PlasmaComponents.Button {
            text: i18n("Cancel")

            onClicked: {
                enabled = false
                requestCancel()
            }
        }
    }
}
