import QtQuick 2.0
import QtQuick.Layouts 1.3

import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.extras 2.0 as PlasmaExtras

import org.nxos.softwarecenter 1.0

Flickable {
    contentHeight: tasksViewRoot.height

    flickableDirection: Flickable.VerticalFlick
    ColumnLayout {
        id: tasksViewRoot
        width: parent.width > 1000 ? 976 : parent.width - 60;
        anchors.horizontalCenter: parent.horizontalCenter

        PlasmaExtras.Heading {
            text: "Running Tasks"
            visible: TasksController.model.rowCount() > 0
            level: 4
        }

        Repeater {
            id: tasksListView

            model: TasksController.model
            delegate: TaskListItemDelegate {
                onRequestCancel: TasksController.cancelTask(task_id)
                app_icon: "package-x-generic"
                app_name: task_application_name
                app_author: task_application_author

                progress_value: task_progress_value
                progress_total: task_progress_total
                progress_message: task_progress_message
            }
        }

        PlasmaExtras.Heading {
            text: "Updates"
            visible: UpgraderController.model.rowCount() > 0
            level: 4
        }

        Repeater {
            id: upgradesListView
            model: UpgraderController.model
            delegate: UpgradeListItemDelegate {
                icon: "package-x-generic"
                name: new_app_name
                version: new_app_version

                changeslog_message: "Newer version available"

                onRequestUpgrade: UpgraderController.upgrade(old_app_id,
                                                             new_app_id)
            }
        }

        RowLayout {
            Layout.fillWidth: true
            visible: RegistryController.model.rowCount() > 0

            PlasmaExtras.Heading {
                Layout.fillWidth: true
                text: "History"
                level: 4
            }

            PlasmaComponents.Button {
                Layout.rightMargin: 18
                iconName: "trash-empty"
                onClicked: RegistryController.clearRecords()
            }
        }

        Repeater {
            id: recordsListView
            model: RegistryController.model
            delegate: TaskRecordItem {
                Layout.topMargin: 12
                app_icon: "package-x-generic"
                app_name: task_application_name
                app_version: task_application_version

                record_timestamp: timestamp
                record_message: message
            }
        }
    }

    // Togle tasks notifications for this view
    Component.onCompleted: NotificationsController.tasksNotificationsEnabled = false
    Component.onDestruction: NotificationsController.tasksNotificationsEnabled = true
}
