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
    property string progress_message: task_progress_message
    property string progress_value: task_progress_vale
    property string progress_total: task_progress_vale

    RowLayout {
        id: innerLayout
        anchors.left: parent.left
        anchors.leftMargin: 18
        anchors.right: parent.right
        anchors.rightMargin: 12

        anchors.verticalCenter: parent.verticalCenter

        PlasmaComponents.Label {
            Layout.alignment: Qt.AlignTop
            Layout.preferredWidth: 200
            Layout.maximumWidth: 200
            wrapMode: Text.WordWrap

            text: task_description
        }
    }
}
