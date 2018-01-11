import QtQuick 2.0
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

PlasmaComponents.ListItem {
    height: Math.max(38, innerLayout.height)

    property string id: task_id
    property string type: task_type
    property string description: task_description
    property string status: task_status
    property var progress_message: task_progress_message
    property var progress_value: task_progress_value
    property var progress_total: task_progress_total

    RowLayout {
        id: innerLayout
        anchors.left: parent.left
        anchors.leftMargin: 18
        anchors.right: parent.right
        anchors.rightMargin: 12

        anchors.verticalCenter: parent.verticalCenter

        PlasmaComponents.Label {
            Layout.alignment: Qt.AlignTop

            wrapMode: Text.WordWrap

            text: task_description
        }

        ColumnLayout {
            PlasmaComponents.ProgressBar {
                id: progressBar

                value: progress_value ? progress_value : 0
                maximumValue: progress_total ? progress_total : 0
            }

            PlasmaComponents.Label {
                wrapMode: Text.WordWrap

                text: task_progress_message ? task_progress_message : ""
            }
        }
    }
}
